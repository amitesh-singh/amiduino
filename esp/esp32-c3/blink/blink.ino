#define LED 8

void setup()
{
    Serial.begin(9600);
    delay(500) ;
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

    Serial.println("Starting blinky");
}


void loop()
{
    Serial.println("Starting blinky");
    digitalWrite(LED, LOW);
    delay(1000);
    digitalWrite(LED, HIGH);
    delay(1000);
}
