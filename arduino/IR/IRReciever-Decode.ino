#include <IRremote.h>

const byte IR_RECV_PIN = 2;

IRrecv irrecv(IR_RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // start the receiver
}

void loop()
{
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }

  delay(100);
}
