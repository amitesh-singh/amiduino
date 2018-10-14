//This is LOLLIN board V3 i have, which has on board led at D4 (which is mapped to GPIO2)
#define LOLLIN_LED BUILTIN_LED
//In my esp12e board on Perfboard has LED on Digital port 2 
#define MYESPBOARD_LED 2

void setup()
{
  pinMode(LOLLIN_LED, OUTPUT);
  Serial.begin(9600);

  pinMode(5, OUTPUT);
  digitalWrite(5, 1);
}

void loop()
{
  digitalWrite(LOLLIN_LED, 1);
  delay(1000);
  Serial.println("Blinks");
  digitalWrite(LOLLIN_LED, 0);
  delay(1000);
}
