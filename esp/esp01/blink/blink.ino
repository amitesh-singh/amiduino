
// esp01 has led at Tx end, led = 1
// esp01s the one which has pullup at CH_PD has LED at GPIO2
//uint8_t led = 1;
uint8_t led = 2;

void setup()
{
   //Serial.begin(115200);
   pinMode(led, OUTPUT);   
}

void loop()
{
   digitalWrite(led, LOW);   
   delay(1000);                     
   digitalWrite(led, HIGH);  
   delay(2000);
}
