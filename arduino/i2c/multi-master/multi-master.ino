//!- Multimaster mode for Arduino
//
//This examples shows how  to setup 2 arduinos as master and communicate to each other. 
// we are going to use Wire library
//Swap MY_ADDRESS to OTHER_ADDRESS for other arduino.
//
// A4 (arduino1, master 1)   - A4 (arduino2, master 2) SDA
// A5 (arduino1, master 1)   - A5 (arduino2, master2)  SCL
// (C) Amitesh Singh - 2017

#include <Arduino.h>
#include <Wire.h>

#define MY_ADDRESS 0x08
#define OTHER_ADDRESS 0x09

#define BUTTON 10
#define LED 13

static void receiveEvent(int);

void setup()
{
    pinMode(LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);

    Wire.begin(MY_ADDRESS);
    Serial.begin(9600);

    Wire.onReceive(receiveEvent);
}

byte last_state = 0;
void loop()
{
    if (digitalRead(BUTTON) != last_state)
    {
        last_state = digitalRead(BUTTON);
        //send this change to other master
        Wire.beginTransmission(OTHER_ADDRESS);
        Wire.write(last_state);
        Wire.endTransmission();
    }
}

static void receiveEvent(int howmany)
{
    while (Wire.available() > 0)
    {
        byte c = Wire.read();
        digitalWrite(LED, !c);
        Serial.print(c);
    }
    Serial.println("");
}