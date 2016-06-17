#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

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

// 16x2 position tracking
// Refer: https://newbiehack.com/MicrocontrollersMakingtheLCDShowNumbers.aspx
//          Command values
//    1st row 0 ........ 15
//    2nd row 64.........79
//    gotoLocation command initial value 0b10000000 == 0x80;
// Say you want to move to home i.e. 0,0 => Send_A_Command(0x80 + 0)
// move at 2nd item on 2nd row : Send_A_Command(0x80 + 65).
// This is how you role ;)
//

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

	Send_A_Command(0x01); // Clear the screen
	_delay_ms(2);
	while(1)
	{

    // Roam at first row with "X"
		for (uint8_t i = 0; i < 16; ++i)
		{
			Send_A_Command(0x80 + i);
			Send_A_String("X");
			_delay_ms((90));

			Send_A_Command(0x80 + i);
			Send_A_String(" "); //erase X from its old position
		}

    //Roam at 2nd Row with "Y"
		for (uint8_t i = 64; i <= 79; ++i)
		{
			Send_A_Command(0x80 + i);
			Send_A_String("Y");
			_delay_ms((150));

			Send_A_Command(0x80 + i);
			Send_A_String(" "); //erase X from its old position
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
