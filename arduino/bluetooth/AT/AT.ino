#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {

     Serial.begin(9600);
     Serial.setTimeout(6000);
     delay(200);

     pinMode(2,OUTPUT); digitalWrite(3,HIGH);

     Serial.println("Enter AT commands:");

     mySerial.begin(9600);
     mySerial.setTimeout(100);
     delay(250);

}

void loop()
{

   if (mySerial.available())
     Serial.write(mySerial.read());

   if (Serial.available())
     mySerial.write(Serial.read());
}

