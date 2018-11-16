#include <WiFi.h>
#include <WebSocketsServer.h>
 
WebSocketsServer webSocket(80);
 
const char* ssid = "POOJA";
const char* password =  "poojasingh";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
    switch(type)
    {
        case WStype_CONNECTED:
        {
            Serial.println("Connected:");
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            webSocket.sendTXT(num, "Connected");
        }
        break;

        case WStype_DISCONNECTED:
        {
            Serial.println("Disconnected.");
        }
        break;

        case WStype_ERROR:
        {
            Serial.println("Error");
        }
        break;
        case WStype_TEXT:
        {
            Serial.printf("[%u] get Text: %s\r\n", num, payload);
            webSocket.sendTXT(num, "holla!");
        }

        default:
        break;
    }
}

void setup()
{
  Serial.begin(9600);
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
 
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  delay(100);
}
 
void loop()
{
    webSocket.loop();
}