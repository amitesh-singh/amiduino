#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> //for itoa

#define MrLCDsCrib PORTB
#define DataDir_MrLCDsCrib DDRB
#define MrLCDsControl PORTD
#define DataDir_MrLCDsControl DDRD
#define LightSwitch 3 // E
#define ReadWrite 4  // RW
#define BiPolarMood 2 //RS

void Check_IF_MrLCD_isBusy(void);
void Peek_A_Boo(void);
void Send_A_Command(unsigned char command);
void Send_A_Character(unsigned char character);
void Send_A_String(char *string);
//                                    Row: 1  2    3   4
char colPosCommandAddress[2] = {0, 64}; //  0, 64, 20, 84
void gotoxyPos(uint8_t x, uint8_t y)
{
   if (x < 0)
     x = 0;
   else if (x > 16)
     x = 16;
   if (y < 0) y = 0;
   else if (y > 1) y = 1;

   //0x80 -> 0b10000000
   Send_A_Command(0x80 + colPosCommandAddress[y] + x);
}

void Send_A_String(char *str)
{
   char *tmp = str;
   while (*tmp != '\0')
     {
        Send_A_Character(*tmp++);
     }
}

int main(void)
{
   DataDir_MrLCDsControl |= 1<<LightSwitch | 1<<ReadWrite | 1<<BiPolarMood;
   _delay_ms(15);

   Send_A_Command(0x01); //Clear Screen 0x01 = 00000001
   _delay_ms(2);
   Send_A_Command(0x38);
   _delay_us(50);
   Send_A_Command(0b00001110);
   _delay_us(50);
   uint8_t character[8] =
     {
        0b10000,
        0b11110,
        0b01010,
        0b01110,
        0b11111,
        0b01000,
        0b01110,
        0b01001
     };


   Send_A_String("Hello Ami!");
   _delay_ms(10000);

   Send_A_Command(0x01); //clear the screen

   // for 5x8 font.
   uint8_t character2[8] =
     {
        10,14,17,17,17,17,14
     };
   uint8_t i = 0;

   //---------- POPULATE 1st character ------
   _delay_ms(2);
   Send_A_Command(0x40); // Address of CGRAM
   _delay_ms(2);
   i = 0;
   while (i != 7)
     {
        Send_A_Character(character[i]);
        _delay_ms(5);
        i++;
     }
   // ----- 1st character is populated

   // --------- POPULATE 2nd character -----
   _delay_ms(2);
   Send_A_Command(0x48);
   _delay_ms(2);
   i = 0;
   while (i != 7)
     {
        Send_A_Character(character2[i]);
        _delay_ms(2);
        i++;
     }

   gotoxyPos(0, 0);
   _delay_ms(1);
   Send_A_Character(0x00); // this actually draws the 0th character to LCD screen (DDRAM)

   gotoxyPos(1, 1);
   //Draw the same character to (1, 1) stored at 0x40 or the first character (ASCI value: 0)
   // CGRAM 1th character is already populated we could just reuse then. the data will be 
   // copied from 0x40 -- 0x47 of CGRAM to DDRAM and hence displays the character. 
   Send_A_Character(0x00);


   gotoxyPos(2, 2);
   _delay_ms(1);
   Send_A_Character(0x01); //we are using 2nd character
   while(1)
     {
     }
}

void Check_IF_MrLCD_isBusy()
{
   DataDir_MrLCDsCrib = 0;
   MrLCDsControl |= 1<<ReadWrite;
   MrLCDsControl &= ~1<<BiPolarMood;

   while (MrLCDsCrib >= 0x80)
     {
        Peek_A_Boo();
     }
   DataDir_MrLCDsCrib = 0xFF; //0xFF means 0b11111111
   _delay_ms(1); //this wait was necessary to make it work - Ami
}

void Peek_A_Boo()
{
   MrLCDsControl |= 1<<LightSwitch;
   asm volatile ("nop");
   asm volatile ("nop");
   asm volatile ("nop");
   asm volatile ("nop");
   MrLCDsControl &= ~1<<LightSwitch;
}

void Send_A_Command(unsigned char command)
{
   Check_IF_MrLCD_isBusy();
   MrLCDsCrib = command;
   MrLCDsControl &= ~ ((1<<ReadWrite)|(1<<BiPolarMood));
   Peek_A_Boo();
   MrLCDsCrib = 0;
}

void Send_A_Character(unsigned char character)
{
   Check_IF_MrLCD_isBusy();
   MrLCDsCrib = character;
   MrLCDsControl &= ~ (1<<ReadWrite);
   MrLCDsControl |= 1<<BiPolarMood;
   Peek_A_Boo();
   MrLCDsCrib = 0;
}
