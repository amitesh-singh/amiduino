#include <avr/io.h>
#include <avr/pgmspace.h> // this is required

//this stores the mystr in FLASH
const char mystr[] PROGMEM = "Hello Cruel World!";
//pgm_read_word
//pgm_read_dword
int main()
{
   LCDstring(PSTR("Hello world!")); // this for one time usage only.

   //if you are going to use this several times, then better to use this code
   for (int i = 0; pgm_read_byte(&mystr[i]); ++i)
     {
        LCDstring(pgm_read_byte(&mystr[i])); //terminates when '\0' null byte comes
     }

   while (1)
     {
     }
   return 0;
}
