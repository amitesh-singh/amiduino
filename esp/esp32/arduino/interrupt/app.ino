
const int interruptPin = 25; //GPIO25
volatile int interruptCounter = 0;
int numberOfInterrupts = 0;

void IRAM_ATTR handleInterrupt()
{
    interruptCounter++;
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Testing interrupts...");

    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
}

void loop()
{
    if (interruptCounter > 0)
    {
        interruptCounter--;

        numberOfInterrupts++;
        Serial.println("Interrupt has occured: Total: ");
        Serial.println(numberOfInterrupts);
    }
}
