//esp32 dac 
// GPIO25 - channel 1
// GPIO26 - channel 2

#define DAC1 25
static int value = 128;
//255 - 3.3v
//128 - 1.65v

void setup()
{
  Serial.begin(9600);
  //set 1.65V
  dacWrite(DAC1, value);

}

void loop()
{
}
