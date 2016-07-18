how to debug keyevents on host
------------------------------
- install evtest
- sudo evtest and choose the device
 e.g.
  sudo evtest
  No device specified, trying to scan all of /dev/input/event*
  Available devices:
  /dev/input/event0: Power Button
  /dev/input/event1: Power Button
  /dev/input/event2: USB USB Keykoard
  /dev/input/event3: USB USB Keykoard
  /dev/input/event4: Logitech USB Receiver
  /dev/input/event5: Logitech USB Receiver
  /dev/input/event6: HDA Intel PCH Front Mic
  /dev/input/event7: HDA Intel PCH Rear Mic
  /dev/input/event8: HDA Intel PCH Line
  /dev/input/event9: HDA Intel PCH Line Out Front
  /dev/input/event10:   HDA Intel PCH Line Out Surround
  /dev/input/event11:   HDA Intel PCH Line Out CLFE
  /dev/input/event12:   HDA Intel PCH Front Headphone
  /dev/input/event13:   HDA Intel PCH HDMI/DP,pcm=3
  /dev/input/event14:   ami keyboard
  Select the device event number [0-14]:

  Input driver version is 1.0.1
  Input device ID: bus 0x3 vendor 0x4242 product 0xe131 version 0x101
  Input device name: "ami keyboard"
  Supported events:
    Event type 0 (EV_SYN)
     Event type 1 (EV_KEY)
       Event code 1 (KEY_ESC)
       Event code 2 (KEY_1)
       Event code 3 (KEY_2)
       Event code 4 (KEY_3)
       Event code 5 (KEY_4)
       Event code 6 (KEY_5)
       Event code 7 (KEY_6)
       Event code 8 (KEY_7)
       Event code 9 (KEY_8)
       Event code 10 (KEY_9)
       Event code 11 (KEY_0)
       Event code 12 (KEY_MINUS)
       Event code 13 (KEY_EQUAL)
       Event code 14 (KEY_BACKSPACE)
       Event code 15 (KEY_TAB)
       Event code 16 (KEY_Q)
       Event code 17 (KEY_W)
       Event code 18 (KEY_E)
       Event code 19 (KEY_R)
       Event code 20 (KEY_T)
       Event code 21 (KEY_Y)
       Event code 22 (KEY_U)
       Event code 23 (KEY_I)
       Event code 24 (KEY_O)
       Event code 25 (KEY_P)
       Event code 26 (KEY_LEFTBRACE)
       Event code 27 (KEY_RIGHTBRACE)
       Event code 28 (KEY_ENTER)
       Event code 29 (KEY_LEFTCTRL)
       Event code 30 (KEY_A)
       Event code 31 (KEY_S)
       Event code 32 (KEY_D)
       Event code 33 (KEY_F)
       Event code 34 (KEY_G)
       Event code 35 (KEY_H)
       Event code 36 (KEY_J)
       Event code 37 (KEY_K)
       Event code 38 (KEY_L)
       Event code 39 (KEY_SEMICOLON)
       Event code 40 (KEY_APOSTROPHE)
       Event code 41 (KEY_GRAVE)
       Event code 42 (KEY_LEFTSHIFT)
       Event code 43 (KEY_BACKSLASH)
       Event code 44 (KEY_Z)
       Event code 45 (KEY_X)
       Event code 46 (KEY_C)
       Event code 47 (KEY_V)
       Event code 48 (KEY_B)
       Event code 49 (KEY_N)
       Event code 50 (KEY_M)
       Event code 51 (KEY_COMMA)
       Event code 52 (KEY_DOT)
       Event code 53 (KEY_SLASH)
       Event code 54 (KEY_RIGHTSHIFT)
       Event code 55 (KEY_KPASTERISK)
       Event code 56 (KEY_LEFTALT)
       Event code 57 (KEY_SPACE)
       Event code 58 (KEY_CAPSLOCK)
       Event code 59 (KEY_F1)
       Event code 60 (KEY_F2)
       Event code 61 (KEY_F3)
       Event code 62 (KEY_F4)
       Event code 63 (KEY_F5)
       Event code 64 (KEY_F6)
       Event code 65 (KEY_F7)
       Event code 66 (KEY_F8)
       Event code 67 (KEY_F9)
       Event code 68 (KEY_F10)
       Event code 69 (KEY_NUMLOCK)
       Event code 70 (KEY_SCROLLLOCK)
       Event code 71 (KEY_KP7)
       Event code 72 (KEY_KP8)
       Event code 73 (KEY_KP9)
       Event code 74 (KEY_KPMINUS)
       Event code 75 (KEY_KP4)
       Event code 76 (KEY_KP5)
       Event code 77 (KEY_KP6)
       Event code 78 (KEY_KPPLUS)
       Event code 79 (KEY_KP1)
       Event code 80 (KEY_KP2)
       Event code 81 (KEY_KP3)
       Event code 82 (KEY_KP0)
       Event code 83 (KEY_KPDOT)
       Event code 86 (KEY_102ND)
       Event code 87 (KEY_F11)
       Event code 88 (KEY_F12)
       Event code 96 (KEY_KPENTER)
       Event code 97 (KEY_RIGHTCTRL)
       Event code 98 (KEY_KPSLASH)
       Event code 99 (KEY_SYSRQ)
       Event code 100 (KEY_RIGHTALT)
       Event code 102 (KEY_HOME)
       Event code 103 (KEY_UP)
       Event code 104 (KEY_PAGEUP)
       Event code 105 (KEY_LEFT)
       Event code 106 (KEY_RIGHT)
       Event code 107 (KEY_END)
       Event code 108 (KEY_DOWN)
       Event code 109 (KEY_PAGEDOWN)
       Event code 110 (KEY_INSERT)
       Event code 111 (KEY_DELETE)
       Event code 119 (KEY_PAUSE)
       Event code 125 (KEY_LEFTMETA)
       Event code 126 (KEY_RIGHTMETA)
       Event code 127 (KEY_COMPOSE)
     Event type 4 (EV_MSC)
       Event code 4 (MSC_SCAN)
     Event type 17 (EV_LED)
       Event code 0 (LED_NUML) state 1
           Event code 1 (LED_CAPSL) state 0
               Event code 2 (LED_SCROLLL) state 0
                   Event code 3 (LED_COMPOSE) state 0
                       Event code 4 (LED_KANA) state 0
                       Key repeat handling:
                         Repeat type 20 (EV_REP)
       Repeat code 0 (REP_DELAY)
         Value    250
             Repeat code 1 (REP_PERIOD)
         Value     33
         Properties:
         Testing ... (interrupt to exit)

