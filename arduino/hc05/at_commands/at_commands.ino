#include <SoftwareSerial.h>

//RxD (arduino) - Tx (HC05)
//TxD (arduino) - Rx (HC05) - Apply voltage divider (5v * 2/3 = 3.3v)
#define RxD 3
#define TxD 2

SoftwareSerial bt(RxD, TxD);

void setup()
{
   //bt.begin(9600); //bluetooth AT serial init
   Serial.begin(9600);
   Serial.println("Starting...");
   pinMode(13, OUTPUT);
   digitalWrite(13, HIGH);
  delay(1000);
 //  while (!bt.available());

 bt.begin(38400); //at command mode for HC05
 
}

byte c;
void loop()
{
 
  if (Serial.available())
  {
    bt.write(Serial.read());
    
  }
  if (bt.available())
  {
    Serial.write(bt.read());
  }
}
