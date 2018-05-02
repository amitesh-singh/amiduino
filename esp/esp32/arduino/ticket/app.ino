#include <Ticker.h>

Ticker led1;
Ticker led2;

void blinkled1()
{
    //led 1 blinks
    Serial.println("led1 blinks");
}

void blinkled2()
{
    Serial.println("led2 blinks");
}

void setup()
{
    Serial.begin(9600);

    //0.1 second
    led1.attach(1, blinkled1);
    //2.5 second
    led2.attach(2.5, blinkled2);
}

void loop()
{
	
}
