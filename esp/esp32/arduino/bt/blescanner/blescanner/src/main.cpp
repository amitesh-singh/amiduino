#include <Arduino.h>
#include <string.h>

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 10;
BLEScan *pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s ", advertisedDevice.toString().c_str());
      Serial.print("RSSI: ");
      Serial.println(advertisedDevice.getRSSI());

    }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Scanning..");
  
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop()
{
   // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}