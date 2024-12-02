#!/usr/bin/env python3
# vim:et:smarttab:ts=4:sw=4:foldmethod=marker:

# on debian:
# a i gir1.2-gtk-3.0


__NIL = """

http://electronics.stackexchange.com/questions/107869/software-for-workshop-electronic-components-stock-management/107878#107878

Software for workshop - electronic components stock management
up vote


Im looking for reliable software that will help me to manage electronic components, tools, drills, wires in my workshop.

Currently I have probably more than 1000 diffrent components. My brain can't manage this anymore.

My "wishlist":

* preferable free or low cost software

* not too complicated

* locations feature ("where is it" eg. Shelf 1 -> Box 1)

* stock/shopping history - where and when I purchased part and how much I paid

* categories with tree structure

* some functionality to assign part to projects (example: I want to note somewhere that im using MCP3424 in "Acurrate voltometer v2" and "Raspberry Pi Solar Energy Logger" projects, also I want to see full part list for "Acurrate voltometer v2" project)

* client-server architecture would be nice

* (optional wish) functionality that allows me prepare shopping lists (sometimes I forget to order something, I don't want to order just 1 part an pay for the shipping, so I'm planning buying it next time)

EDIT / BOUNTY COMMENT

So far nothing really interesting appeared in answers. Excel or PHP based solutions are not productive enough (too much clicking). zParts is too simple, I can write something like this myself in 2 hours :)



# self.detail_grid.override_background_color( Gtk.StateFlags.NORMAL, Gdk.RGBA( 1.0, 0.7, 0.7 ) )




Detail Field Types:
  text        text field
  number      float number
  integer     integer number
  id          an id belonging to a key of a table
  bool        boolean
  textarea    multiline text field, args=linecount
  code        code field, arge=(group, show deleted, readonly)
  codero      readonly code field




"""

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk, GObject
import sqlite3
import sys
import time
import traceback


# {{{ class Database
class Database:

    # {{{ __init__( self, dbpath )
    def __init__( self, dbpath ):
        """
        Initialize database class.
        """
        self.do_log = True
        self.__db = sqlite3.connect( dbpath + ".sqlite" )
        version = 0
        try:
            version = int( self.quickfetch( """
                select CF_VALUE from CONFIG where CF_NAME = 'DB-Version'
            """ )[0] )
        except:
            pass
        createfunctions = (
            self.__create_v1,
        )
        currentversion = len(createfunctions)
        if version < currentversion:
            for i in range( version, currentversion ):
                print( "updating database to version %d/%d" % (
                        i+1, currentversion ) )
                createfunctions[i]()
            self.__db.execute( """
                update CONFIG set CF_VALUE = ? where CF_NAME = 'DB-Version'
            """, ( "%d" % currentversion, ) )
            self.__db.commit()
    # }}} __init__( self, dbpath )

    # {{{ close( self )
    def close( self ):
        try:
            self.__db.close()
        except:
            traceback.print_exc()
    # }}} close( self )

    # {{{ __create_v1( self )
    def __create_v1( self ):
        """
        Create database version 1.
        """

        # create and initialize table 'CONFIG'
        self.__db.execute( """
            create table CONFIG (
                CF_NAME                 text    not null,
                CF_VALUE                text    not null, 
                constraint CONFIG_PK primary key ( CF_NAME )
            )
        """ )
        self.__db.execute( """
            insert into CONFIG (
                CF_NAME,
                CF_VALUE
            ) values (
                'DB-Version',
                0
            )
        """ )

        # create table 'PART'
        self.__db.execute( """
            create table PART (
                P_ID                    integer primary key,
                P_NAME                  text    not null,
                P_DESCRIPTION           text    not null
            )
        """ )
        self.__db.execute( """
            create unique index PART_NAME_UK on PART ( P_NAME )
        """ )

        # create table 'PARTTAG'
        self.__db.execute( """
            create table PARTTAG (
                P_ID                    integer not null,
                PT_CODE_CD              integer not null,
                PT_GROUP_CD             integer not null,
                PT_PRIMARY              integer not null,
                PT_TEXT                 text    not null,
                PT_AMOUNT               integer not null,
                constraint PARTTAG_PK primary key ( P_ID, PT_CODE_CD )
            )
        """ )
        self.__db.execute( """
            create index PARTTAG_PRIM_IDX on PARTTAG (
                P_ID, PT_GROUP_CD, PT_PRIMARY, P_ID
            )
        """ )
        self.__db.execute( """
            create index PARTTAG_CODE_IDX on PARTTAG (
                PT_CODE_CD, P_ID
            )
        """ )

        # create table 'SUPPLIER'
        self.__db.execute( """
            create table SUPPLIER (
                S_ID                    integer primary key,
                S_DISPLAY_NAME          text    not null,
                S_NAME                  text    not null,
                S_ADDRESS_LINE_1        text    not null,
                S_ADDRESS_LINE_2        text    not null,
                S_ZIP                   text    not null,
                S_CITY                  text    not null,
                S_COUNTRY_CD            integer not null,
                S_HOMEPAGE              text     null
            )
        """ )
        self.__db.execute( """
            create unique index SUPPLIER_DISPLAY_NAME_UK on SUPPLIER (
                S_DISPLAY_NAME
            )
        """ )

        # create table 'PARTSUPPLIER'
        self.__db.execute( """
            create table PARTSUPPLIER (
                P_ID                    integer not null,
                S_ID                    integer not null,
                PS_ORDER_CODE           text    not null,
                PS_LAST_ORDER_DATE      integer not null,
                PS_LAST_ORDER_PRICE     integer not null,
                constraint PARTSUPPLIER_PK primary key ( P_ID, S_ID )
            )
        """ )

        # create table 'PROJECT'
        self.__db.execute( """
            create table PROJECT (
                PR_ID                   integer primary key,
                PR_NAME                 text    not null,
                PR_DESCRIPTION          text    not null,
                PR_HOMEPAGE             text    not null
            )
        """ )
        self.__db.execute( """
            create unique index PROJECT_NAME_UK on PROJECT ( PR_NAME )
        """ )

        # create table 'PROJECTPART'
        self.__db.execute( """
            create table PROJECTPART (
                PR_ID                   integer not null,
                P_ID                    integer not null,
                PRP_COUNT               integer not null,
                constraint PROJECTPART_PK primary key ( PR_ID, P_ID )
            )
        """ )

        # create table 'ORDER'
        self.__db.execute( """
            create table ORDERS (
                O_ID                    integer primary key,
                S_ID                    integer not null,
                O_STATE_CD              integer not null,
                O_SHIPPING_COMPANY      text        null,
                O_TRACKING_NUMBER       text        null
            )
        """ )

        # create table 'ORDERPART'
        self.__db.execute( """
            create table ORDERPART (
                O_ID                    integer not null,
                P_ID                    integer not null,
                OP_COUNT                integer not null,
                OP_PRICE                real    not null,
                OP_CURRENCY_CD          integer not null,
                constraint ORDERPART_PK primary key ( O_ID, P_ID )
            )
        """ )

        # create table 'CONVERSIONRATE'
        self.__db.execute( """
            create table CONVERSIONRATE (
                CR_CURRENCY_FROM_CD     integer not null,
                CR_CURRENCY_TO_CD       integer not null,
                CR_TIMESTAMP            integer not null,
                CR_RATE                 real    not null,
                constraint CONVERSIONRATE_PK primary key ( 
                    CR_CURRENCY_FROM_CD, CR_CURRENCY_TO_CD, CR_CURRENCY_TO_CD
                )
            )
        """ )

        # create table 'DOCUMENT'
        self.__db.execute( """
            create table DOCUMENT (
                D_ID                    integer primary key,
                P_ID                    integer not null,
                P_TYPE_CD               integer not null,
                D_PATH                  text    not null,
                D_COMMENT               text        null
            )
        """ )
        self.__db.execute( """
            create index DOCUMENT_PART_ID_IDX on DOCUMENT (
                P_ID
            )
        """ )


        # ----------------------------------------------
        # create table 'CODES'
        self.__db.execute( """
            create table CODES (
                CD_ID                   integer not null,
                CD_PARENT_ID            integer not null,
                CD_DEPTH                integer not null,
                CD_TEXT                 text    not null,
                CD_UNUSED               integer not null,
                CD_DELETED              integer not null,
                constraint CODES_PK primary key ( CD_ID )
            )
        """ )
        self.__db.execute( """
            create unique index CODES_PARENT_NAME_UK on CODES (
                CD_PARENT_ID, CD_TEXT
            )
        """ )
        self.__db.execute( """
            create table CODESEARCH (
                CD_ID                   integer not null,
                CD_ANCESTOR_ID          integer not null,
                CD_DEPTH_DIFF           integer not null,
                constraint CODESEARCH_PK primary key ( CD_ANCESTOR_ID, CD_ID )
            )
        """ )

        # insert code group 'Category'
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                1, 0, 0, 0, 0, 'Category'
            )
        """ )

        # insert code group 'Location'
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                2, 0, 0, 0, 0, 'Location'
            )
        """ )

        # insert code group 'Order State'
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                3, 0, 0, 0, 0, 'Order State'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                3001, 3, 1, 0, 0, 'Wishlist'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                3002, 3, 1, 0, 0, 'Ordered'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                3003, 3, 1, 0, 0, 'Partially Arrived'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                3004, 3, 1, 0, 0, 'Arrived'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                3005, 3, 1, 0, 0, 'Cancelled'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                3006, 3, 1, 0, 0, 'Lost'
            )
        """ )

        # insert code group 'Currency'
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                4, 0, 0, 0, 0, 'Currency'
            )
        """ )

        # insert code group 'Country'
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                5, 0, 0, 0, 0, 'Country'
            )
        """ )

        # insert code group 'Property'
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                6, 0, 0, 0, 0, 'Property'
            )
        """ )

        # insert code group 'Document Type'
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                7, 0, 0, 0, 0, 'Document Type'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                7000, 7, 1, 0, 0, 'Datasheet'
            )
        """ )

        # insert code group ''
        #self.__db.execute( """
        #    insert into CODES (
        #        CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
        #    ) values (
        #        1, 0, 0, 0, ''
        #    )
        #""" )


        # insert code group 'Spacer' just to keep the lower values unused
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                99999, 0, 0, 1, 1, 'Spacer'
            )
        """ )
        self.testdata()
    # }}} __create_v1( self )

    # {{{ testdata( self )
    def testdata( self ):
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                1000, 1, 1, 0, 0, 'Discrete'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                1001, 1, 1, 0, 0, 'Micros'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                1002, 1, 1, 0, 0, 'Opamp / Comparator'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                1010, 1000, 2, 0, 0, 'Diode'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                1011, 1000, 2, 0, 0, 'Bipolar Transistor'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                1012, 1001, 2, 0, 0, 'AVR'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                2000, 2, 1, 0, 0, 'Office'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                2001, 2000, 2, 0, 0, 'Shelf 1'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                2002, 2000, 2, 0, 0, 'Shelf 2'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                4000, 4, 1, 0, 0, 'INR'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                4001, 4, 1, 0, 0, 'CHF'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                4002, 4, 1, 0, 0, 'USD'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                5000, 5, 1, 0, 0, 'India'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                5001, 5, 1, 0, 0, 'China'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                5002, 5, 1, 0, 0, 'Switzerland'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                6001, 6, 1, 0, 0, 'Voltage'
            )
        """ )
        self.__db.execute( """
            insert into CODES (
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_UNUSED, CD_DELETED, CD_TEXT
            ) values (
                6002, 6, 1, 0, 0, 'Current'
            )
        """ )

        self.__db.execute( """
            insert into PART (
                P_ID, P_NAME, P_DESCRIPTION
            ) values (
                1, 'LM324', 'opamp'
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                1, 1002, 1, 1, '', 0
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                1, 2001, 2, 1, 'Drawer 11', 3
            )
        """ )

        self.__db.execute( """
            insert into PART (
                P_ID, P_NAME, P_DESCRIPTION
            ) values (
                2, 'LM339', 'comparator'
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                2, 1002, 1, 1, '', 0
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                2, 2001, 2, 1, 'Drawer 22', 3
            )
        """ )

        self.__db.execute( """
            insert into PART (
                P_ID, P_NAME, P_DESCRIPTION
            ) values (
                3, '2N3055', 'NPN power ttransistor'
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                3, 1011, 1, 1, '', 0
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                3, 2001, 2, 1, 'Drawer 10', 3
            )
        """ )

        self.__db.execute( """
            insert into PART (
                P_ID, P_NAME, P_DESCRIPTION
            ) values (
                4, 'ATmega48', 'microcontroller'
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                4, 1012, 1, 1, '', 0
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                4, 2001, 2, 1, 'Drawer 21', 3
            )
        """ )

        self.__db.execute( """
            insert into PART (
                P_ID, P_NAME, P_DESCRIPTION
            ) values (
                5, '1N4148', 'diode'
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                5, 1010, 1, 1, '', 0
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                5, 2002, 2, 1, 'Drawer 13', 13
            )
        """ )

        self.__db.execute( """
            insert into PART (
                P_ID, P_NAME, P_DESCRIPTION
            ) values (
                6, '1N4007', 'diode'
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                6, 1010, 1, 1, '', 0
            )
        """ )
        self.__db.execute( """
            insert into PARTTAG (
                P_ID, PT_CODE_CD, PT_GROUP_CD, PT_PRIMARY,
                PT_TEXT, PT_AMOUNT
            ) values (
                6, 2002, 2, 1, 'Drawer 12', 30
            )
        """ )
    # }}} testdata( self )

    # {{{ quickfetch( self, query, args=None, default=None, all=False )
    def quickfetch( self, query, args=None, default=None, all=False ):
        """
        Quickly fetch some values.
        """
        if self.do_log:
            print( "quickfetch( %s )" % query )
            if args != None:
                print( "  args =", args )
        cursor = self.__db.cursor()
        if args != None:
            cursor.execute( query, args )
        else:
            cursor.execute( query )
        result = default
        if all:
            rows = cursor.fetchall()
            if len(rows) > 0:
                result = rows
        else:
            row = cursor.fetchone()
            if row != None:
                result = row
            # +++ check if too many rows?
        cursor.close()
        return result
    # }}} quickfetch( self, query, args=None, default=None, all=False )

    # {{{ fetch( self, query, args )
    def fetch( self, query, args=None ):
        if self.do_log:
            print( "fetch( %s )" % query )
            if args != None:
                print( "  args =", args )
        cursor = self.__db.cursor()
        if args != None:
            cursor.execute( query, args )
        else:
            cursor.execute( query )
        def cursor_iter( cursor ):
            row = cursor.fetchone()
            while row != None:
                yield row
                row = cursor.fetchone()
            cursor.close()
        return cursor_iter( cursor )
    # }}} fetch

    # {{{ execute( self, sql, args )
    def execute( self, sql, args=None ):
        if self.do_log:
            print( "execute( %s )" % sql )
            if args != None:
                print( "  args =", args )
        cursor = self.__db.cursor()
        if args != None:
            cursor.execute( sql, args )
        else:
            cursor.execute( sql )
        cursor.close()
    # }}} execute

    # {{{ begin( self )
    def begin( self ):
        # transactions are started with the first change (i believe)
        pass
    # }}} begin

    # {{{ commit( self )
    def commit( self ):
        if self.do_log:
            print( "commit()" )
        self.__db.commit()
    # }}} commit

    # {{{ rollback( self )
    def rollback( self ):
        if self.do_log:
            print( "rollback()" )
        self.__db.rollback()
    # }}} rollback

    # {{{ escape( self, text )
    def escape( self, text ):
        return text.replace( "'", "''" )
    # }}} escape
