//#include <IRremote.h>
#include "samsungtvremote.h"
//IRsend irsend;

//#define SMART_HUB 0xE0E09E61
//#define ENTER     0xE0E016E9
//#define UP        0xE0E006F9
//#define DOWN      0xE0E08679
//#define LEFT      0xE0E0A659
//#define RIGHT 0xE0E046B9
//#define POWER  0xE0E040BF
//#define SOURCE 0xE0E0807F
//#define HDMI   0xE0E0D12E
//#define BUTTON1  0xE0E020DF
//#define BUTTON2  0xE0E0A05F
//#define BUTTON3  0xE0E0609F
//#define BUTTON4 0xE0E010EF
//#define BUTTON5 0xE0E0906F
//#define BUTTON6 0xE0E050AF
//#define BUTTON7 0xE0E030CF
//#define BUTTON8 0xE0E0B04F
//#define BUTTON9 0xE0E0708F
//#define BUTTON0 0xE0E08877
//#define PRECHANNEL 0xE0E0C837
//#define VOLUP     0xE0E0E01F
//#define VOLDOWN  0xE0E0D02F
//#define MUTE    0xE0E0F00F
//#define CHLIST  0xE0E0D629
//#define CHUP  0xE0E048B7
//#define CHDOWN  0xE0E008F7
//#define MENU 0xE0E058A7
//#define TOOLS 0xE0E0D22D
//#define EXIT 0xE0E0B44B
//#define BACKWARD 0xE0E0A25D
//#define PAUSE   0xE0E052AD
//#define FORWARD 0xE0E012ED
//#define RECORD 0xE0E0926D
//#define PLAY 0xE0E0E21D
//#define STOP 0xE0E0629D

samsungtvremote _smarttvremote;
//  An IR LED must be connected to Arduino PWM pin 3.
void setup()
{
   Serial.begin(9600);
   Serial.println("init..");
}

void loop()
{
   //tested and it seems to work well. ;)
   // send 3 times.
   for (uint8_t i = 0; i < 3; ++i)
     {
	   _smarttvremote.powerButton();
        //irsend.sendSAMSUNG(POWER, 32); //this is on/off test
        //delay(5); //delay for 5 ms
     }
   Serial.println("sent ir to switch off tv");
   delay(10000);
}
