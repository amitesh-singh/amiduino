//file: main.cpp
#include "Arduino.h"
#include "simple.h"

void setup(){
  Serial.begin(115200);
  simpleFunc();
}

void loop(){
  Serial.println("loop");
  delay(1000);
}
