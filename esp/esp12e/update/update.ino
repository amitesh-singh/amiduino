
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "FS.h"

//TODO for updating the code via ESP-now

void setup() {
  Serial.begin(115200);
  Serial.println();
  SPIFFS.begin();
  Dir dir = SPIFFS.openDir("/");

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  File file = SPIFFS.open("/blinkESP.bin", "r");


  uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
  if (!Update.begin(maxSketchSpace, U_FLASH)) { //start with max available size
    Update.printError(Serial);
    Serial.println("ERROR");
  }
 
  while (file.available()) {
    uint8_t ibuffer[128];
    file.read((uint8_t *)ibuffer, 128);
    Serial.println((char *)ibuffer);
    Update.write(ibuffer, sizeof(ibuffer));
 
  }

 
  Serial.print(Update.end(true));
  digitalWrite(BUILTIN_LED, HIGH);
  file.close();
}

void loop() {
 
}


