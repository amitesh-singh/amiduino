
#include <Servo.h>  // servo library


Servo servo1;  // servo control object

static void servomove(int degree)
{
  servo1.attach(9);
  delay(10);
  servo1.write(degree);
  delay(1000);
  servo1.detach();
  //making pwm pin low will save more power?
  digitalWrite(9, LOW);
}

void setup()
{
  servomove(0);
  delay(5000);

}


void loop()
{
  int position;

  // To control a servo, you give it the angle you'd like it
  // to turn to. Servos cannot turn a full 360 degrees, but you
  // can tell it to move anywhere between 0 and 180 degrees.

  // Change position at full speed:

  servomove(90);    // Tell servo to go to 90 degrees

  delay(1000);         // Pause to get it time to move

  servomove(180);   // Tell servo to go to 180 degrees

  delay(1000);         // Pause to get it time to move

  servomove(0);     // Tell servo to go to 0 degrees

  delay(1000);         // Pause to get it time to move
}