# }}} class Database

# {{{ class Code
class Code:

    # {{{ __init__( self, code, parent, depth, text, unused, deleted )
    def __init__( self, code, parent, depth, text, unused, deleted ):
        self.code = code
        self.parent = parent
        self.depth = depth
        self.text = text
        self.unused = unused
        self.deleted = deleted
        self.next = None
        self.child = None
    # }}} __init__( self, code, parent, depth, text, unused, deleted )

    # {{{ index_of( self, code, include_deleted=True, deleted_obj=False )
    def index_of( self, code, include_deleted=True, deleted_obj=False ):
        child = self.child
        index = 0
        while child.code != code:
            if include_deleted or child.deleted == 0:
                index += 1
            child = child.next
        if child == None:
            return -1
        if child.deleted != 0 and not ( include_deleted or deleted_obj ):
            return -1
        return index
    # }}} index_of( self, code, include_deleted=True, deleted_obj=False )

    # {{{ set_value( self, name, value )
    def set_value( self, name, value ):
        setattr( self, name, value )
    # }}} set_value( self, name, value )

    # {{{ get_value( self, name )
    def get_value( self, name ):
        return getattr( self, name )
    # }}} get_value( self, name )
# }}} class Code

# {{{ class Codes
class Codes:

    # {{{ __init__( self, database )
    def __init__( self, database ):
        self.database = database
        self.groups = {}
        self.codes = {}
        self.root = Code( 0, 0, -1, 'Root', 0, 0 )
        self.codes[0] = self.root
        self.models = []
        self.read_all_codes()
    # }}} __init__( self, database )

    # {{{ register_model( self, model )
    def register_model( self, model ):
        self.models.append( model )
    # }}} register_model( self, model )

    # {{{ read_all_codes( self )
    def read_all_codes( self ):
        sql = """
            select
                CD_ID, CD_PARENT_ID, CD_DEPTH, CD_TEXT, CD_UNUSED, CD_DELETED
            from
                CODES
            where
                CD_ID != 99999
            order by
                CD_DEPTH, CD_PARENT_ID, CD_TEXT DESC
        """
        rows = self.database.quickfetch( sql, all=True )
        for code, parent, depth, text, unused, deleted in rows:
            code = Code( code, parent, depth, text, unused, deleted )
            self.add_code_object( code )
        self.print_tree()
    # }}} read_all_codes( self )

    # {{{ add_code_object( self, code )
    def add_code_object( self, code ):
        # print( "add_code_object:", code.code, code.text )
        self.codes[code.code] = code
        parent = self.codes[code.parent]
        if parent.child == None:
            parent.child = code
        elif code.text < parent.child.text:
            code.next = parent.child
            parent.child = code
        else:
            prev = parent.child
            while prev.next != None and code.text >= prev.next.text:
                prev = prev.next
            code.next = prev.next
            prev.next = code
        self.models_insert( code )
    # }}} add_code_object( self, code )

    # {{{ print_tree( self )
    def print_tree( self ):
        print( "print_tree:" )
        self.print_tree_rec( self.root.child, "  " )
        print( "----" )
    # }}} print_tree( self )

    # {{{ print_tree_rec( self, code, indent )
    def print_tree_rec( self, code, indent ):
        while code != None:
            print( indent, code.code, code.text )
            self.print_tree_rec( code.child, indent+"  " )
            code = code.next
    # }}} print_tree_rec( self, code, indent )

    # {{{ get_code( self, code )
    def get_code( self, code ):
        return self.codes[code]
    # }}} get_code( self, code )

    # {{{ get_text( self, code, default=None )
    def get_text( self, code, default=None ):
        try:
            return self.get_code( code ).text
        except:
            if default == None:
                raise
            return default
    # }}} get_text( self, code, default=None )

    # {{{ get_path_texts( self, code, include_group )
    def get_path_texts( self, code, include_group ):
        texts = []
        self.get_path_texts_rec( texts, code, include_group )
        return texts
    # }}} get_path_texts( self, code, include_group )

    # {{{ get_path_texts_rec( self, texts, code, include_group )
    def get_path_texts_rec( self, texts, code, include_group ):
        c = self.codes[code]
        if c.parent != 0:
            self.get_path_texts_rec( texts, c.parent, include_group )
        elif not include_group:
            return
        texts.append( c.text )
    # }}} get_path_texts_rec( self, texts, code, include_group )

    # {{{ update( self, values )
    def update( self, values ):
        self.database.begin()
        try:
            # insert code
            sql = """
                update
                    CODES
                set
                    CD_TEXT = ?,
                    CD_UNUSED = ?,
                    CD_DELETED = ?
                where
                    CD_ID = ?
            """
            code = values["code"]
            codeobj = self.codes[code]
            prev_deleted = codeobj.deleted
            args = [ values["text"], values["unused"], values["deleted"], code ]
            self.database.execute( sql, args )
            # 
            self.database.commit()
            # +++ sort!?!
            codeobj.text = values["text"]
            codeobj.unused = values["unused"]
            codeobj.deleted = values["deleted"]
            self.models_update( codeobj, prev_deleted )
        except:
            traceback.print_exc()
            self.database.rollback()
    # }}} update( self, values )

    # {{{ models_update( self, codeobj, prev_deleted )
    def models_update( self, codeobj, prev_deleted ):
        for model in self.models:
            model.code_updated( codeobj, prev_deleted )
    # }}} models_update( self, codeobj, prev_deleted )

    # {{{ insert( self, values )
    def insert( self, values ):
        self.database.begin()
        try:
            # insert code
            sql = """
                insert into CODES (
                    CD_PARENT_ID, CD_DEPTH, CD_TEXT, CD_UNUSED, CD_DELETED
                ) values (
                    ?, ?, ?, ?, ?
                )
            """
            parent = values["parent"]
            parentobj = self.get_code( parent )
            depth = parentobj.depth + 1
            values["depth"] = depth
            args = [ parent, depth, values["text"],
                    values["unused"], values["deleted"] ]
            self.database.execute( sql, args )
            # find out code ID
            sql = """
                select
                    CD_ID
                from
                    CODES
                where
                    CD_PARENT_ID = ? and
                    CD_TEXT = ?
            """
            args = [ parent, values["text"] ]
            code = self.database.quickfetch( sql, args )[0]
            values["code"] = code
            #
            codeobj = Code( code, parent, depth,
                    values["text"], values["unused"], values["deleted"] )
            # fill search table
            sql = """
                insert into CODESEARCH (
                    CD_ID, CD_ANCESTOR_ID, CD_DEPTH_DIFF
                ) values (
                    ?, ?, ?
                )
            """
            ancestorobj = codeobj
            depthdiff = 0
            while ancestorobj.code != 0:
                args = [ code, ancestorobj.code, depthdiff ]
                self.database.execute( sql, args )
                ancestorobj = self.get_code( ancestorobj.parent )
                depthdiff += 1
            # 
            self.database.commit()
            self.add_code_object( codeobj )
        except:
            traceback.print_exc()
            self.database.rollback()
    # }}} insert( self, values )

    # {{{ models_insert( self, codeobj )
    def models_insert( self, codeobj ):
        for model in self.models:
            model.code_inserted( codeobj )
    # }}} models_insert( self, codeobj )

    # {{{ delete( self, code )
    def delete( self, code ):
        pass
    # }}} delete( self, code )

    # {{{ models_delete( self, codeobj )
    def models_delete( self, codeobj ):
        for model in self.models:
            model.code_deleted( codeobj )
    # }}} models_delete( self, codeobj )
