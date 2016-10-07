### Notes

1. include \<avr/eeprom.h\>
2. In Makefile, add `%(TARGET).eeprom` at all:
3. Use `EEMEM` to define EEMEM variables.
4. Use avrdude -c usbasp -p m16 -U eeprom:w:eep.eeprom to write into eeprom.
   Put some extra care to make sure you initialize eeprom section via avrdude
   or else the eeprom variables won't initialize well.  
   The full command to flash both `flash` and `eeprom`.  
   `avrdude -c usbasp -p m16 -U eeprom:w:eep.eeprom -U flash:w:eep.hex`  
5. eeprom functions

   * eeprom_read_byte() - to read 1 byte  

     `uint8_t EEMEM initial_val = 1;       
      uint8_t ram_initial_val = eeprom_read_byte(&initial_val);`  

   * eeprom_read_word() - to read 2 bytes  
      ` uint16_t val = 1024;    
        uint16_t adc_val = eeprom_read_word(&val); `  

   * eeprom_read_dword() - to read 4 bytes  
   * eeprom_read_block() - to read a big chunk  
        
      ` unsigned char EEMEM eeprom_str[] = "Hello Ami";`  
      `char buf[10];`     
      ` eeprom_read_block(buf, eeprom_str, 10);`    
