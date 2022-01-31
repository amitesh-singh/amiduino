
#include <ESP8266WiFi.h>

#ifdef LWIP_IPV6
#include <lwip/dns.h>
#include <AddrList.h>

#endif

const char ssid[] = "Aviaarav-2.4G";
const char passwd[] = "poojasingh";

void setup()
{
   Serial.begin(115200);
   Serial.println("Setting up ipv6");

   WiFi.begin(ssid, passwd);
#if LWIP_IPV6

   // Manually set DNS-Server ipv6 address
   ip_addr_t ip6_dns;

   // GOOGLE EXTERNAL - 2001:4860:4860::8888
   IP_ADDR6(&ip6_dns,PP_HTONL(0x20014860UL), PP_HTONL(0x48600000UL), PP_HTONL(0x00000000UL), PP_HTONL(0x00008888UL));

   dns_setserver(0, &ip6_dns);
   delay(100);

   // wait for global ipv6 address
   for (bool configured = false; !configured;) {
        for (auto addr : addrList)

          if ( !addr.isLocal() && addr.isV6() ) {
               configured = true;
               break;
          }
        Serial.print('.');
        delay(500);
   }
#else
   // wait for ipv4 address
   if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println(F("Connection Failed!"));
   }
#endif

#if LWIP_IPV6
   // Manually set DNS-Server ipv6 address
//   ip_addr_t ip6_dns;

   // GOOGLE EXTERNAL - 2001:4860:4860::8888
   IP_ADDR6(&ip6_dns,PP_HTONL(0x20014860UL), PP_HTONL(0x48600000UL), PP_HTONL(0x00000000UL), PP_HTONL(0x00008888UL));

   dns_setserver(0, &ip6_dns);
#endif
   Serial.print("Connected, IP: ");
   Serial.println(WiFi.localIP());
   Serial.print("ipv 6");

    for (auto a : addrList) {
    Serial.printf("IF='%s' IPv6=%d local=%d hostname='%s' addr= %s\r\n",
               a.ifname().c_str(),
               a.isV6(),
               a.isLocal(),
               a.ifhostname(),
               a.toString().c_str());

    if (a.isLegacy()) {
      Serial.printf(" / mask:%s / gw:%s\r\n",
                 a.netmask().toString().c_str(),
                 a.gw().toString().c_str());
    }}

   //Serial.println(WiFi.localIPv6());
}

void loop()
{
}
