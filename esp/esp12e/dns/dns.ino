#include "udplogger.h"
#include "Dns.h"
extern "C" {
#include "lwip/err.h"
#include "lwip/dns.h"
#include "lwip/dhcp.h"
}


// the IP address for the shield:
IPAddress dns(8, 8, 8, 8);  //Google dns  
IPAddress dns_internal(192, 168, 1, 254);

char ssid[] = "Aviaarav-2.4G";    // your network SSID (name)
char pass[] = "poojasingh"; // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
udplogger logger;
void setup()
{  
  // Initialize serial and wait for port to open:
  Serial.begin(115200);

  delay(5000);
  WiFi.mode(WIFI_STA);
  /*
but we need to set static ip..
  if (!WiFi.config(IPAddress(192,168,1,17), IPAddress(192,168,1,254),
              IPAddress(255, 255, 255, 0), dns_internal, dns))
    {
       Serial.println("Failed to configure");
    }
    */
  // attempt to connect to Wifi network:
  Serial.println("Trying to connect");

  WiFi.begin(ssid, pass);
   while(WiFi.status() != WL_CONNECTED)
      {
         delay(500);
         Serial.print(".");
      }

  Serial.println("Connected: ");
  Serial.println(WiFi.localIP());
  //this does not work if we set before WiFi.begin()
  //set internal dhcp first
  dns_setserver(0, dns_internal);
  //set google's dns server
  dns_setserver(1, dns);

  IPAddress log_server_ip;

  if (WiFi.hostByName("google.com", log_server_ip) == 1)
    {
       Serial.print("IpAddr");
       Serial.println(log_server_ip);
    }
  else
    {
       Serial.println("Failed to resolve DNS");
    }
  if (WiFi.hostByName("aarav.home", log_server_ip) == 1)
    {
       Serial.print("IpAddr:");
       Serial.println(log_server_ip);
    }
  else
    {
       Serial.println("Failed to resolve DNS");
    }

  logger.setServer(log_server_ip, 16003);
  logger.connect();
  Serial.print("Dns configured.");
  logger.print("Connected to aarav.home");
}

void loop () {
  logger.print("hey aarav.home");
  Serial.println("hey aarav.home");
  delay(10000);
}
 
