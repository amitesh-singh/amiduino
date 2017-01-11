#include <SoftwareSerial.h>

//RxD (arduino) - Tx (HC05)
//TxD (arduino) - Rx (HC05) - Apply voltage divider (5v * 2/3 = 3.3v)
#define RxD 3
#define TxD 2

SoftwareSerial bt(RxD, TxD);

void setup()
{
   bt.begin(9600); //bluetooth AT serial init
   Serial.begin(9600);
   Serial.println("Starting...");
   pinMode(13, OUTPUT);
   digitalWrite(13, LOW);
}

byte c;
void loop()
{
   while (!bt.available());

   c = bt.read();

   if (c == 'a')
     {
        Serial.println("Pin 13 high");
        digitalWrite(13, HIGH);
     }
   else if (c == 'b')
     {
        Serial.println("Pin 13 Low");
        digitalWrite(13, LOW);
     }
}