# }}} class Codes

# {{{ class EmptyTreeModel
class EmptyTreeModel( GObject.GObject, Gtk.TreeModel ):

    # {{{ __init__( self  )
    def __init__( self  ):
        GObject.GObject.__init__( self )
    # }}} __init__( self  )

    # {{{ do_get_flags( self )
    def do_get_flags( self ):
        return Gtk.TreeModelFlags.ITERS_PERSIST
    # }}} do_get_flags( self )

    # {{{ do_get_n_columns( self )
    def do_get_n_columns( self ):
        return 5
    # }}} do_get_n_columns( self )

    # {{{ do_get_column_type( self, index )
    def do_get_column_type( self, index ):
        return ( int, int, str, int, int )[index]
    # }}} do_get_column_type( self, index )

    # {{{ do_get_iter( self, path )
    def do_get_iter( self, path ):
        return ( False, None )
    # }}} do_get_iter( self, path )

    # {{{ do_get_iter_root( self )
    def do_get_iter_root( self ):
        return ( False, None )
    # }}} do_get_iter_root( self )

    # {{{ do_get_path( self, treeiter )
    def do_get_path( self, treeiter ):
        return None
    # }}} do_get_path( self, treeiter )

    # {{{ do_get_value( self, treeiter, column )
    def do_get_value( self, treeiter, column ):
        return None
    # }}} do_get_value( self, treeiter, column )

    # {{{ do_iter_next( self, treeiter )
    def do_iter_next( self, treeiter ):
        return ( False, None )
    # }}} do_iter_next( self, treeiter )

    # {{{ do_iter_children( self, treeiter )
    def do_iter_children( self, treeiter ):
        return ( False, None )
    # }}} do_iter_children( self, treeiter )

    # {{{ do_iter_has_child( self, treeiter )
    def do_iter_has_child( self, treeiter ):
        return False
    # }}} do_iter_has_child( self, treeiter )

    # {{{ do_iter_n_children( self, treeiter )
    def do_iter_n_children( self, treeiter ):
        return 0
    # }}} do_iter_n_children( self, treeiter )

    # {{{ do_iter_nth_child( self, treeiter, n )
    def do_iter_nth_child( self, treeiter, n ):
        return ( False, None )
    # }}} do_iter_nth_child( self, treeiter, n )

    # {{{ do_iter_parent( self, treeiter )
    def do_iter_parent( self, treeiter ):
        return ( False, None )
    # }}} do_iter_parent( self, treeiter )

empty_tree_model = EmptyTreeModel()
# }}} class EmptyTreeModel

# {{{ class FilteredCodeTreeModel
class FilteredCodeTreeModel( GObject.GObject, Gtk.TreeModel ):

    # {{{ __init__( self, codes, group=0, show_deleted=False, register=True )
    def __init__( self, codes, group=0, show_deleted=False, register=True ):
        GObject.GObject.__init__( self )
        self.codes = codes
        self.group = group
        self.show_deleted = show_deleted
        if register:
            self.codes.register_model( self )
    # }}} __init__( self, codes, group=0, show_deleted=False, register=True )

    # {{{ do_get_flags( self )
    def do_get_flags( self ):
        return Gtk.TreeModelFlags.ITERS_PERSIST
    # }}} do_get_flags( self )

    # {{{ do_get_n_columns( self )
    def do_get_n_columns( self ):
        return 6
    # }}} do_get_n_columns( self )

    # {{{ do_get_column_type( self, index )
    def do_get_column_type( self, index ):
        return ( int, int, int, str, int, int )[index]
    # }}} do_get_column_type( self, index )

    # {{{ do_get_iter( self, path )
    def do_get_iter( self, path ):
        treeiter = None
        for index in path:
            ok, treeiter = self.do_iter_nth_child( treeiter, index )
            if treeiter == None:
                break
        if treeiter == None:
            return ( False, None )
        return ( True, treeiter )
    # }}} do_get_iter( self, path )

    # {{{ do_get_iter_root( self )
    def do_get_iter_root( self ):
        code = self.codes.get_code( self.group ).child
        if code == None:
            return ( False, None )
        treeiter = Gtk.TreeIter()
        treeiter.user_data = code.code
        return ( True, treeiter )
    # }}} do_get_iter_root( self )

    # {{{ do_get_path( self, treeiter )
    def do_get_path( self, treeiter ):
        if treeiter == None:
            return None
        path = []
        code = self.codes.get_code( treeiter.user_data )
        while code.code != self.group:
            parent = self.codes.get_code( code.parent )
            path.insert( 0, parent.index_of( code.code, self.show_deleted ) )
            code = parent
        return Gtk.TreePath( path )
    # }}} do_get_path( self, treeiter )

    # {{{ do_get_value( self, treeiter, column )
    def do_get_value( self, treeiter, column ):
        if treeiter == None:
            return None
        code = self.codes.get_code( treeiter.user_data )
        if column == 3:
            return code.text
        elif column == 0:
            return code.code
        elif column == 1:
            return code.parent
        elif column == 2:
            return code.depth
        elif column == 4:
            return code.unused
        elif column == 5:
            return code.deleted
        else:
            return None
    # }}} do_get_value( self, treeiter, column )

    # {{{ do_iter_next( self, treeiter )
    def do_iter_next( self, treeiter ):
        if treeiter == None:
            return ( False, None )
        code = self.codes.get_code( treeiter.user_data )
        if code.next == None:
            return ( False, None )
        treeiter.user_data = code.next.code
        return ( True, treeiter )
    # }}} do_iter_next( self, treeiter )

    # {{{ do_iter_children( self, treeiter )
    def do_iter_children( self, treeiter ):
        if treeiter == None:
            return self.do_get_iter_root()
        code = self.codes.get_code( treeiter.user_data )
        if code.child == None:
            return ( False, None )
        treeiter = Gtk.TreeIter()
        treeiter.user_data = code.child.code
        return ( True, treeiter )
    # }}} do_iter_children( self, treeiter )

    # {{{ do_iter_has_child( self, treeiter )
    def do_iter_has_child( self, treeiter ):
        return self.do_iter_n_children( treeiter ) > 0
    # }}} do_iter_has_child( self, treeiter )

    # {{{ do_iter_n_children( self, treeiter )
    def do_iter_n_children( self, treeiter ):
        if treeiter == None:
            code = self.group
        else:
            code = treeiter.user_data
        n = 0
        child = self.codes.get_code( code ).child
        while child != None:
            if self.show_deleted or not child.deleted:
                n += 1
            child = child.next
        return n
    # }}} do_iter_n_children( self, treeiter )

    # {{{ do_iter_nth_child( self, treeiter, n )
    def do_iter_nth_child( self, treeiter, n ):
        if treeiter == None:
            code = self.group
        else:
            code = treeiter.user_data
        child = self.codes.get_code( code ).child
        if child == None:
            return ( False, None )
        while child != None and n > 0:
            child = child.next
            n -= 1
        if child == None:
            return ( False, None )
        treeiter = Gtk.TreeIter()
        treeiter.user_data = child.code
        return ( True, treeiter )
    # }}} do_iter_nth_child( self, treeiter, n )

    # {{{ do_iter_parent( self, treeiter )
    def do_iter_parent( self, treeiter ):
        if treeiter == None:
            return None
        parent = self.codes.get_code( treeiter.user_data ).parent
        if parent == 0 or parent == self.group:
            return ( False, None )
        treeiter = Gtk.TreeIter()
        treeiter.user_data = parent
        return ( True, treeiter )
    # }}} do_iter_parent( self, treeiter )

    # {{{ iter_for_code( self, code )
    def iter_for_code( self, code ):
        treeiter = Gtk.TreeIter()
        treeiter.user_data = code
        return treeiter
    # }}} iter_for_code( self, code )

    # {{{ get_code_by_path( self, treepath )
    def get_code_by_path( self, treepath ):
        return self.get_code_by_iter( self.do_get_iter( treepath )[1] )
    # }}} get_code_by_path( self, treepath )

    # {{{ get_code_by_iter( self, treeiter )
    def get_code_by_iter( self, treeiter ):
        if treeiter == None:
            return None
        return self.codes.get_code( treeiter.user_data )
    # }}} get_code_by_iter( self, treeiter )

    # {{{ get_iter_and_path( self, codeobj, deleted_obj=False )
    def get_iter_and_path( self, codeobj, deleted_obj=False ):
        code = codeobj.code
        path = []
        while codeobj.code != self.group:
            if codeobj.code == 0 and self.group != 0:
                # the specified code is not in this group
                return None, None
            parent = self.codes.get_code( codeobj.parent )
            index = parent.index_of( codeobj.code, self.show_deleted,
                    deleted_obj )
            if index < 0:
                return None, None
            path.insert( 0, index )
            codeobj = parent
        treeiter = Gtk.TreeIter()
        treeiter.user_data = code
        return treeiter, Gtk.TreePath( path )
    # }}} get_iter_and_path( self, codeobj, deleted_obj=False )

    # {{{ code_updated( self, codeobj, prev_deleted )
    def code_updated( self, codeobj, prev_deleted ):
        deleted_obj = False
        if not self.show_deleted and not prev_deleted and codeobj.deleted:
            deleted_obj = True
        treeiter, treepath = self.get_iter_and_path( codeobj, deleted_obj )
        if treeiter:
            if codeobj.deleted:
                self.row_changed( treepath )
            else:
                self.row_changed( treepath, treeiter )
    # }}} code_updated( self, codeobj, prev_deleted )

    # {{{ code_inserted( self, codeobj )
    def code_inserted( self, codeobj ):
        treeiter, treepath = self.get_iter_and_path( codeobj )
        if treeiter:
            self.row_inserted( treepath, treeiter )
    # }}} code_inserted( self, codeobj )

    # {{{ code_deleted( self, codeobj )
    def code_deleted( self, codeobj ):
        treeiter, treepath = self.get_iter_and_path( codeobj )
        if treeiter:
            self.row_deleted( treepath )
    # }}} code_deleted( self, codeobj )
# }}} class FilteredCodeTreeModel

