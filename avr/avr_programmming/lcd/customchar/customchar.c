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
/*
CG ROM : this the memory which holds the permanent fonts you call to be displayed . this holds the pattern
for every single character of  predefined lcd font. and you call the content of this memory by the placeing
  corresponding ascii value on the lcd port . like for retrieval of   'A' you have to send the ascii value
	 of 'A' which is 0x41 to the lcd.   CGROM can also be seen as computer hard drive from where
 you load your required program into ram to start working. but it is not modify able because it's rom.

CG RAM : this memory works same as CG ROM but as this is ram we can modify it's content any time .
so this is the place where whe have to first store our custom character pattern. then that pattern
 can be sent to display.
the HD44780 has total 8 CG RAM memory location . so we can generate only up to 8 custom characters .
 but you can always change the content of CG RAM on the fly to generate new characters .

the addresses  of 8 CG RAM location  goes from  0x00 to 0x07.

or example if you want to write you first custom character to 0x00 CG RAM location then you have to send lcd a command
lcdCmd(0x40);
after this now you are ready to send data to CG RAM location 0x00.
after sending the whole pattern for location 0x00 the location pointer will auto increase to 0x01 location

or if you want to store something to 0x03 location then send direct command lcdCmd(0x43);

 now we know how to goto the CG RAM but now problem is

HOW TO DECIDE AND WRITE CUSTOM FONT TO CG RAM  ?

a standard 5x8 dots font is being shown in the image as you can see there are total 8 bytes of data on the whole to decide pattern of the font.

as yo can see all you need to do is decide the first row pattern like in the example image of a battery symbol, you can make any symbol you can imagine like degree celsius , Sine wave symbol Square wave symbol, compass, arrow etc.
in first row the first pixel is off  so lsb bit 0 is 0 and pixel 1 is on so bit1 is 1 and pixel 2 is on so bit 2 is 1
this method apply on each row of the font ( 8 rows ). which gives us total 8 bytes of data first msb 3 bits are always 0.

you can also use this web tool to decide this pattern  and code.


Writing to CG RAM
Writing to CG RAM is a lot like moving the cursor to a particular position on the display and displaying
characters at that new location. The steps involved are:
Set RS (Register Select) and R/W (Read/Write) pins of the LCD to initialize the LCD to accept instructions
Set the CG RAM address by sending an instruction byte from 64 to 127 (locations 0-63 in CG RAM).
Switch to Data Mode by changing the setting of RS pin
Send bytes with the bit patterns for your symbol(s). The LCD controller automatically increments CG RAM
 addresses, in the same way as it increments cursor positions on the display.
To leave CG RAM, switch to Command Mode to set address counter to a valid display address (e.g. 128,
 1st character of 1st line); the clear-screen instruction (byte 1); or the home instruction (byte 2).
 Now bytes are once again being written to the visible portion of the display.
To display the defined custom character print ASCII codes 0 through 7.

DD RAM : DDRAM is the memory which holds only those characters which are currently on the screen .
 means if there is a message is currently being displayed on the screen then it has to be  on the DDRAM
for example if you want to display "hello" on the screen then you have load pattern of h from the CG ROM
 TO DD RAM then do the same for 'e' ,'l' ,'l' and 'o'.

the address of cg ram is totally depends on the size of the lcd like for

16 x 2 LCD            Row1      0x80 0x81 0x82 0x83 0x84 0x85 0x86 through 0x8F
                              Row2     0xCO 0xC1 0xC2 0xC3 0xC4 0xC5 0xC6 through 0xCF

20 x 1 LCD            Row1     0x80 0x81 0x82 0x83 through 0x93

20 x 2 LCD            Row1     0x80 0x81 0x82 0x83 through 0x93
      Row2     0xCO 0xC1 0xC2 0xC3 through 0xD3

20 x 4 LCD            Row1     0x80 0x81 0x82 0x83 through 0x93
      Row2     0xCO 0xC1 0xC2 0xC3 through 0xD3
      Row3     0x94 0x95 0x96 0x97 through 0xA7
      Row4     0xD4 0xD5 0xD6 0xD7 through 0xE7

40 x 2 L CD           Row1     0x80 0x81 0x82 0x83 through 0xA7
      Row2     0xCO 0xC1 0xC2 0xC3 through 0xE7
*/
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

// for 5x8 font.
	uint8_t character2[8] =
	{
	10,14,17,17,17,17,14
	};
	uint8_t i = 0;

	gotoxyPos(1, 0);
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

	//goto prev. x,y position
	gotoxyPos(1, 0);
	_delay_ms(1);
	Send_A_Character(0x00); // this actually draws the stuffs

	gotoxyPos(1, 1);
	_delay_ms(2);
	Send_A_Command(72); // Address of CGRAM
	_delay_ms(2);
	i = 0;
	while (i != 7)
	{
		Send_A_Character(character2[i]);
		_delay_ms(5);
		i++;
	}

	//goto prev. x,y position
	gotoxyPos(1, 1);
	_delay_ms(1);
	Send_A_Character(0x01); // this actually draws the stuffs

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
