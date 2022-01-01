#include <ESP8266WiFi.h>
#include <espnow.h>

//set esp-now channel based on router wifi

static uint8_t ledPin = BUILTIN_LED;
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

const char *ssid = "Aviaarav-2.4G";

int32_t getWiFiChannel(const char *ssid)
{
   if (int32_t n = WiFi.scanNetworks())
     {
        for (uint8_t i=0; i<n; i++)
          {
             Serial.println(WiFi.SSID(i));
             if (!strcmp(ssid, WiFi.SSID(i).c_str()))
               {
                  return WiFi.channel(i);
               }
          }
     }

  return 0;
}

void setup()
{
    pinMode(ledPin, OUTPUT);

    Serial.begin(115200);
    Serial.println("Starting Wifi..");

    /*
    WiFi.mode(WIFI_STA);
    WiFi.begin("Aviaarav-2.4G", "poojasingh");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected..");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.print("WiFi Channel: ");
    Serial.println(WiFi.channel());
    */

   //To use both wifi and esp-now, they should be on same channel
   // earlier i'd manually set the wifi channel fixed in wifi router

    int channel = getWiFiChannel(ssid);
    WiFi.mode(WIFI_STA);

    WiFi.printDiag(Serial);
    //set the channel to wifi one
    wifi_promiscuous_enable(1);
    wifi_set_channel(channel);
    wifi_promiscuous_enable(0);

    WiFi.printDiag(Serial);

    //init esp now
    if (esp_now_init() != 0)
    {
      Serial.println("Failed to initialize esp-now");
      return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb([](uint8_t *macaddr, uint8_t status)
    {
      if (status == 0)
      {
        Serial.println("Sent - successful");
      }
      else
      {
        Serial.println("Sent - unsuccessful");
      }
    });

    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, channel, nullptr, 0);
}

void loop()
{
    digitalWrite(ledPin, 1);
    delay(1000);
    digitalWrite(ledPin, 0);
    delay(1000);
    uint8_t d = 1;
    esp_now_send(broadcastAddress, (&d), 4);
}

