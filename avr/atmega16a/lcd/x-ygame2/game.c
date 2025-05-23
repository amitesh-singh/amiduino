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

	Send_A_String("X-Y game ;)");

	_delay_ms(10000); // show it for 10 seconds

	Send_A_Command(0x01);
	_delay_ms(2);

	char pos[3];
	while(1)
	{
		for (uint8_t i = 0; i < 16; ++i)
		{
			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 0;
			Send_A_Command(0x80 + i);
			_delay_ms(1);
			Send_A_String("X");

			_delay_ms(500);
		// show the X location in 2nd row
			Send_A_Command(0x80 + 75);
			_delay_ms(1);

			Send_A_String("  ");
   		_delay_ms(1);

			itoa(i, pos, 10);
			Send_A_Command(0x80 + 75);
			_delay_ms(1);
			Send_A_String(pos);
			_delay_ms(1);
			Send_A_Command(0x80 + i);
			_delay_ms(1);
			Send_A_String(" "); //erase X from its old position
			_delay_ms(1);
		}

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
