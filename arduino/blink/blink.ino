//This is to test the new low voltage drive mosfets i bought from aliexpress.
// IRLZ44N - Pins order: GDS
// IRL3705N - pins order: GDS

void setup()
{
   pinMode(3, OUTPUT);
   digitalWrite(3, HIGH);
   pinMode(13, OUTPUT);
}

void loop()
{
   digitalWrite(13, HIGH);
   delay(1000);
   digitalWrite(13, LOW);
   delay(1000);
}