keyboard notes picked from VUSBmorse
------------------------------------

/* Reportbuffer format:

	0  Modifier byte
	1  reserved
	2  keycode array (0)
	3  keycode array (1)
	4  keycode array (2)
	5  keycode array (3)
	6  keycode array (4)
	7  keycode array (5)
	
	<< This is the standard usb-keyboard reportbuffer. It allows for 6 simultaneous keypresses to be detected (excl. modifier keys). In this application we only use 1, so the last 5 bytes in this buffer will always remain 0. >>
	<< I decided not to optimize this in order to make it easy to add extra keys that can be pressed simultaneously>>
	
   Modifier byte: 8 bits, each individual bit represents one of the modifier keys.

   	bit0  LEFT CTRL		(1<<0)
	bit1  LEFT SHIFT	(1<<1)
	bit2  LEFT ALT		(1<<2)
	bit3  LEFT GUI		(1<<3)
	bit4  RIGHT CTRL	(1<<4)
	bit5  RIGHT SHIFT	(1<<5)
	bit6  RIGHT ALT		(1<<6)
	bit7  RIGHT GUI		(1<<7)

	an example of a reportBuffer for a CTRL+ALT+Delete keypress:

	{((1<<0)+(1<<2)),0,76,0,0,0,0,0}

	the first byte holds both the LEFT CTRL and LEFT  modifier keys the 3rd byte holds the delete key (== decimal 76)

*/


usbasp bootloader write flash debug message with avrdude
----------------------------------------------------------

avrdude: reading input file "main.hex"
avrdude: input file main.hex auto detected as Intel Hex
avrdude: writing flash (1970 bytes):

Writing |                                                    | 0% 0.00savrdude: usbasp_program_paged_write("f
lash", 0x0, 128)
Writing | ###                                                | 6% 0.01savrdude: usbasp_program_paged_write("f
lash", 0x80, 128)
Writing | ######                                             | 12% 0.02savrdude: usbasp_program_paged_write("
flash", 0x100, 128)
Writing | #########                                          | 18% 0.04savrdude: usbasp_program_paged_write("
flash", 0x180, 128)
Writing | #############                                      | 25% 0.05savrdude: usbasp_program_paged_write("
flash", 0x200, 128)
Writing | ################                                   | 31% 0.06savrdude: usbasp_program_paged_write("
flash", 0x280, 128)
Writing | ###################                                | 37% 0.07savrdude: usbasp_program_paged_write("
flash", 0x300, 128)
Writing | ######################                             | 43% 0.09savrdude: usbasp_program_paged_write("
flash", 0x380, 128)
Writing | #########################                          | 50% 0.10savrdude: usbasp_program_paged_write("
flash", 0x400, 128)
Writing | ############################                       | 56% 0.11savrdude: usbasp_program_paged_write("
flash", 0x480, 128)
Writing | ###############################                    | 62% 0.12savrdude: usbasp_program_paged_write("
flash", 0x500, 128)
Writing | ##################################                 | 68% 0.14savrdude: usbasp_program_paged_write("
flash", 0x580, 128)
Writing | ######################################             | 75% 0.15savrdude: usbasp_program_paged_write("
flash", 0x600, 128)
Writing | #########################################          | 81% 0.16savrdude: usbasp_program_paged_write("
flash", 0x680, 128)
Writing | ############################################       | 87% 0.17savrdude: usbasp_program_paged_write("
flash", 0x700, 128)
Writing | ###############################################    | 93% 0.19savrdude: usbasp_program_paged_write("
flash", 0x780, 128)
Writing | ################################################## | 100% 0.20s

avrdude: 1970 bytes of flash written

