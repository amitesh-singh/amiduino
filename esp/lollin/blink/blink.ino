//This is LOLLIN board V3 i have, which has on board led at D4 (which is mapped to GPIO2)
#define LOLLIN_LED D4

void setup()
{
  pinMode(LOLLIN_LED, OUTPUT);
  digitalWrite(LOLLIN_LED, 0);

}

void loop()
{
  digitalWrite(LOLLIN_LED, 1);
  delay(1000);
  digitalWrite(LOLLIN_LED, 0);
  delay(1000);
}
