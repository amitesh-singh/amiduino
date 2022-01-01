 #include <ESP8266WiFi.h>

  static uint8_t ledPin = BUILTIN_LED;

void setup()
{
    pinMode(ledPin, OUTPUT);

    Serial.begin(115200);
    Serial.println("Starting Wifi..");

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
}

void loop()
{
    digitalWrite(ledPin, 1);
    delay(1000);
    digitalWrite(ledPin, 0);
    delay(1000);
}

