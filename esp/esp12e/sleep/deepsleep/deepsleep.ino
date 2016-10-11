#include <Ticker.h>

Ticker blinker;
static uint8_t ledStatus = 0;
uint32_t counter = 0;

static void blink()
{
   digitalWrite(13, ledStatus);
   ledStatus = (ledStatus + 1) % 2;
   if (counter++ > 20)
     {
        Serial.println("Going to sleep...for 10 seconds..");
        ESP.deepSleep(10000000, WAKE_RF_DEFAULT); //sleep for 10 seconds
        //After this, the device will reboot,
        // make sure GPIO_16 is connected to RST then only it will work.
     }
}
void setup()
{
   Serial.begin(115200);
   pinMode(13, OUTPUT);
   blinker.attach(0.25, blink);
   Serial.println("setup....");
}

void loop()
{
}
