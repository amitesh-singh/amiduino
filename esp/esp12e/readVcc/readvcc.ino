// testing this one on Lollin board.
#define LOLLIN_LED 2
/*
ESP.getVcc():
This function is used to read NodeMCU module VCC voltage. ADC pin must be kept unconnected.

Note that ADC mode should be changed to read system voltage before reading VCC supply voltage.

To change ADC mode use ADC_MODE(mode) just after #include lines of your sketch.

Modes are ADC_TOUT (for external voltage), ADC_VCC (for system voltage). By default, it reads external voltage.

*/

/*
Readings are bit Off:
According to the datasheet of ESP8266, TOUT (ADC pin of ESP8266) pin must be dangled (floating) while
 measuring the power supply voltage through ADC. But on NodeMCU Dev Kit/Modules it is connected to voltage divider network of
  resistors (100Kohm & 220Kohm) for the sake of external voltage range (0-3.3V) support as shown in below figure.
  Refer: http://www.electronicwings.com/nodemcu/nodemcu-adc-with-arduino-ide

  So we have to remove the network, and then read the readings.
  

*/

// Define this at Top
ADC_MODE(ADC_VCC);

void setup()
{
  pinMode(LOLLIN_LED, OUTPUT);
  digitalWrite(LOLLIN_LED, 0);
  Serial.begin(9600);

  Serial.println("Read Vcc: ");
}

static uint32_t vcc;

void loop()
{
  digitalWrite(LOLLIN_LED, 1);
  delay(1000);
  digitalWrite(LOLLIN_LED, 0);
  delay(1000);
  //call this to print Vcc
  vcc = ESP.getVcc();
  Serial.print("System voltage(mV): ");
  Serial.println(vcc);
}
