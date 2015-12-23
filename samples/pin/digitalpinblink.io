const int LED = 13;
void setup()
{
  pinMode(LED, OUTPUT);

}

void loop()
{
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}