# {{{ class CodeChooser
class CodeChooser( Gtk.Dialog ):

    # {{{ __init__( self, window, codes, group, show_deleted, .. )
    def __init__( self, window, codes, group, show_deleted,
                prev_value, last_chosen ):
        # since this treeview has more bugs than features always
        # create a new dialog and destroy it after using once
        Gtk.Dialog.__init__( self, "Code Chooser", window )
        self.codes = codes
        self.add_buttons( "OK", Gtk.ResponseType.ACCEPT,
                "Cancel", Gtk.ResponseType.REJECT )
        self.set_modal( True )
        self.set_default_size( 500, 400 )
        model = FilteredCodeTreeModel( codes, group, show_deleted, False )
        tree = Gtk.TreeView( empty_tree_model )
        tree.connect( "row-activated", self.row_activated )
        tree.set_model( model )
        renderer = Gtk.CellRendererText()
        column = Gtk.TreeViewColumn( "Code", renderer, text=3 )
        tree.append_column( column )
        tree.set_headers_visible( False )
        sel = tree.get_selection()
        sel.set_mode( Gtk.SelectionMode.BROWSE )
        scroll = Gtk.ScrolledWindow()
        scroll.set_hexpand( True )
        scroll.set_vexpand( True )
        scroll.add( tree )
        scroll.show_all()
        self.vbox.pack_start( scroll, False, True, 3 )
        if group == 0:
            self.set_title( "Choose any code" )
        else:
            text = codes.get_text( group )
            self.set_title( "Choose a %s" % text )
        if prev_value == None:
            print( "prev_value == None" )
            if group in last_chosen:
                prev_value = last_chosen
            else:
                prev_value = model.do_get_value(
                        model.do_get_iter_root()[1], 0 )
                print( "prev_value =", prev_value )
        treeiter = model.iter_for_code( prev_value )
        treepath = model.do_get_path( treeiter )
        tree.expand_to_path( treepath )
        sel.select_path( treepath )
        response = self.run()
        value = prev_value
        if response == Gtk.ResponseType.ACCEPT:
            model, treeiter = sel.get_selected()
            value = model.do_get_value( treeiter, 0 )
            print( "chosen=", value )
            if value != prev_value:
                last_chosen[group] = value
        else:
            value = None
        self.hide()
        self.value = value
    # }}} __init__

    # {{{ row_activated( self, tree, path, column )
    def row_activated( self, tree, path, column ):
        self.response( Gtk.ResponseType.ACCEPT )
    # }}} row_activated( self, tree, path, column )
# }}} class CodeChooser

# {{{ class InputWidget
class InputWidget:

    # {{{ __init__( self )
    def __init__( self ):
        self.widget = None
        self.height = 1
    # }}} __init__( self )

    # {{{ has_value( self )
    def has_value( self ):
        return True
    # }}} has_value( self )

    # {{{ get_value( self )
    def get_value( self ):
        pass
    # }}} get_value( self )

    # {{{ set_value( self, value )
    def set_value( self, value ):
        pass
    # }}} set_value( self, value )
# }}} class InputWidget

# {{{ class InputWidgetGenericText
class InputWidgetGenericText( InputWidget ):

    # {{{ __init__( self, input_purpose )
    def __init__( self, input_purpose ):
        InputWidget.__init__( self )
        self.widget = Gtk.Entry( input_purpose=input_purpose )
    # }}} __init__( self, input_purpose )

    # {{{ get_value( self )
    def get_value( self ):
        return self.widget.get_text()
    # }}} get_value( self )

    # {{{ set_value( self, value )
    def set_value( self, value ):
        if value == None:
            value = ""
        self.widget.set_text( value )
    # }}} set_value( self, value )
# }}} class InputWidgetGenericText

# {{{ class InputWidgetText
class InputWidgetText( InputWidgetGenericText ):

    # {{{ __init__( self )
    def __init__( self ):
        InputWidgetGenericText.__init__( self, Gtk.InputPurpose.FREE_FORM )
    # }}} __init__( self )

    # {{{ has_value( self )
    def has_value( self ):
        return self.widget.get_text() != ""
    # }}} has_value( self )
# }}} class InputWidgetText

# {{{ class InputWidgetNumber
class InputWidgetNumber( InputWidgetGenericText ):

    # {{{ __init__( self )
    def __init__( self ):
        InputWidgetGenericText.__init__( self, Gtk.InputPurpose.NUMBER )
    # }}} __init__( self )

    # {{{ has_value( self )
    def has_value( self ):
        return True
    # }}} has_value( self )

    # {{{ get_value( self )
    def get_value( self ):
        return float( self.widget.get_text() )
    # }}} get_value( self )

    # {{{ set_value( self, value )
    def set_value( self, value ):
        if value == None:
            self.widget.set_text( "" )
        else:
            # +++ nice rounding?
            self.widget.set_text( "%g" % value )
    # }}} set_value( self, value )
# }}} class InputWidgetNumber

# {{{ class InputWidgetInteger
class InputWidgetInteger( InputWidgetGenericText ):

    # {{{ __init__( self )
    def __init__( self ):
        InputWidgetGenericText.__init__( self, Gtk.InputPurpose.DIGITS )
    # }}} __init__( self )

    # {{{ has_value( self )
    def has_value( self ):
        return True
    # }}} has_value( self )

    # {{{ get_value( self )
    def get_value( self ):
        text = self.widget.get_text()
        if text == "":
            return None
        return int( text )
    # }}} get_value( self )

    # {{{ set_value( self, value )
    def set_value( self, value ):
        if value == None:
            self.widget.set_text( "" )
        else:
            self.widget.set_text( "%d" % value )
    # }}} set_value( self, value )
# }}} class InputWidgetInteger

# {{{ class InputWidgetID
class InputWidgetID( InputWidgetInteger ):

    # {{{ __init__( self )
    def __init__( self ):
        InputWidgetInteger.__init__( self )
        self.widget.set_property( "editable", False )
    # }}} __init__( self )
# }}} class InputWidgetID

# {{{ class InputWidgetCheckButton
class InputWidgetCheckButton( InputWidget ):

    # {{{ __init__( self )
    def __init__( self ):
        InputWidget.__init__( self )
        self.widget = Gtk.CheckButton()
    # }}} __init__( self )

    # {{{ has_value( self )
    def has_value( self ):
        return True
    # }}} has_value( self )

    # {{{ get_value( self )
    def get_value( self ):
        if self.widget.get_active():
            return 1
        return 0
    # }}} get_value( self )

    # {{{ set_value( self, value )
    def set_value( self, value ):
        self.widget.set_active( value == 1 )
    # }}} set_value( self, value )
# }}} class InputWidgetCheckButton

# {{{ class InputWidgetTextarea
class InputWidgetTextarea( InputWidget ):

    # {{{ __init__( self, height )
    def __init__( self, height ):
        InputWidget.__init__( self )
        self.height = height
        self.widget = Gtk.Frame()
        self.scroll = Gtk.ScrolledWindow()
        self.scroll.set_hexpand( True )
        self.scroll.set_border_width( 3 )
        # +++ create a nice border like Gtk.Entry has
        self.textview = Gtk.TextView()
        self.textbuffer = self.textview.get_buffer()
        self.scroll.add( self.textview )
        self.widget.add( self.scroll )
    # }}} __init__( self, height )

    # {{{ z__init__( self, height )
    def z__init__( self, height ):
        InputWidget.__init__( self )
        self.height = height
        self.widget = Gtk.ScrolledWindow()
        self.widget.set_hexpand( True )
        self.widget.set_border_width( 3 )
        # +++ create a nice border like Gtk.Entry has
        self.textview = Gtk.TextView()
        self.textbuffer = self.textview.get_buffer()
        self.widget.add( self.textview )
    # }}} z__init__( self, height )

    # {{{ get_value( self )
    def get_value( self ):
        start = self.textbuffer.get_start_iter()
        end = self.textbuffer.get_end_iter()
        return self.textbuffer.get_text( start, end, False )
    # }}} get_value( self )

    # {{{ set_value( self, value )
    def set_value( self, value ):
        if value == None:
            value = ""
        self.textbuffer.set_text( value )
    # }}} set_value( self, value )
# }}} class InputWidgetTextarea

# {{{ class InputWidgetCode
class InputWidgetCode( InputWidget ):

    # {{{ __init__( self, application, group, show_deleted, readonly )
    def __init__( self, application, group, show_deleted, readonly ):
        InputWidget.__init__( self )
        self.application = application
        self.group = group
        self.show_deleted = show_deleted
        self.readonly = readonly
        self.widget = Gtk.Button( label="-" )
        self.widget.connect( "clicked", self.choose_code )
        self.prev_value = None
        self.value = None
    # }}} __init__( self, application, group, show_deleted, readonly )

    # {{{ choose_code( self, combo )
    def choose_code( self, combo ):
        if self.readonly and self.prev_value != None:
            return
        value = self.application.choose_code( self.group,
                    self.show_deleted, self.value )
        if value != None:
            prev_value = self.prev_value
            self.set_value( value )
            self.prev_value = prev_value
    # }}} choose_code( self, combo )

    # {{{ has_value( self )
    def has_value( self ):
        return self.value != None
    # }}} has_value( self )

    # {{{ get_value( self )
    def get_value( self ):
        return self.value
    # }}} get_value( self )

    # {{{ set_value( self, value )
    def set_value( self, value ):
        self.prev_value = value
        self.value = value
        if value == None:
            self.widget.set_label( "-" )
        else:
            code = self.application.codes.get_code( self.value )
            self.widget.set_label( code.text )
    # }}} set_value( self, value )
# }}} class InputWidgetCode

# {{{ class InputWidgetCodeReadonly
class InputWidgetCodeReadonly( InputWidgetInteger ):

    # {{{ __init__( self, application, group, show_deleted )
    def __init__( self, application, group, show_deleted ):
        InputWidgetCode( self, application, group, show_deleted )
    # }}} __init__( self, application, group, show_deleted )

    # {{{ choose_code( self, combo )
    def choose_code( self, combo ):
        pass
    # }}} choose_code( self, combo )
# }}} class InputWidgetCodeReadonly

# {{{ class InputWidgetCodeReadonly2
class InputWidgetCodeReadonly2( InputWidgetInteger ):

    # {{{ __init__( self, application )
    def __init__( self, application ):
        InputWidgetInteger.__init__( self )
        self.application = application
        self.widget.set_property( "editable", False )
    # }}} __init__( self, application )

    # {{{ set_value( self, value )
    def set_value( self, value ):
        if value == None:
            self.widget.set_text( "-" )
        else:
            code = self.application.codes.get_code( value )
            self.widget.set_text( code.text )
    # }}} set_value( self, value )
# }}} class InputWidgetCodeReadonly2

# {{{ class Thing
class Thing:

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ set_value( self, name, value )
    def set_value( self, name, value ):
        setattr( self, name, value )
    # }}} set_value( self, name, value )

    # {{{ get_value( self, name )
    def get_value( self, name ):
        return getattr( self, name )
    # }}} get_value( self, name )

    # {{{ get_value_by_index( self, column, codes )
    def get_value_by_index( self, column, codes ):
        pass
    # }}} get_value_by_index( self, column, codes )
# }}} class Thing

