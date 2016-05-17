// INT0 & INT1 example

const byte BUTTON = 2;
const byte BUTTON2 = 3;

const byte LED = 13;
const byte LED2 = 12;

void wakeup()
{
  Serial.println("Interrupt is called");
  if (digitalRead(BUTTON) == HIGH)
  {
    Serial.println("LED digital pin 13 is set low");
    digitalWrite(LED, LOW);
  }
  else
  {
    Serial.println("LED digital pin 13 is set high");
    digitalWrite(LED, HIGH);
  }
}
void wakeup2()
{
  Serial.println("Interrupt is called");
  if (digitalRead(BUTTON2) == HIGH)
  {
    Serial.println("LED digital pin 12 is set low");
    digitalWrite(LED2, LOW);
  }
  else
  {
    Serial.println("LED digital pin 12 is set high");
    digitalWrite(LED2, HIGH);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(LED2, OUTPUT);

 // pinMode(BUTTON, INPUT); not needed since already input is the default state of pins
  digitalWrite(BUTTON, OUTPUT); // enable internal pullups
  digitalWrite(BUTTON2, OUTPUT);

  attachInterrupt(0, wakeup, CHANGE);
  attachInterrupt(1, wakeup2, CHANGE); // INT1 on pd3 aka digital pin 3.
}

void loop()
{
}
