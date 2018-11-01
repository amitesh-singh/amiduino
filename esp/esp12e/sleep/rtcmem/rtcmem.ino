/*
   The stored data into RTC memory is retained between Deep Sleep cycles.
   However, the data might be lost after power cycling the esp8266.

   [system data, 256 bytes] -- [user data, 512 bytes]
   We can only access user data.

 */

#define SLEEP_TIME 5

struct rtcStore
{
   uint32_t crc32;
   u8 data[508];
};

static rtcStore rtcData;
static u32 crcData;

static uint32_t calculateCRC32(const uint8_t *data, size_t length)
{
   uint32_t crc = 0xffffffff;
   while (length--)
     {
        u8 c = *data++;
        for (u32 i = 0x80; i > 0; i >>= 1)
          {
             bool bit = crc & 0x80000000;
             if (c & i)
               {
                  bit = !bit;
               }
             crc <<= 1;
             if (bit)
               {
                  crc ^= 0x04c11db7;
               }
          }
     }
   return crc;
}

static void printMemory()
{
   char buf[3];
   u8 *ptr = (u8 *)&rtcData;
   for (u16 i = 0; i < sizeof(rtcData); i++)
     {
        sprintf(buf, "%02X", ptr[i]);
        Serial.print(buf);
        if ((i + 1) % 32 == 0)
          {
             Serial.println();
          }
        else
          {
             Serial.print(" ");
          }
     }
   Serial.println();
}

void setup()
{
   Serial.begin(9600);
   delay(1000);
   Serial.println();

   if (ESP.rtcUserMemoryRead(0, (uint32_t *)&rtcData, sizeof(rtcData)))
     {
        Serial.println("READ:");
        printMemory();

        Serial.println();

        crcData = calculateCRC32((u8 *)&rtcData.data[0], sizeof(rtcData.data));
        Serial.print("CRC32 of data: ");
        Serial.println(crcData);

        Serial.print("CRC32 of RTC Data: ");
        Serial.println(rtcData.crc32);

        if (crcData != rtcData.crc32)
          Serial.println("Data is invalid..");
        else
          Serial.println("Data is Valid..");
     }
   //generate random value for data
   for (u16 i = 0; i < sizeof(rtcData.data); ++i)
     rtcData.data[i] = random(0, 128);

   rtcData.crc32 = calculateCRC32((u8 *)&rtcData.data[0], sizeof(rtcData.data));

   if (ESP.rtcUserMemoryWrite(0, (u32 *)&rtcData, sizeof(rtcData)))
     {
        Serial.println("Write: ");
        printMemory();
     }

   //sleeps for 5ss
   ESP.deepSleep(SLEEP_TIME*1000000);
}

void loop()
{

}