# {{{ class ThingModel
class ThingModel( GObject.GObject, Gtk.TreeModel ):

    # {{{ __init__( self, application, types )
    def __init__( self, application, types ):
        GObject.GObject.__init__( self )
        self.application = application
        self.types = []
        self.objects = []
    # }}} __init__( self, application, types )

    # {{{ clear( self )
    def clear( self ):
        for i in range( len(self.objects)-1, -1, -1 ):
            del self.objects[i]
            self.row_deleted( Gtk.TreePath( [ i ] ) )
    # }}} clear( self )

    # {{{ append( self, obj )
    def append( self, obj ):
        i = len( self.objects )
        self.objects.append( obj )
        treeiter = Gtk.TreeIter()
        treeiter.user_data = i
        self.row_inserted( Gtk.TreePath( [ i ] ), treeiter )
    # }}} append( self, obj )

    # {{{ do_get_flags( self )
    def do_get_flags( self ):
        return Gtk.TreeModelFlags.ITERS_PERSIST | Gtk.TreeModelFlags.LIST_ONLY
    # }}} do_get_flags( self )

    # {{{ do_get_n_columns( self )
    def do_get_n_columns( self ):
        return len( self.types )
    # }}} do_get_n_columns( self )

    # {{{ do_get_column_type( self, index )
    def do_get_column_type( self, index ):
        return len(self.types[index])
    # }}} do_get_column_type( self, index )

    # {{{ do_get_iter( self, path )
    def do_get_iter( self, path ):
        if len(path) != 1:
            return ( False, None )
        i = path[0]
        if i < 0 or i >= len(self.objects):
            return ( False, None )
        treeiter = Gtk.TreeIter()
        treeiter.user_data = i
        return ( True, treeiter )
    # }}} do_get_iter( self, path )

    # {{{ do_get_iter_root( self )
    def do_get_iter_root( self ):
        if len(self.objects) == 0:
            return ( False, None )
        treeiter = Gtk.TreeIter()
        treeiter.user_data = 0
        return ( True, treeiter )
    # }}} do_get_iter_root( self )

    # {{{ do_get_path( self, treeiter )
    def do_get_path( self, treeiter ):
        if treeiter == None:
            return None
        return Gtk.TreePath( [ treeiter.user_data ] )
    # }}} do_get_path( self, treeiter )

    # {{{ get_object( self, treeiter )
    def get_object( self, treeiter ):
        if treeiter == None:
            return None
        if treeiter.user_data < 0 or treeiter.user_data >= len(self.objects):
            return None
        return self.objects[treeiter.user_data]
    # }}} get_object( self, treeiter )

    # {{{ do_get_value( self, treeiter, column )
    def do_get_value( self, treeiter, column ):
        if treeiter == None:
            return None
        if treeiter.user_data < 0 or treeiter.user_data >= len(self.objects):
            return None
        obj = self.objects[treeiter.user_data]
        return obj.get_value_by_index( column, self.application.codes )
    # }}} do_get_value( self, treeiter, column )

    # {{{ do_iter_next( self, treeiter )
    def do_iter_next( self, treeiter ):
        if treeiter == None:
            return ( False, None )
        n = treeiter.user_data + 1
        if n >= len(self.objects):
            return ( False, None )
        treeiter.user_data = n
        return ( True, treeiter )
    # }}} do_iter_next( self, treeiter )

    # {{{ do_iter_children( self, treeiter )
    def do_iter_children( self, treeiter ):
        if treeiter == None:
            return self.do_get_iter_root()
        return ( False, None )
    # }}} do_iter_children( self, treeiter )

    # {{{ do_iter_has_child( self, treeiter )
    def do_iter_has_child( self, treeiter ):
        return False
    # }}} do_iter_has_child( self, treeiter )

    # {{{ do_iter_n_children( self, treeiter )
    def do_iter_n_children( self, treeiter ):
        return 0
    # }}} do_iter_n_children( self, treeiter )

    # {{{ do_iter_nth_child( self, treeiter, n )
    def do_iter_nth_child( self, treeiter, n ):
        if treeiter != None:
            return ( False, None )
        treeiter = Gtk.TreeIter()
        treeiter.user_data = n
        return ( True, treeiter )
    # }}} do_iter_nth_child( self, treeiter, n )

    # {{{ do_iter_parent( self, treeiter )
    def do_iter_parent( self, treeiter ):
        return ( False, None )
    # }}} do_iter_parent( self, treeiter )
# }}} class ThingModel

