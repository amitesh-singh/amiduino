#include <Arduino.h>
#include <string.h>

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 10;
BLEScan *pBLEScan;

volatile bool mibandDetected = false;

bool detectMiBand3(BLEAdvertisedDevice &advertisedDevice)
{
  if (advertisedDevice.getName() == "Mi Band 3" || advertisedDevice.getAddress().toString() == "d0:5e:1a:26:bb:05")
  {
    return true;
  }

  return false;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s ", advertisedDevice.toString().c_str());
      Serial.print("RSSI: ");
      Serial.println(advertisedDevice.getRSSI());
    }
};

const int lightpin = 26;

void setup()
{
  Serial.begin(115200);
  Serial.println("Scanning..");

  //still trigger issue on bootup ..
  pinMode(lightpin, OUTPUT);
  digitalWrite(lightpin, HIGH);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

int bleDevicesCount = 0;
enum LightState
{
  ON,
  OFF
};

LightState lightstate = OFF;

const int THRESHOLD = -70;

void loop()
{
   // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  bleDevicesCount = foundDevices.getCount();
  Serial.println(bleDevicesCount);
  Serial.println("Scan done!");

  for (int i = 0; i < bleDevicesCount; ++i)
  {
    BLEAdvertisedDevice d = foundDevices.getDevice(i);
    if (detectMiBand3(d))
    {
      Serial.println(d.getRSSI());
      if (d.getRSSI() >= THRESHOLD && lightstate != ON)
      {
          Serial.println("light is ON");
          digitalWrite(lightpin, LOW);
          lightstate = ON;
      }
      else if (d.getRSSI() < THRESHOLD && lightstate == ON)
      {
        Serial.println("Light is OFF");
        lightstate = OFF;
        digitalWrite(lightpin, HIGH);
      }
    }
  }
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(10000);
}
