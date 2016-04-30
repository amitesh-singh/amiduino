#include <Arduino.h>

#define LED 0 // At PB0

void setup() {
    // set the LED pin to be an output pin
    pinMode(LED, OUTPUT);
}

void loop() {

    while (true) {
        // write a 1 (digital signal high)
        digitalWrite(LED, HIGH);
        delay(100);
        // write a 0 (digital signal low)
        digitalWrite(LED, LOW);
        delay(200);
    }
}