# {{{ class ThingTab
class ThingTab:

    # {{{ __init__( self, application, name, as_notebook, with_label, parent_notebook )
    def __init__( self, application, name, as_notebook, with_label, parent_notebook ):
        self.as_notebook = as_notebook
        self.tab_label = None
        self.tab_notebook = None
        self.tab_vbox = None
        if with_label or not as_notebook:
            self.tab_vbox = Gtk.Box( orientation=Gtk.Orientation.VERTICAL,
                    spacing=3 )
            self.tab_widget = self.tab_vbox
        if as_notebook:
            self.tab_notebook = Gtk.Notebook()
            if with_label:
                self.tab_label = Gtk.Label( label="Selected:" )
                self.tab_vbox.pack_start( self.tab_label, False, True, 3 )
                self.tab_vbox.pack_start( self.tab_notebook, True, True, 0 )
            else:
                self.tab_widget = self.tab_notebook
        self.widget = self.tab_widget
        parent_notebook.append_page( self.tab_widget, Gtk.Label( label=name ) )
        self.application = application
        self.name = name
        self.detail_inputs = {}
        self.detail_field_names = []
        self.selected_object = None
        self.additional_tabs = []
        self.id_field_name = None
        self.add_search_fields = True
    # }}} __init__( self, application, name, as_notebook, with_label, parent_notebook )

    # {{{ initialize( self )
    def initialize( self ):
        for row in self.get_detail_field_types():
            self.detail_field_names.append( row[0] )
        self.append_page( self.create_search_page(), 'Search', True )
        self.append_page( self.create_detail_page(), 'Detail' )
    # }}} initialize( self )

    # {{{ append_page( self, page, name, search=False )
    def append_page( self, page, name, search=False ):
        if self.as_notebook:
            self.tab_notebook.append_page( page, Gtk.Label( label=name ) )
        elif search:
            self.tab_vbox.pack_end( page, True, True, 0 )
        else:
            self.tab_vbox.pack_start( page, False, True, 3 )
    # }}} append_page( self, page, name, search=False )

    # {{{ add_tab( self, tab )
    def add_tab( self, tab ):
        self.additional_tabs.append( tab )
    # }}} add_tab( self, tab )

    # {{{ create_search_page( self )
    def create_search_page( self ):
        vbox = Gtk.Box( orientation=Gtk.Orientation.VERTICAL, spacing=3 )
        if self.add_search_fields:
            vbox.pack_start( self.create_search_fields(), False, True, 3 )
        vbox.pack_end( self.create_search_table(), True, True, 0 )
        return vbox
    # }}} create_search_page( self )

    # {{{ create_search_fields( self )
    def create_search_fields( self ):
        hbox = Gtk.Box( orientation=Gtk.Orientation.HORIZONTAL, spacing=3 )
        button = Gtk.Button( label="Clear" )
        button.connect( "clicked", self.clear_search_table )
        hbox.pack_end( button, False, True, 3 )
        button = Gtk.Button( label="All" )
        button.connect( "clicked", self.search_table_all )
        hbox.pack_end( button, False, True, 3 )
        button = Gtk.Button( label="Search" )
        button.connect( "clicked", self.search_table_search )
        hbox.pack_end( button, False, True, 3 )
        self.search_table_entry = Gtk.Entry()
        hbox.pack_start( self.search_table_entry, True, True, 3 )
        return hbox
    # }}} create_search_fields( self )

    # {{{ create_search_table( self )
    def create_search_table( self ):
        types = []
        for index, title, typ in self.get_search_table_columns():
            types.append( str )
        self.model = ThingModel( self.application, types )
        self.search_table = Gtk.TreeView( model=self.model )
        for index, title, typ in self.get_search_table_columns():
            renderer = Gtk.CellRendererText()
            column = Gtk.TreeViewColumn( title, renderer, text=index )
            self.search_table.append_column( column )
            "row-activated"
        self.search_table.connect( "row-activated", self.search_row_activated )
        scroll = Gtk.ScrolledWindow()
        scroll.set_hexpand( True )
        scroll.add( self.search_table )
        return scroll
    # }}} create_search_table( self )

    # {{{ search_row_activated( self, treeview, treepath, tvcol )
    def search_row_activated( self, treeview, treepath, tvcol ):
        print( "row activated" )
        treeiter = self.model.get_iter( treepath )
        obj = self.model.get_object( treeiter )
        self.show_object_detail( obj )
    # }}} search_row_activated( self, treeview, treepath, tvcol )

    # {{{ show_object_detail( self, obj )
    def show_object_detail( self, obj ):
        self.selected_object = obj
        for k, v in self.detail_inputs.items():
            v.set_value( obj.get_value( k ) )
        if self.tab_label != None:
            txt = "Selected: " + obj.get_label_text()
            self.tab_label.set_label( txt )
        if self.as_notebook:
            self.tab_notebook.set_current_page( 1 )
        if self.id_field_name != None:
            name = self.id_field_name
            values = { name: obj.get_value( name ) }
            for tab in self.additional_tabs:
                tab.search_related( values )
    # }}} show_object_detail( self, obj )

    # {{{ get_detail_values( self )
    def get_detail_values( self ):
        values = {}
        for k, v in self.detail_inputs.items():
            values[k] = v.get_value()
        return values
    # }}} get_detail_values( self )

    # {{{ create_detail_page( self )
    def create_detail_page( self ):
        self.detail_grid = Gtk.Grid()
        self.detail_grid.set_row_homogeneous( True )
        self.detail_grid.set_row_spacing( 3 )
        self.detail_grid.set_column_spacing( 6 )
        buttons = self.create_standard_buttons( self.detail_clear,
            self.detail_new, self.detail_save )
        self.detail_grid.attach( buttons, 0, 0, 2, 1 )
        y = 1
        for name, label, typ, args in self.get_detail_field_types():
            widget = Gtk.Label( label=label )
            widget.set_xalign( 0.0 )
            self.detail_grid.attach( widget, 0, y, 1, 1 )
            if typ == "text":
                widget = InputWidgetText()
            elif typ == "number":
                widget = InputWidgetNumber()
            elif typ == "integer":
                widget = InputWidgetInteger()
            elif typ == "id":
                widget = InputWidgetID()
            elif typ == "bool":
                widget = InputWidgetCheckButton()
            elif typ == "textarea":
                widget = InputWidgetTextarea( args )
            elif typ == "code":
                widget = InputWidgetCode( self.application,
                        args[0], args[1], args[2] )
            elif typ == "codero":
                widget = InputWidgetCodeReadonly( self.application )
            else:
                raise Exception( "Unknown field type '%s'." % str(typ) )
            self.detail_inputs[name] = widget
            self.detail_grid.attach( widget.widget, 1, y, 1, widget.height )
            y += widget.height
        vbox = Gtk.Box( orientation=Gtk.Orientation.VERTICAL, spacing=3 )
        vbox.set_border_width( 3 )
        vbox.pack_start( self.detail_grid, False, True, 3 )
        vbox.pack_start( Gtk.Label( label=" " ), True, True, 3 )
        return vbox
    # }}} create_detail_page( self )

    # {{{ create_standard_buttons( self, clearfunc, newfunc, savefunc )
    def create_standard_buttons( self, clearfunc, newfunc, savefunc ):
        hbox = Gtk.Box( orientation=Gtk.Orientation.HORIZONTAL, spacing=3 )
        button = Gtk.Button( label="Clear" )
        button.connect( "clicked", clearfunc )
        hbox.pack_start( button, False, False, 0 )
        button = Gtk.Button( label="New" )
        button.connect( "clicked", newfunc )
        hbox.pack_start( button, False, False, 0 )
        button = Gtk.Button( label="Save" )
        button.connect( "clicked", savefunc )
        hbox.pack_start( button, False, False, 0 )
        return hbox
    # }}} create_standard_buttons( self, clearfunc, newfunc, savefunc )

    # {{{ clear_search_table( self, widget )
    def clear_search_table( self, widget ):
        self.clear()
    # }}} clear_search_table( self, widget )

    # {{{ search_table_all( self, widget )
    def search_table_all( self, widget ):
        self.search( { "text": None } )
    # }}} search_table_all( self, widget )

    # {{{ search_table_search( self, widget )
    def search_table_search( self, widget ):
        text = self.search_table_entry.get_text()
        if text == "":
            text = None
        self.search( { "text": text } )
    # }}} search_table_search( self, widget )

    # {{{ has_detail_input_value( self, name )
    def has_detail_input_value( self, name ):
        if not name in self.detail_inputs:
            print( "Name", name, "not in detail inputs." )
        return self.detail_inputs[name].has_value()
    # }}} has_detail_input_value( self, name )

    # {{{ get_detail_input_value( self, name )
    def get_detail_input_value( self, name ):
        return self.detail_inputs[name].get_value()
    # }}} get_detail_input_value( self, name )

    # {{{ set_detail_input_value( self, name, value )
    def set_detail_input_value( self, name, value ):
        return self.detail_inputs[name].set_value( value )
    # }}} set_detail_input_value( self, name, value )

    # {{{ clear( self )
    def clear( self ):
        self.model.clear()
    # }}} clear( self )

    # {{{ detail_clear( self, button )
    def detail_clear( self, button ):
        # clear detail fields
        for i in self.detail_inputs.values():
            i.set_value( None )
        # clear additional tabs
        for tab in self.additional_tabs:
            tab.clear()
            tab.detail_clear( None )
        # unset selected object
        self.selected_object = None
        if self.tab_label != None:
            self.tab_label.set_label( "Selected:" )
    # }}} detail_clear( self, button )

    # {{{ detail_new( self, button )
    def detail_new( self, button ):
        # insert new record
        # clear additional tabs
        types = self.get_db_field_types()
        database = self.application.database
        database.begin()
        try:
            for tablename, cols in types:
                read_rowid = False
                rowid_col = None
                rowid_var = None
                args = []
                sql = "insert into %s ( " % tablename
                vals = " ) values ( "
                pre = ""
                for colname, coltype, usage, value in cols:
                    if usage == "const":
                        # const always has a value
                        pass
                    elif self.has_detail_input_value( value ):
                        # OK, field has a value
                        pass
                    elif coltype == "str" and usage == "field":
                        # field may have an empty string
                        pass
                    else:
                        # error, field must have a value
                        raise Exception( "field %s must have a value" % value )
                    if usage == "rowid":
                        # rowid is set automatically but must be read back
                        read_rowid = True
                        rowid_col = colname
                        rowid_var = value
                    elif usage == "const":
                        # set a constant
                        sql += "%s%s" % ( pre, colname )
                        if coltype == "str":
                            value = "'%s'" % database.escape( value )
                        vals += "%s%s" % ( pre, value )
                        pre = ", "
                    else:
                        # set the field value
                        sql += "%s%s" % ( pre, colname )
                        vals += "%s?" % ( pre, )
                        pre = ", "
                        fieldvalue = self.get_detail_input_value( value )
                        args.append( fieldvalue )
                sql += vals + " )"
                database.execute( sql, args )
                if read_rowid:
                    # read back the rowid
                    args = []
                    sql = "select %s from %s where " % ( rowid_col, tablename )
                    pre = ""
                    for colname, coltype, usage, value in cols:
                        if usage == "uniq":
                            # search the unique key identifying the row
                            sql += "%s%s = ?" % ( pre, colname )
                            fieldvalue = self.get_detail_input_value( value )
                            args.append( fieldvalue )
                            pre = " and "
                    rowid = database.quickfetch( sql, args )
                    self.set_detail_input_value( rowid_var, rowid )
            database.commit()
        except:
            traceback.print_exc()
            # WTF? where's my transaction?
            try:
                database.rollback()
            except:
                pass
            # ++++ meckermecker
            return
        # +++ update selected_object and view
    # }}} detail_new( self, button )

    # {{{ detail_save( self, button )
    def detail_save( self, button ):
        # update record
        types = self.get_db_field_types()
        if self.selected_object == None:
            # no id so it must be a new record
            self.detail_new( button )
            return
        for colname, coltype, usage, value in types[0][1]:
            if usage == "rowid":
                break
        if usage == "rowid" and self.get_detail_input_value( value ) == None:
            # no id so it must be a new record
            self.detail_new( button )
            return
        database = self.application.database
        database.begin()
        try:
            for tablename, cols in types:
                args = []
                sql = "update %s set " % tablename
                pre = ""
                keytype = ( "key", "upd" )
                for colname, coltype, usage, value in cols:
                    if usage in ( "upd", "uniq", "field", ):
                        sql += "%s%s = ?" % ( pre, colname )
                        pre = ", "
                        fieldvalue = self.get_detail_input_value( value )
                        args.append( fieldvalue )
                    if usage == "rowid":
                        keytype = ( usage, )
                    elif usage == "uniq" and keytype != "rowid":
                        keytype = ( usage, )
                sql += " where "
                pre = ""
                for colname, coltype, usage, value in cols:
                    if usage in keytype:
                        sql += "%s%s = ?" % ( pre, colname )
                        fieldvalue = self.selected_object.get_value( value )
                        args.append( fieldvalue )
                        pre = "and "
                database.execute( sql, args )
            database.commit()
        except:
            traceback.print_exc()
            # WTF? where's my transaction?
            try:
                database.rollback()
            except:
                pass
            # ++++ meckermecker
            return
        # +++ update selected_object and view
    # }}} detail_save( self, button )

    # {{{ code_to_text( self, code )
    def code_to_text( self, code ):
        return self.application.codes.get_text( code )
    # }}} code_to_text( self, code )

    # {{{ code_path_to_texts( self, code, include_group )
    def code_path_to_texts( self, code, include_group ):
        return self.application.codes.get_path_texts( code, include_group )
    # }}} code_path_to_texts( self, code, include_group )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        pass
    # }}} get_detail_field_types( self )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        """
        Return a tuple containing table definitions.

        Each table definition is a tuple containing the table name
        followed by a tuple containing the column definitions.

        Each column definition is a tuple containing four strings:
        Column name, column type, usage and parameter.

        Column type is either "int" or "str".

        Usage can be "const", "field", "rowid", "key" or "upd".

        const: A constant field, the value is specified in the parameter.

        field: A normal field, the field name is specified in the parameter.

        rowid: A field containing the rowid, the primary unique key.

        key: A field belonging to the primary key.

        upd: A field belonging to the primary key but which can be changed.
        """
        pass
    # }}} get_db_field_types( self )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        pass
    # }}} get_search_table_columns( self )

    # {{{ search_related( self, values )
    def search_related( self, values ):
        self.search( values )
        for k, v in values.items():
            self.detail_inputs[k].set_value( v )
    # }}} search_related( self, values )

    # {{{ search( self, values )
    def search( self, values ):
        self.clear()
        sql, args = self.get_search_sql( values )
        sql += " LIMIT 100"
        rowiter = self.application.database.fetch( sql, args )
        n = len( self.detail_field_names )
        for row in rowiter:
            obj = self.object_constructor()
            for i in range( 0, n ):
                obj.set_value( self.detail_field_names[i], row[i] )
            self.model.append( obj )
    # }}} search( self, values )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        pass
    # }}} get_search_sql( self, values )
# }}} class ThingTab

# {{{ class Part
class Part( Thing ):

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ get_label_text( self )
    def get_label_text( self ):
        return self.name
    # }}} get_label_text( self )

    # {{{ get_value_by_index( self, index, codes )
    def get_value_by_index( self, index, codes ):
        if index == 1:
            return codes.get_text( self.category_cd )
        if index == 2:
            text = codes.get_text( self.location_cd )
            if self.location_text != "":
                text += ", " + self.location_text
            return text
        if index == 3:
            return self.location_amount
        return self.name
    # }}} get_value_by_index( self, index, codes )
# }}} class Part

# {{{ class PartTab
class PartTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Parts", True, True, notebook )
        self.application = application
        self.object_constructor = Part
        self.id_field_name = "part_id"
        self.initialize()
        self.add_tab( PartPropertyTab( application, self.tab_notebook ) )
        self.add_tab( PartDocumentationTab( application, self.tab_notebook ) )
        self.add_tab( PartCategoryTab( application, self.tab_notebook ) )
        self.add_tab( PartSupplierTab( application, self.tab_notebook ) )
        self.add_tab( PartProjectTab( application, self.tab_notebook ) )
        self.add_tab( PartOrderTab( application, self.tab_notebook ) )
    # }}} __init__( self, application, notebook )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        return (
            ( 0, "Name", "text" ),
            ( 1, "Category", "text" ),
            ( 2, "Location", "text" ),
            ( 3, "Amount", "integer" ),
        )
    # }}} get_search_table_columns( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "part_id", "ID", "id", None ),
            ( "name", "Name", "text", None ),
            ( "descr", "Description", "textarea", 3 ),
            ( "category_cd", "Category", "code", ( 1, False, False ) ),
            ( "location_cd", "Location Code", "code", ( 2, False, False ) ),
            ( "location_text", "Location Text", "text", None ),
            ( "location_amount", "Amount", "integer", None ),
        )
    # }}} get_detail_field_types( self )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        sql = """
                select
                    P.P_ID, P.P_NAME, P.P_DESCRIPTION,
                    CAT.PT_CODE_CD as CATEGORY_CD,
                    LOC.PT_CODE_CD as LOCATION_CD,
                    LOC.PT_TEXT as LOCATION_TEXT,
                    LOC.PT_AMOUNT as LOCATION_AMOUNT
                from
                    PART P,
                    PARTTAG CAT,
                    PARTTAG LOC
                where
                    CAT.P_ID = P.P_ID and
                    CAT.PT_GROUP_CD = 1 and
                    CAT.PT_PRIMARY = 1 and
                    LOC.P_ID = P.P_ID and
                    LOC.PT_GROUP_CD = 2 and
                    LOC.PT_PRIMARY = 1
        """
        text = values["text"]
        if text != None:
            text = self.application.database.escape( text )
            sql += " and P_NAME like '%%%s%%' " % text
        if False:
            text = self.application.database.escape( text )
            sql += """
                    and exists (
                        select
                            P_ID
                        from
                            PARTTAG PT
                        where
                            PT.P_ID = P.P_ID and
                            PT.PT_GROUP_CD = 1 and
                            PT.PT_CODE_CD = 1002
                    )
            """
        return sql, None
    # }}} get_search_sql( self, values )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        return (
            (
                "PART",
                (
                    ( "P_ID",           "int", "rowid", "part_id"  ),
                    ( "P_NAME",         "str", "uniq",  "name" ),
                    ( "P_DESCRIPTION",  "str", "field", "descr" ),
                ),
            ),
            (
                "PARTTAG",
                (
                    ( "P_ID",           "int", "key",   "part_id" ),
                    ( "PT_CODE_CD",     "int", "upd",   "category_cd" ),
                    ( "PT_GROUP_CD",    "int", "const", "1" ),
                    ( "PT_PRIMARY",     "int", "const", "1" ),
                    ( "PT_TEXT",        "str", "const", "" ),
                    ( "PT_AMOUNT",      "int", "const", "0" ),
                ),
            ),
            (
                "PARTTAG",
                (
                    ( "P_ID",           "int", "key",   "part_id" ),
                    ( "PT_CODE_CD",     "int", "upd",   "location_cd" ),
                    ( "PT_GROUP_CD",    "int", "const", "2" ),
                    ( "PT_PRIMARY",     "int", "const", "1" ),
                    ( "PT_TEXT",        "str", "field", "location_text" ),
                    ( "PT_AMOUNT",      "int", "field", "location_amount" ),
                ),
            ),
        )
    # }}} get_db_field_types( self )
# }}} class PartTab

# {{{ class PartProperty
class PartProperty( Thing ):

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ get_value_by_index( self, index, codes )
    def get_value_by_index( self, index, codes ):
        if index == 0:
            return codes.get_text( self.property_cd )
        return self.property_value
    # }}} get_value_by_index( self, index, codes )
# }}} class PartProperty

# {{{ class PartPropertyTab
class PartPropertyTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Properties", False, False, notebook )
        self.application = application
        self.object_constructor = PartProperty
        self.add_search_fields = False
        self.initialize()
    # }}} __init__( self, application, notebook )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        return (
            ( 0, "Property", "text" ),
            ( 1, "Value", "text" ),
        )
    # }}} get_search_table_columns( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "part_id", "Part ID", "id", None ),
            ( "property_cd", "Property", "code", ( 6, False, False ) ),
            ( "property_value", "Value", "text", None ),
        )
    # }}} get_detail_field_types( self )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        sql = """
                select
                    P_ID,
                    PT_CODE_CD as PROPERTY_CD,
                    PT_TEXT as PROPERTY_VALUE
                from
                    PARTTAG
                where
                    P_ID = ? and
                    PT_GROUP_CD = 6
        """
        return sql, [ values["part_id"] ]
    # }}} get_search_sql( self, values )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        return (
            (
                "PARTTAG",
                (
                    ( "P_ID",           "int", "key",   "part_id" ),
                    ( "PT_CODE_CD",     "int", "upd",   "property_cd" ),
                    ( "PT_GROUP_CD",    "int", "const", "6" ),
                    ( "PT_PRIMARY",     "int", "const", "0" ),
                    ( "PT_TEXT",        "str", "field", "property_value" ),
                    ( "PT_AMOUNT",      "int", "const", "0" ),
                ),
            ),
        )
    # }}} get_db_field_types( self )
# }}} class PartPropertyTab

# {{{ class PartDocumentation
class PartDocumentation( Thing ):

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ get_value_by_index( self, index, codes )
    def get_value_by_index( self, index, codes ):
        if index == 0:
            return codes.get_text( self.type_cd )
        return self.path
    # }}} get_value_by_index( self, index, codes )
# }}} class PartDocumentation

# {{{ class PartDocumentationTab
class PartDocumentationTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Documents", False, False, notebook )
        self.application = application
        self.object_constructor = PartDocumentation
        self.add_search_fields = False
        self.initialize()
    # }}} __init__( self, application, notebook )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        return (
            ( 0, "Type", "text" ),
            ( 1, "Path", "text" ),
        )
    # }}} get_search_table_columns( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "id", "Document ID", "id", None ),
            ( "part_id", "Part ID", "id", None ),
            ( "type_cd", "Type", "code", ( 7, False, False ) ),
            ( "path", "Path", "text", None ),
            ( "comment", "Comment", "textarea", 3 ),
        )
    # }}} get_detail_field_types( self )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        sql = """
                select
                    D_ID,
                    P_ID,
                    P_TYPE_CD,
                    D_PATH,
                    D_COMMENT
                from
                    DOCUMENT
                where
                    P_ID = ?
        """
        return sql, [ values["part_id"] ]
    # }}} get_search_sql( self, values )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        return (
            (
                "DOCUMENT",
                (
                    ( "D_ID",           "int", "rowid", "id" ),
                    ( "P_ID",           "int", "field", "part_id" ),
                    ( "P_TYPE_CD",      "int", "field", "type_cd" ),
                    ( "D_PATH",         "str", "field", "path" ),
                    ( "D_COMMENT",      "str", "field", "comment" ),
                ),
            ),
        )
    # }}} get_db_field_types( self )
# }}} class PartDocumentationTab

# {{{ class PartCategory
class PartCategory( Thing ):

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ get_value_by_index( self, index, codes )
    def get_value_by_index( self, index, codes ):
        if index == 0:
            return codes.get_text( self.category_cd )
        return ( "no", "yes" )[ self.primary ]
    # }}} get_value_by_index( self, index, codes )
# }}} class PartCategory

# {{{ class PartCategoryTab
class PartCategoryTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Categories", False, False, notebook )
        self.application = application
        self.object_constructor = PartCategory
        self.add_search_fields = False
        self.initialize()
    # }}} __init__( self, application, notebook )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        return (
            ( 0, "Category", "text" ),
            ( 1, "Primary", "text" ),
        )
    # }}} get_search_table_columns( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "part_id", "Part ID", "id", None ),
            ( "category_cd", "Property", "code", ( 1, False, False ) ),
            ( "primary", "Primary", "bool", None ),
        )
    # }}} get_detail_field_types( self )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        sql = """
                select
                    P_ID,
                    PT_CODE_CD as PROPERTY_CD,
                    PT_PRIMARY
                from
                    PARTTAG
                where
                    P_ID = ? and
                    PT_GROUP_CD = 1
        """
        return sql, [ values["part_id"] ]
    # }}} get_search_sql( self, values )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        return (
            (
                "PARTTAG",
                (
                    ( "P_ID",           "int", "key",   "part_id" ),
                    ( "PT_CODE_CD",     "int", "upd",   "category_cd" ),
                    ( "PT_GROUP_CD",    "int", "const", "1" ),
                    ( "PT_PRIMARY",     "int", "const", "0" ),
                    ( "PT_TEXT",        "str", "const", "" ),
                    ( "PT_AMOUNT",      "int", "const", "0" ),
                ),
            ),
        )
    # }}} get_db_field_types( self )
# }}} class PartCategoryTab

# {{{ class PartSupplier
class PartSupplier( Thing ):

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ get_value_by_index( self, index, codes )
    def get_value_by_index( self, index, codes ):
        if index == 0:
            return codes.get_text( self.category_cd )
        return ( "no", "yes" )[ self.primary ]
    # }}} get_value_by_index( self, index, codes )
# }}} class PartSupplier

# {{{ class PartSupplierTab
class PartSupplierTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Suppliers", False, False, notebook )
        self.application = application
        self.object_constructor = PartSupplier
        self.add_search_fields = False
        self.initialize()
    # }}} __init__( self, application, notebook )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        return (
            ( 0, "Supplier", "text" ),
            ( 1, "Primary", "text" ),
        )
    # }}} get_search_table_columns( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "part_id", "Part ID", "id", None ),
            ( "category_cd", "Property", "code", ( 6, False, False ) ),
            ( "primary", "Primary", "bool", None ),
        )
    # }}} get_detail_field_types( self )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        sql = """
                select
                    P_ID,
                    PT_CODE_CD as PROPERTY_CD,
                    PT_PRIMARY
                from
                    PARTTAG
                where
                    P_ID = ? and
                    PT_GROUP_CD = 1
        """
        return sql, [ values["part_id"] ]
    # }}} get_search_sql( self, values )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        return (
            (
                "PARTTAG",
                (
                    ( "P_ID",           "int", "key",   "part_id" ),
                    ( "PT_CODE_CD",     "int", "upd",   "category_cd" ),
                    ( "PT_GROUP_CD",    "int", "const", "1" ),
                    ( "PT_PRIMARY",     "int", "const", "0" ),
                    ( "PT_TEXT",        "str", "const", "" ),
                    ( "PT_AMOUNT",      "int", "const", "0" ),
                ),
            ),
        )
    # }}} get_db_field_types( self )
# }}} class PartSupplierTab

# {{{ class PartProject
class PartProject( Thing ):

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ get_value_by_index( self, index, codes )
    def get_value_by_index( self, index, codes ):
        if index == 0:
            return codes.get_text( self.category_cd )
        return ( "no", "yes" )[ self.primary ]
    # }}} get_value_by_index( self, index, codes )
# }}} class PartProject

# {{{ class PartProjectTab
class PartProjectTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Projects", False, False, notebook )
        self.application = application
        self.object_constructor = PartProject
        self.add_search_fields = False
        self.initialize()
    # }}} __init__( self, application, notebook )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        return (
            ( 0, "Project", "text" ),
            ( 1, "Primary", "text" ),
        )
    # }}} get_search_table_columns( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "part_id", "Part ID", "id", None ),
            ( "category_cd", "Property", "code", ( 6, False, False ) ),
            ( "primary", "Primary", "bool", None ),
        )
    # }}} get_detail_field_types( self )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        sql = """
                select
                    P_ID,
                    PT_CODE_CD as PROPERTY_CD,
                    PT_PRIMARY
                from
                    PARTTAG
                where
                    P_ID = ? and
                    PT_GROUP_CD = 1
        """
        return sql, [ values["part_id"] ]
    # }}} get_search_sql( self, values )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        return (
            (
                "PARTTAG",
                (
                    ( "P_ID",           "int", "key",   "part_id" ),
                    ( "PT_CODE_CD",     "int", "upd",   "category_cd" ),
                    ( "PT_GROUP_CD",    "int", "const", "1" ),
                    ( "PT_PRIMARY",     "int", "const", "0" ),
                    ( "PT_TEXT",        "str", "const", "" ),
                    ( "PT_AMOUNT",      "int", "const", "0" ),
                ),
            ),
        )
    # }}} get_db_field_types( self )
# }}} class PartProjectTab

# {{{ class PartOrder
class PartOrder( Thing ):

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ get_value_by_index( self, index, codes )
    def get_value_by_index( self, index, codes ):
        if index == 0:
            return codes.get_text( self.category_cd )
        return ( "no", "yes" )[ self.primary ]
    # }}} get_value_by_index( self, index, codes )
# }}} class PartOrder

# {{{ class PartOrderTab
class PartOrderTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Orders", False, False, notebook )
        self.application = application
        self.object_constructor = PartOrder
        self.add_search_fields = False
        self.initialize()
    # }}} __init__( self, application, notebook )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        return (
            ( 0, "Order", "text" ),
            ( 1, "Primary", "text" ),
        )
    # }}} get_search_table_columns( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "part_id", "Part ID", "id", None ),
            ( "category_cd", "Property", "code", ( 6, False, False ) ),
            ( "primary", "Primary", "bool", None ),
        )
    # }}} get_detail_field_types( self )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        sql = """
                select
                    P_ID,
                    PT_CODE_CD as PROPERTY_CD,
                    PT_PRIMARY
                from
                    PARTTAG
                where
                    P_ID = ? and
                    PT_GROUP_CD = 1
        """
        return sql, [ values["part_id"] ]
    # }}} get_search_sql( self, values )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        return (
            (
                "PARTTAG",
                (
                    ( "P_ID",           "int", "key",   "part_id" ),
                    ( "PT_CODE_CD",     "int", "upd",   "category_cd" ),
                    ( "PT_GROUP_CD",    "int", "const", "1" ),
                    ( "PT_PRIMARY",     "int", "const", "0" ),
                    ( "PT_TEXT",        "str", "const", "" ),
                    ( "PT_AMOUNT",      "int", "const", "0" ),
                ),
            ),
        )
    # }}} get_db_field_types( self )
# }}} class PartOrderTab

# {{{ class Supplier
class Supplier( Thing ):

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ get_label_text( self )
    def get_label_text( self ):
        return self.dispname
    # }}} get_label_text( self )

    # {{{ get_value_by_index( self, column, codes )
    def get_value_by_index( self, column, codes ):
        return self.dispname
    # }}} get_value_by_index( self, column, codes )
# }}} class Supplier

# {{{ class SupplierTab
class SupplierTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Suppliers", True, True, notebook )
        self.application = application
        self.object_constructor = Supplier
        self.initialize()
        self.append_page( Gtk.Label( label='not implemented yet' ), 'Parts' )
        self.append_page( Gtk.Label( label='not implemented yet' ), 'Orders' )
    # }}} __init__( self, application, notebook )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        return (
            ( 1, "Name", "text" ),
        )
    # }}} get_search_table_columns( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "id", "ID", "id", None ),
            ( "dispname", "Display Name", "text", None ),
            ( "name", "Name", "text", None ),
            ( "addr1", "Address Line 1", "text", None ),
            ( "addr2", "Address Line 2", "text", None ),
            ( "zip", "ZIP", "text", None ),
            ( "city", "City", "text", None ),
            ( "country", "Country", "code", ( 5, False, False ) ),
            ( "homepage", "Homepage", "text", None ),
        )
    # }}} get_detail_field_types( self )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        sql = """
                select
                    S_ID,
                    S_DISPLAY_NAME,
                    S_NAME,
                    S_ADDRESS_LINE_1,
                    S_ADDRESS_LINE_2,
                    S_ZIP,
                    S_CITY,
                    S_COUNTRY_CD,
                    S_HOMEPAGE
                from
                    SUPPLIER
        """
        pre = " where"
        args = []
        text =values["text"]
        if text != None:
            sql += pre
            sql += "( S_DISPLAY_NAME like ? or S_ADDRESS_LINE_1 like ? )"
            like = "%%%s%%" % text
            args.append( like )
            args.append( like )
            pre = " and"
        return sql, args
    # }}} get_search_sql( self, values )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        return (
            (
                "SUPPLIER",
                (
                    ( "S_ID",               "int", "rowid", "id" ),
                    ( "S_DISPLAY_NAME",     "str", "uniq",  "dispname" ),
                    ( "S_NAME",             "str", "field", "name" ),
                    ( "S_ADDRESS_LINE_1",   "str", "field", "addr1" ),
                    ( "S_ADDRESS_LINE_2",   "str", "field", "addr2" ),
                    ( "S_ZIP",              "str", "field", "zip" ),
                    ( "S_CITY",             "str", "field", "city" ),
                    ( "S_COUNTRY_CD",       "int", "field", "country" ),
                    ( "S_HOMEPAGE",         "str", "field", "homepage" ),
                )
            ),
        )
    # }}} get_db_field_types( self )
# }}} class SupplierTab

# {{{ class Project
class Project( Thing ):

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ get_label_text( self )
    def get_label_text( self ):
        return self.name
    # }}} get_label_text( self )

    # {{{ get_value_by_index( self, column, codes )
    def get_value_by_index( self, column, codes ):
        return self.name
    # }}} get_value_by_index( self, column, codes )
# }}} class Project

# {{{ class ProjectTab
class ProjectTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Projects", True, True, notebook )
        self.application = application
        self.object_constructor = Project
        self.initialize()
        self.append_page( Gtk.Label( label='not implemented yet' ), 'Parts' )
    # }}} __init__( self, application, notebook )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        return (
            ( 1, "Name", "text" ),
        )
    # }}} get_search_table_columns( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "id", "ID", "id", None ),
            ( "name", "Name", "text", None ),
            ( "descr", "Description", "textarea", 3 ),
            ( "homepage", "Homepage", "text", None ),
        )
    # }}} get_detail_field_types( self )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        sql = """
                select
                    PR_ID,
                    PR_NAME,
                    PR_DESCRIPTION,
                    PR_HOMEPAGE
                from
                    PROJECT
        """
        pre = " where"
        args = []
        text =values["text"]
        if text != None:
            sql += "%s PR_NAME like ?" % pre
            args.append( "%%%s%%" % text )
            pre = " and"
        return sql, args
    # }}} get_search_sql( self, values )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        return (
            (
                "PROJECT",
                (
                    ( "PR_ID",              "int", "rowid", "id" ),
                    ( "PR_NAME",            "str", "uniq",  "name" ),
                    ( "PR_DESCRIPTION",     "str", "field", "descr" ),
                    ( "PR_HOMEPAGE",        "str", "field", "homepage" ),
                )
            ),
        )
    # }}} get_db_field_types( self )
# }}} class ProjectTab

# {{{ class Order
class Order( Thing ):

    # {{{ __init__( self )
    def __init__( self ):
        pass
    # }}} __init__( self )

    # {{{ get_label_text( self )
    def get_label_text( self ):
        # supplier - date - state
        return self.name
    # }}} get_label_text( self )

    # {{{ get_value_by_index( self, column, codes )
    def get_value_by_index( self, column, codes ):
        return "123" # +++++
    # }}} get_value_by_index( self, column, codes )
# }}} class Order

# {{{ class OrderTab
class OrderTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Orders", True, True, notebook )
        self.application = application
        self.object_constructor = Order
        self.initialize()
        self.append_page( Gtk.Label( label='not implemented yet' ), 'Parts' )
    # }}} __init__( self, application, notebook )

    # {{{ get_search_table_columns( self )
    def get_search_table_columns( self ):
        return (
            ( 1, "ID", "integer" ),
        )
    # }}} get_search_table_columns( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "id", "ID", "id", None ),
            ( "supplier_id", "Supplier", "integer", None ),
            ( "state_cd", "State", "code", ( 3, False, False ) ),
            ( "shipping", "Shipping Service", "text", None ),
            ( "tracking", "Tracking Number", "text", None ),
        )
    # }}} get_detail_field_types( self )

    # {{{ get_search_sql( self, values )
    def get_search_sql( self, values ):
        sql = """
                select
                    O_ID,
                    S_ID,
                    O_STATE_CD,
                    O_SHIPPING_COMPANY,
                    O_TRACKING_NUMBER
                from
                    ORDERS
        """
        pre = " where"
        text =values["text"]
        if text != None:
            text = self.application.database.escape( text )
            sql += "%s O_NAME like '%%%s%%' " % text
            pre = " and"
        return sql, None
    # }}} get_search_sql( self, values )

    # {{{ get_db_field_types( self )
    def get_db_field_types( self ):
        return (
            (
                "ORDERS",
                (
                    ( "O_ID",               "int", "rowid", "id" ),
                    ( "S_ID",               "str", "uniq",  "supplier_id" ),
                    ( "O_STATE_CD",         "str", "field", "state_cd" ),
                    ( "O_SHIPPING_COMPANY", "str", "field", "shipping" ),
                    ( "O_TRACKING_NUMBER",  "str", "field", "tracking" ),
                )
            ),
        )
    # }}} get_db_field_types( self )
# }}} class OrderTab

# {{{ class CodeTab
class CodeTab( ThingTab ):

    # {{{ __init__( self, application, notebook )
    def __init__( self, application, notebook ):
        ThingTab.__init__( self, application, "Codes", False, False, notebook )
        self.application = application
        self.initialize()
    # }}} __init__( self, application, notebook )

    # {{{ create_search_page( self )
    def create_search_page( self ):
        self.code_model = FilteredCodeTreeModel( self.application.codes,
                0, True, True )
        self.code_treeview = Gtk.TreeView( model=self.code_model )
        self.code_treeview.connect( "row-activated", self.row_activated )
        renderer = Gtk.CellRendererText()
        column = Gtk.TreeViewColumn( "Code", renderer, text=3 )
        self.code_treeview.append_column( column )
        scroll = Gtk.ScrolledWindow()
        scroll.set_hexpand( True )
        scroll.add( self.code_treeview )
        return scroll
    # }}} create_search_page( self )

    # {{{ get_detail_field_types( self )
    def get_detail_field_types( self ):
        return (
            ( "code", "Code", "id", None ),
            ( "parent", "Parent", "code", ( 0, False, True ) ),
            ( "text", "Name", "text", None ),
            ( "unused", "Unused", "bool", None ),
            ( "deleted", "Deleted", "bool", None ),
        )
    # }}} get_detail_field_types( self )

    # {{{ row_activated( self, tree, path, column )
    def row_activated( self, tree, path, column ):
        code = self.code_model.get_code_by_path( path )
        self.show_object_detail( code )
    # }}} row_activated( self, tree, path, column )

    # {{{ detail_new( self, button )
    def detail_new( self, button ):
        values = self.get_detail_values()
        self.application.codes.insert( values )
    # }}} detail_new( self, button )

    # {{{ detail_save( self, button )
    def detail_save( self, button ):
        values = self.get_detail_values()
        self.application.codes.update( values )
    # }}} detail_save( self, button )
# }}} class CodeTab

# {{{ class Window
class Window( Gtk.Window ):

    # {{{ __init__( self, application )
    def __init__( self, application ):
        Gtk.Window.__init__( self, title="Labor Lager V0.1" )
        self.application = application
        self.set_size_request( 800, 600 )
        self.connect( "delete-event", self.delete_event )

        self.notebook = Gtk.Notebook()
        PartTab( application, self.notebook )
        SupplierTab( application, self.notebook )
        ProjectTab( application, self.notebook )
        OrderTab( application, self.notebook )
        CodeTab( application, self.notebook )
        self.add( self.notebook )

        self.show_all()
    # }}} __init__( self, application )


    # {{{ delete_event( self, x, y )
    def delete_event( self, x, y ):
        if self.allow_close():
            Gtk.main_quit()
            return False
        return True
    # }}} delete_event( self, x, y )

    # {{{ allow_close( self )
    def allow_close( self ):
        if self.application.modified:
            self.application.modified = False
            return False
        return True
    # }}} allow_close( self )

    # {{{ on_button_clicked( self, widget )
    def on_button_clicked( self, widget ):
        print( "Hello World" )
    # }}} on_button_clicked( self, widget )
# }}} class Window

# {{{ class Application
class Application():

    # {{{ __init__( self )
    def __init__( self ):
        self.database = Database( "lablag" )
        self.codes = Codes( self.database )
        self.window = Window( self )
        self.modified = False
        self.last_chosen_codes = {}
    # }}} __init__( self )

    # {{{ choose_code( self, group, show_deleted, prev_value )
    def choose_code( self, group, show_deleted, prev_value ):
        chooser = CodeChooser( self.window, self.codes,
                        group, show_deleted,
                        prev_value, self.last_chosen_codes )
        return chooser.value
    # }}} choose_code( self, group, show_deleted, prev_value )

    # {{{ run( self )
    def run( self ):
        Gtk.main()
    # }}} run( self )

    # {{{ cleanup( self )
    def cleanup( self ):
        self.database.close()
    # }}} cleanup( self )
# }}} class Application

# {{{ 'main'
if __name__ == "__main__":
    app = Application()
    try:
        app.run()
    except:
        pass
    app.cleanup()
# }}} 'main'

