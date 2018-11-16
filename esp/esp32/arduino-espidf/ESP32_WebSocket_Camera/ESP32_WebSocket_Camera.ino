#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "canvas_htm.h"
#include "OV7670.h"


const int SIOD = 21; //SDA
const int SIOC = 22; //SCL

const int VSYNC = 34;
const int HREF = 35;

const int XCLK = 32;
const int PCLK = 33;

const int D0 = 27;
const int D1 = 17;
const int D2 = 16;
const int D3 = 15;
const int D4 = 14;
const int D5 = 13;
const int D6 = 12;
const int D7 = 4;

const int TFT_DC = 2;
const int TFT_CS = 5;
//DIN <- MOSI 23
//CLK <- SCK 18

#define ssid      "POOJA"
#define password    "poojasingh"

OV7670 *camera = nullptr;

//WiFiMulti wifiMulti;
WiFiServer server(80);

unsigned char pix = 0;

//unsigned char bmpHeader[BMP::headerSize];

unsigned char start_flag = 0xAA;
unsigned char end_flag = 0xFF;
unsigned char ip_flag = 0x11;

WebSocketsServer webSocket(81);    // create a websocket server on port 81

void startWebSocket()
{ // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}

void startWebServer()
{
   server.begin();
   Serial.println("Http web server started.");
}

void serve()
{
  WiFiClient client = server.available();
  if (client) 
  {
    //Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        //Serial.write(c);
        if (c == '\n') 
        {
          if (currentLine.length() == 0) 
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print(canvas_htm);
            client.println();
            break;
          } 
          else 
          {
            currentLine = "";
          }
        } 
        else if (c != '\r') 
        {
          currentLine += c;
        }
        
      }
    }
    // close the connection:
    client.stop();

  }  
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t payloadlength) 
{ // When a WebSocket message is received
 
  int blk_count = 0;

  char canvas_VGA[] = "canvas-VGA";
  char canvas_Q_VGA[] = "canvas-Q-VGA";
  char canvas_QQ_VGA[] = "canvas-QQ-VGA";
  char canvas_QQQ_VGA[] = "canvas-QQQ-VGA";
  char ipaddr[26] ;
  IPAddress localip;
  
  switch (type)
    {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
           webSocket.sendBIN(0, &ip_flag, 1);
           localip = WiFi.localIP();
           sprintf(ipaddr, "%d.%d.%d.%d", localip[0], localip[1], localip[2], localip[3]);
           webSocket.sendTXT(0, (const char *)ipaddr);

      }
      break;
    case WStype_TEXT:                     // if new text data is received
      if (payloadlength == sizeof(canvas_QQQ_VGA)-1) {
        if (memcmp(canvas_QQQ_VGA, payload, payloadlength) == 0) {
              Serial.printf("canvas_QQQ_VGA");
              webSocket.sendBIN(0, &end_flag, 1);
              delete camera;
              camera = new OV7670(OV7670::Mode::QQQVGA_RGB565, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
        }
      } else if (payloadlength == sizeof(canvas_QQ_VGA)-1) {
        if (memcmp(canvas_QQ_VGA, payload, payloadlength) == 0) {
              Serial.printf("canvas_QQ_VGA");
              webSocket.sendBIN(0, &end_flag, 1);
              delete camera;
              camera = new OV7670(OV7670::Mode::QQVGA_RGB565, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
        }
      } else if (payloadlength == sizeof(canvas_Q_VGA)-1) {
        if (memcmp(canvas_Q_VGA, payload, payloadlength) == 0) {
              Serial.printf("canvas_Q_VGA");
              webSocket.sendBIN(0, &end_flag, 1);
              delete camera;
              camera = new OV7670(OV7670::Mode::QVGA_RGB565, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
        }
      } else if (payloadlength == sizeof(canvas_VGA)-1) {
        if (memcmp(canvas_VGA, payload, payloadlength) == 0) {
              Serial.printf("canvas_VGA");
              webSocket.sendBIN(0, &end_flag, 1);
              delete camera;
              camera = new OV7670(OV7670::Mode::VGA_RGB565, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
        }
      }

      blk_count = camera->yres/I2SCamera::blockSlice;//30, 60, 120
      for (int i=0; i<blk_count; i++)
        {

           if (i == 0)
             {
                camera->startBlock = 1;
                camera->endBlock = I2SCamera::blockSlice;
                webSocket.sendBIN(0, &start_flag, 1);
             }

           if (i == blk_count-1)
             {
                webSocket.sendBIN(0, &end_flag, 1);
             }

           camera->oneFrame();
           webSocket.sendBIN(0, camera->frame, camera->xres * I2SCamera::blockSlice * 2);
           camera->startBlock += I2SCamera::blockSlice;
           camera->endBlock   += I2SCamera::blockSlice;
        }

      break;
    case WStype_ERROR:                     // if new text data is received
      Serial.printf("Error \n");
    default:
      Serial.printf("WStype %x not handled \n", type);

    }
}

void initWifiStation()
{
   WiFi.mode(WIFI_AP_STA);
   WiFi.begin(ssid, password);    
   Serial.println("Connecting to WiFi");
   while (WiFi.status() != WL_CONNECTED)
     {
        delay(50);
        Serial.print(".");
     }
   Serial.println(String("Connected to the WiFi network (") + ssid + ")" );

   Serial.print("Station IP address: ");
   Serial.println(WiFi.localIP());
}

void setup()
{
   Serial.begin(9600);
   initWifiStation();
   startWebSocket();
   startWebServer();
}

void loop()
{
   webSocket.loop();
   serve();
}
