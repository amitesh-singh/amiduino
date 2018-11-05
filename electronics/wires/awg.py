#!/usr/bin/python3

import math
import sys

pi4 = math.pi / 4

class AWG:
        def __init__( self, awg=None, dmm=None, sqmm=None ):
                if awg != None:
                        dmm = 0.127 * 92 ** ((36-awg)/39)
                        sqmm = dmm * dmm * pi4
                elif dmm != None:
                        awg = 36 - 39 * math.log( dmm/0.127, 92 )
                        sqmm = dmm * dmm * pi4
                elif sqmm != None:
                        dmm = (sqmm / pi4) ** 0.5
                        awg = 36 - 39 * math.log( dmm/0.127, 92 )
                else:
                        raise Exception( "No value specified." )
                self.awg = awg
                self.dmm = dmm
                self.sqmm = sqmm


def doit():
        for s in sys.argv[1:]:
                if s.endswith( "awg" ):
                        awg = AWG( awg=float( s[:-3] ) )
                elif s.endswith( "d" ):
                        awg = AWG( dmm=float( s[:-1] ) )
                elif s.endswith( "dmm" ):
                        awg = AWG( dmm=float( s[:-3] ) )
                elif s.endswith( "sqmm" ):
                        awg = AWG( sqmm=float( s[:-4] ) )
                else:
                        awg = AWG( awg=float( s ) )
                print( "awg %d = dia %.2fmm = area %.03fsqmm" % ( awg.awg, awg.dmm, awg.sqmm ) )

if __name__ == "__main__":
        doit()
