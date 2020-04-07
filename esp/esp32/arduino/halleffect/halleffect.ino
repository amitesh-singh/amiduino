static int val = 0;
static const int ledPin = 5;

void setup()
{
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    digitalWrite(ledPin, 1);
    val = hallRead();
    Serial.println(val);
    delay(1000);
    digitalWrite(ledPin, 0);
}