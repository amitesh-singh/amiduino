//This is LOLLIN board V3 i have, which has on board led at D4 (which is mapped to GPIO2)
#define LOLLIN_LED D4
//In my esp12e board on Perfboard has LED on Digital port 2 
#define MYESPBOARD_LED 2

void setup()
{
  pinMode(MYESPBOARD_LED, OUTPUT);
  Serial.begin(115200);

  digitalWrite(MYESPBOARD_LED, 0); //led is on
  pinMode(D0, INPUT);
}

void loop()
{
   uint8_t buttonTouched = digitalRead(D0);

   if (buttonTouched == 1)
     {
        Serial.println("touched");
     }
    else
    {
      Serial.println("Not touched");
    }
   delay(100);
}
