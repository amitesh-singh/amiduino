#include <Arduino.h>
#include <WiFi.h>
#include <IPv6Address.h>
esp_netif_t* get_esp_interface_netif(esp_interface_t interface);

IPv6Address globalIPv6()
{
   esp_ip6_addr_t addr;

   if(esp_netif_get_ip6_global(get_esp_interface_netif(ESP_IF_WIFI_STA), &addr))
     {
        return IPv6Address();
     }
   return IPv6Address(addr.addr);
}

void setup()
{
   Serial.begin(115200);
   Serial.println("setting up wifi");
   WiFi.begin("Aviaarav-2.4G", "poojasingh");

   WiFi.enableIpV6();
   while (WiFi.status() != WL_CONNECTED)
     {
        delay(100);
        Serial.print(".");
     }
   WiFi.enableIpV6();

   Serial.print("Local ip: ");
   Serial.println(WiFi.localIP());
   Serial.print("link ipv6: ");
   Serial.println(WiFi.localIPv6());


}

void loop()
{
   delay(1000);
   Serial.println("ping...");
   Serial.print("link ipv6: ");
   Serial.println(WiFi.localIPv6());
   Serial.print("gloabl ipv6: ");
   Serial.println(globalIPv6().toString());
}
