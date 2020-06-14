//use nodemcu 1.0 (ESP12E board)
// PWM dac code sample code

#define DAC D1
#define LED D4
ADC_MODE(ADC_VCC);

static int dutycycle = 0;
String line;
static uint32_t vcc;

void setup()
{
   pinMode(LED, OUTPUT);
   digitalWrite(LED, 0); // LED is ON

   Serial.begin(115200);
   //50% duty cycle
   pinMode(DAC, OUTPUT);
   Serial.println("Starting....");
   //default frequency is 1Khz
   /*
    * PWM frequency is 1kHz by default. Call analogWriteFreq(new_frequency) to change the frequency. Valid values are from 100Hz up to 40000Hz.
    */
   analogWriteFreq(20000);

   analogWrite(DAC, dutycycle);
}

void loop()
{
   while (Serial.available())
     {
        line = Serial.readString();
        dutycycle = atoi(line.c_str());
        Serial.println(dutycycle);

        analogWrite(DAC, dutycycle);

        vcc = ESP.getVcc();
        Serial.print("System voltage(mV): ");
        Serial.println(vcc);
        Serial.println("=====================\n\n"); 
     }
}
