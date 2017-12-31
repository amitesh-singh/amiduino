project('avrbuild', 'c')

strip = find_program('avr-strip')
avrdude = find_program('avrdude')
size = find_program('avr-size')
objcopy = find_program('avr-objcopy')

mcu = 'attiny13'
avrdudemcu = 't13'
#cpuspeed = '9600000'
cpuspeed = '4800000'
#cpuspeed = '1200000'

cargs =        ['-Os',
                '-g',
                '-std=c99',
                '-Wall',
#'-funsigned-char',
#               '-funsigned-bitfields',
#               '-fpack-struct',
#               '-fshort-enums',
              '-ffunction-sections',
              '-fdata-sections',
                '-DF_CPU=' + cpuspeed,
                '-DUART_RX_ENABLED',
                '-DUART_TX_ENABLED',
                '-DUART_BAUDRATE=38400',
                '-mmcu=' + mcu,
                ]
linkargs =      ['-Wl,-Map,blink.map',
                 '-Wl,--gc-sections',
                 '-Wl,--relax',
                 '-ffunction-sections',
                '-fdata-sections',
                 '-mmcu=attiny13a'
                ]

srcfiles = ['blink.c', 'uart.c']

exe = executable('blink', srcfiles,
           c_args: cargs,
           link_args: linkargs,
           build_by_default: true
           )

#stripbuild = run_target('strip', command: [strip, (exe.full_path())], depends: exe)
run_target('eep', command: [objcopy, ['-O', 'ihex', '-j', '.eeprom', '--set-section-flags=.eeprom=alloc,load', '--no-change-warnings', 
                                                  '--change-section-lma', '.eeprom=0', exe.full_path(), exe.full_path() + '.eep']], depends: exe)
run_target('hex', command: [objcopy, ['-O', 'ihex', '-R', '.eeprom', exe.full_path(), exe.full_path() + '.hex']], depends: exe)
run_target('size', command: [size, exe.full_path(), '--mcu=' + mcu, '-C', '--format=avr'], depends: exe)

run_target('probe', command: [avrdude,'-c', 'usbasp', '-p', avrdudemcu], depends: exe)
run_target('upload', command: [avrdude,'-c', 'usbasp', '-p', avrdudemcu, '-U', 'flash:w:' + exe.full_path() + '.hex'], depends: exe)
