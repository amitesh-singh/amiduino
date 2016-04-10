/**
 * ...
 *
 * @author Matthias L. Jugel
 *
 * Copyright 2015 ubirch GmbH (http://www.ubirch.com)
 *
 * == LICENSE ==
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <Arduino.h>

#define LED 13

void setup() {
    // set the LED pin to be an output pin
    pinMode(LED, OUTPUT);
}

// this tells the IDE and the compiler, that it's okay to have an endless loop
// (we never return from this function)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

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

#pragma clang diagnostic pop