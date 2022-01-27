#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h>

#define ssid "Aviaarav-2.4G"
#define passwd  "poojasingh"

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\r\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);

  while (WiFi.status() != WL_CONNECTED)
    {
       delay(500);
       Serial.print(".");
    }

  Serial.println("WIFI connected");
  Serial.println(WiFi.localIP());
}

class auto_updater
{
   //only for http, for https use WiFiClientSecure 
   //WiFiClientSecure client;
   //client.setInsecure();
   WiFiClient client_;
   String server_, json_file_, firmware_, current_ver_;
   String version_;
   uint16_t port_;
   StaticJsonDocument<200> root;
   bool need_update_ = false;

  public:

   auto_updater(const String &server, uint16_t port, const String &json_file, const String &ver, uint8_t led_pin = LED_BUILTIN): server_(server),
    json_file_(json_file), current_ver_(ver), port_(port)
   {
      //TODO: disable it in production
      ESPhttpUpdate.setLedPin(led_pin, LOW);
      // Add optional callback notifiers
      ESPhttpUpdate.onStart([]()
                            {
                            Serial.println("CALLBACK:  HTTP update process started");
                            });
      ESPhttpUpdate.onEnd([]()
                          {
                          Serial.println("CALLBACK:  HTTP update process finished");
                          });
      ESPhttpUpdate.onProgress([](int cur, int total)
                               {
                               Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\r\n", cur, total);
                               });
      ESPhttpUpdate.onError([](int err)
                            {
                            Serial.printf("CALLBACK:  HTTP update fatal error code %d\r\n", err);
                            });
   }

   bool check_json()
     {
        WiFiClient client;
        bool ret = false;
        HTTPClient http_client;
        http_client.begin(client_, server_, port_, json_file_);
        //http_client.begin(client, "http://www.google.com");
        int http_code = http_client.GET();
        String payload;
        if (http_code > 0)
          {
             if (http_code == HTTP_CODE_OK)
               {
                  root.clear();
                  Serial.println("Got the json file");
                  payload = http_client.getString();
                  Serial.println(payload);
                  DeserializationError error = deserializeJson(root, payload);
                  if (error)
                    {
                       Serial.println("Deserialiazation failed");
                       return false;
                    }
                  if (root.containsKey("version"))
                    {
                       version_ = String( root["version"]);
                       Serial.print("version: ");
                       Serial.println(version_);
                       if (version_ > current_ver_)
                         {
                            Serial.println("need update set to true");
                            need_update_ = true;
                         }
                       else
                         {
                            Serial.println("need update set to false");
                            need_update_ = false;
                         }
                    }
                  if (root.containsKey("firmware"))
                    {
                       firmware_ = String(root["firmware"]);
                       Serial.print("firmware: ");
                       Serial.println(firmware_);
                    }
                  ret = true;
               }
          }
        else
          {
             Serial.printf("Error: %s\r\n", http_client.errorToString(http_code).c_str());
             ret = false;
          }
        http_client.end();

        return ret;
     }

   void update()
     {
        if (!check_json()) return;
        if (need_update_)
          {
             Serial.print("New firmware detected: version: ");
             Serial.print(version_);
             Serial.print(", firmware: ");
             Serial.println(firmware_);
             t_httpUpdate_return ret = ESPhttpUpdate.update(client_, server_, port_, firmware_);
             switch (ret)
               {
                 case HTTP_UPDATE_FAILED:
                     Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\r\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                     break;

                 case HTTP_UPDATE_NO_UPDATES:
                     Serial.println("HTTP_UPDATE_NO_UPDATES");
                     break;

                 case HTTP_UPDATE_OK:
                     Serial.println("HTTP_UPDATE_OK");
                     break;
               }
          }
        else
          {
             Serial.print("no need to update");
          }
     }
};

void check_update()
{
   auto_updater au("192.168.1.11", 8000, "/file.json", "0.1");
   au.update();
   delay(10000);
}
void loop()
{
   check_update();
}
