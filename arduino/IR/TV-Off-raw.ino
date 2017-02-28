//http://www.alanzucconi.com/2015/08/19/how-to-hack-any-ir-remote-controller/
#include <IRremote.h>

// IR emitter LED is connected at Pin 3 of Arduino Uno
IRsend irsend;

unsigned int code [] = { 1800, 400, 200, 400, 250, 350, 250 };
void setup()
{
   Serial.begin(9600);
}


void loop()
{
   int khz = 38; // 38 khz
   //Samsung Smart TV Off signal
   unsigned int irSignal[] = {4600,4350,700,1550,650,
        1550,650,1600,650,450,650,450,650,450,650,450,700,400,700,1550,650,1550,650,1600,650,450,650,450,
        650,450,700,450,650,450,650,450,650,1550,700,450,650,450,650,450,650,450,650,450,700,400,650,1600,650,
        450,650,1550,650,1600,650,1550,650,1550,700,1550,650,1550,650};

   irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);

   Serial.println("Sent TV Power Off signal");

   delay(5000 * 12); //In this example, the signal will be repeated every 1 min, approximately.
}

