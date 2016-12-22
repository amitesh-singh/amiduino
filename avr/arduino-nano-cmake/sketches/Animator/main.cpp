#include <UTFT.h>
#include <URTouch.h>

#define TOUCH_ORIENTATION  LANDSCAPE//PORTRAIT

UTFT    myGLCD(CTE32HR, 38, 39, 40, 41);

URTouch  myTouch( 6, 5, 4, 3, 2);
#include "animator.h"

animator anim1;
coreloop apploop(60);

void _anim1_cb(void *d)
{
   Serial.println("anim1_cb");
}

void setup()
{
   Serial.begin(115200);
   anim1.add(_anim1_cb, 0);
}

void loop()
{
   apploop.loop();
}
