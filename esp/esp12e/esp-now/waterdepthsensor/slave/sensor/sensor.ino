
#include "espnowhelper.h"

#define DEBUG
#define SENSOR_ID 1

// On Lollin board
#define TRIGGER 5  // (D1)
#define ECHO  4  // D2
//http://www.sintexplastics.com/wp-content/uploads/2017/04/ProductCatalogue2017.pdf
// 100l - dia (inches): 43.3, height (inches): 48.2, dia menhole: 15.7 inches
// 48.2 inches = 122.4 cm
const static long fullDistance = 123; // in cm

struct __attribute__((__packed__)) waterinfo
{
    uint8_t sensorid;
    long distance;
    uint8_t percentage;
};

static waterinfo wi;

//Code forCSR04 sonar sensor
// need to power HCSR04 from 5v or else it won't work, i was getting 0cm reading earlier
// connect 1k/2k resistor divider between ECHO 
void getDistance()
{
    long duration;

    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIGGER, LOW);

    duration = pulseIn(ECHO, HIGH);

    wi.distance = (duration/2)/29.1;
    wi.percentage = (wi.distance*100)/fullDistance;
}

#define WIFI_CHANNEL 1
static espnow esp12e;
static uint8_t remoteMac[] = {0x18, 0xFE, 0x34, 0xE1, 0xAC, 0x6A};

static bool retry = false;
static uint8_t retransmit = 0;

void setup()
{

    #ifdef DEBUG
    Serial.begin(9600);
    #endif
    
    wi.sensorid = SENSOR_ID;

    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO, INPUT);

#ifdef DEBUG
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);
#endif

    //init esp-now
    if (esp12e.init(WIFI_STA, ESP_NOW_ROLE_COMBO))
    {
        //failed to initialize espinit
        Serial.println("Failed to initialize esp-now");
        ESP.restart();
        delay(1000);
    }

    esp12e.addPeer(remoteMac, ESP_NOW_ROLE_COMBO, WIFI_CHANNEL, nullptr, 0);

    esp12e.addSendCb([](uint8_t *macaddr, uint8_t status)
    {
    #ifdef DEBUG
        Serial.println("send_cb");
    #endif
    }
    );

    retry = false;
    retransmit = 0;
    esp12e.addRecvCb([](u8 *mac_addr, u8 *data, u8 len)
    {
    #ifdef DEBUG
        Serial.println("recv cb");
    #endif

        retry = true;
    }
    );

    Serial.print("Mac Addr: "); Serial.println(WiFi.macAddress());
}

void loop()
{
    getDistance();
#ifdef DEBUG
    Serial.println("Centimeter: ");
    Serial.print(wi.distance);
    Serial.print(": ");
    Serial.print(wi.percentage);
    Serial.println("");
#endif
    //digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));

    esp12e.send(NULL, (uint8_t *)&wi, sizeof(wi));
    ++retransmit;

    delay(100);
    if (retry || retransmit >= 5)
    {
        //then deep sleep
        Serial.println("going to deep sleep...");
        //
        // sleep for 10s
         ESP.deepSleep(10000000, WAKE_RF_DEFAULT);
        //ESP.restart();
        //delay(100);
    }
}
