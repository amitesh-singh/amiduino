#include <BluetoothSerial.h>

BluetoothSerial serialBT;

void setup()
{
    Serial.begin(115200);
    serialBT.begin("ESP32");

    Serial.println("The device is started, pair it with BT.");
}

void loop()
{
    if(Serial.available())
    {
        serialBT.write(Serial.read());
    }

    if (serialBT.available())
    {
        Serial.write(serialBT.read());
    }

    delay(20);
}