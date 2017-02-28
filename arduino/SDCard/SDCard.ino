/*
- do format the SD Card
  SD card module   Arduino Uno

 MOSI            -    11
 MISO            -    12
 SCK             -    13
 CS              -    4
*/

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

File myFile;

void setup()
{
    Serial.begin(9600);
    if (!Serial) {} // only needed for mcu with native usb support. like m32u4

    Serial.println("Initializing SD Card....");

    if (!SD.begin(4)) 
    {
        Serial.println("Initialization failed.");
        return;
    }

/*
   //uncomment the below code if writing to SD card for first time.
    Serial.println("Initialization Done.");
    myFile = SD.open("test.txt", FILE_WRITE);
    myFile.println("testing 1, 2, 3.");
    myFile.close();
*/
    myFile = SD.open("test.txt");
    Serial.println("test.txt:");

    while (myFile.available())
    {
        Serial.write(myFile.read());
    }
    myFile.close();
}

void loop()
{
}

