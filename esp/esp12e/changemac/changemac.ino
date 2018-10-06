#include <ESP8266WiFi.h>

void changeMac()
{
   static uint8_t mac[] = {0x77, 0x01, 0x02, 0x03, 0x04, 0x05};
   wifi_set_macaddr(STATION_IF, &mac[0]);
}

void setup()
{
    Serial.begin(9600);
    delay(1000);
    for (int i = 0; i < 100; ++i)
    {
        Serial.print(".");
        delay(100);
    }
    Serial.println("");
    Serial.print("mac: "); Serial.println(WiFi.macAddress());

    changeMac();

    Serial.print("new mac: "); Serial.println(WiFi.macAddress());
}

void loop()
{
}
