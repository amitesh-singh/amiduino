const char PROGMEM myName[] = "Esp12E";
const uint8_t myInt PROGMEM = 10;

void setup()
{
   Serial.begin(115200);
   delay(8000);

   char temp[10];

   //check pgmspace.h - cores/esp8266/
   //this is how you load PROGMEM into RAM.
   // functions like memcpy_P, memccpy_P,
   // memmem_P, memcmp_P, strncpy_P,
   //strncmp_P, strncat_P, strnlen_P, strstr_P
   memcpy_P(temp, myName, 7);

   Serial.println("Starting..");
   Serial.println(temp);

   //pgm_read_word,float,dword,byte,ptr,
   uint8_t byte1 = pgm_read_byte(&myInt);
   Serial.print(byte1);

   delay(100);
}

// the loop function runs over and over again forever
void loop()
{
   Serial.println("looping..");
   delay(5000);
}
