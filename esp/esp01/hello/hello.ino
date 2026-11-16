// On esp01, the led (blue) is connected to tx pin; serial won't work if it's used as
// gpio 

void setup()
{
   Serial.begin(115200);
   Serial.println("led setup is done");
}

void loop()
{
   delay(1000);                     
   Serial.println("Blink");
}
