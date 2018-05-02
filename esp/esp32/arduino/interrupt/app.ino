
const int interruptPin = 25; //GPIO25
volatile int interruptCounter = 0;
int numberOfInterrupts = 0;

//IRAM_ATTR tells the complier, that this code Must always be in the 
// ESP32's IRAM, the limited 128k IRAM.  use it sparingly.
void IRAM_ATTR handleInterrupt()
{
    interruptCounter++;
}

// Every function called from the ISR also needs to be 
// declared as IRAM_ATTR yourFunction() {}.
// Also don't call analogRead() in ISR either.

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
