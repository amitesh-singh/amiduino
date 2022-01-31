#pragma once

#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"

class udplogger
{
   bool isConnected = false;
   IPAddress ip_;
   uint16_t port_;
   AsyncUDP udp_;
  public:
   udplogger()
     {
     }

   udplogger(IPAddress ip, uint16_t port): ip_(ip), port_(port)
     {
     }

   void setServer(IPAddress ip, uint16_t port)
     {
        ip_ = ip;
        port_ = port;
     }

   void connect()
     {
        isConnected = udp_.connect(ip_, port_);
        if (!isConnected)
          {
             Serial.println("Failed to connect");
          }
     }

      template<typename T>
      void print(T s)
        {
           if (isConnected)
             udp_.print(s);
        }

      template<typename T>
      void println(T s)
        {
           if (isConnected)
             udp_.print(s);
           udp_.print("\r\n");
        }
};
