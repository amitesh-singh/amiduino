#include <WiFi.h>

static const char *ssid = "gabrugabnru";
static const char *passwd = "passwordisgood";

void setup()
{
    Serial.begin(9600);
    Serial.println("Ok setting it up Soft AP");
    WiFi.softAP(ssid, passwd);
    Serial.println();
    Serial.print("IP addr:");
    Serial.println(WiFi.softAPIP());
}

void loop()
{
}
