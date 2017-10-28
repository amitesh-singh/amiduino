//This is LOLLIN board V3 i have, which has on board led at D4 (which is mapped to GPIO2)
#define LOLLIN_LED D4
//In my esp12e board on Perfboard has LED on Digital port 2 
#define MYESPBOARD_LED 2

void setup()
{
  pinMode(MYESPBOARD_LED, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  digitalWrite(MYESPBOARD_LED, 1);
  delay(1000);
  Serial.println("Blinks");
  digitalWrite(MYESPBOARD_LED, 0);
  delay(1000);
}
