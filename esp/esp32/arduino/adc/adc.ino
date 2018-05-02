//Analog Input
#define ANALOG_PIN_0 0
int analog_value = 0;

void setup()
{
  Serial.begin(9600);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Analog IN Test");
}

void loop()
{
  analog_value = analogRead(ANALOG_PIN_0);
  Serial.println(analog_value);
  delay(500);
}