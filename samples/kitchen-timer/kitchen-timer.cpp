//TODO: Add Seg7Led header instead and use the latest code. :)

class Seg7Led
{
  byte *ledIndex;
  byte convert(byte value)
  {
    switch(value)
    {
      case 1:
       return 6;
      case 2:
      return 91;
      case 3:
      return 79;
      case 4:
      return 102;
      case 5:
      return 109;
      case 6:
      return 125;
      case 7:
      return 7;
      case 8:
      return 127;
      case 9:
      return 111;
      case 0:
      return 63;
    }
  }

  public:
  void reset()
  {
    for (byte i = 0; i < 7; ++i)
      digitalWrite(ledIndex[i], HIGH);
  }
  Seg7Led(byte bytes[])
  {
    ledIndex = bytes;
    for (byte i = 0; i < 7; ++i)
    {
      pinMode(bytes[i], OUTPUT);
    }
  }

  void set(byte value)
  {
    reset(); // reset all pins to low
    value = convert(value);
    for (byte i = 0; i <= 7; ++i)
    {
      if ((value >> (i)) & 1)
      {
        //Serial.println(i);
        //Serial.println( ledIndex[i]);
        digitalWrite(ledIndex[i], LOW);
      }
    }

  }
};

#define SPEAKERPIN 10
byte bytes[] = {2, 3, 4, 5, 6, 7, 8};
Seg7Led led(bytes);
byte val= HIGH;
unsigned long startTime = 0;
byte isAlarm = 0;
// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  pinMode(9, INPUT);
  pinMode(SPEAKERPIN, OUTPUT);

  led.set(0);
  startTime = 0;

   tone(SPEAKERPIN, 400, 200);
   delay(200);
   noTone(10);

   digitalWrite(9, HIGH);
}

byte count = 0;

void loop()
{
   val = digitalRead(9);

  if (val == LOW)
  {
    if (count == 0 && isAlarm == 1)
    {
      noTone(SPEAKERPIN);
      isAlarm = 0;
      delay(1000);
      Serial.println("Setting buzzer off and not increasing counting");
      //digitalWrite(9, HIGH);
      //count = 0;
      return;
    }
    ++count;
    if (count == 10)
      count = 0;
    led.set(count);
    delay(900);

    if (count > 0)
    {
      //endTime = startTime;
      Serial.println("Start the timer");
      startTime = millis();

      //Serial.println(startTime);
      isAlarm = 0;
    }
  }
  else if (isAlarm == 0)
  {
    unsigned long timeLimit = ((unsigned long)(count)) * 60 * 1000;
    //Serial.println(timeLimit);
    if (count > 0 && (millis() - startTime) >= timeLimit)
    {
      //timer expires
      Serial.println("Timer expires");
      // reset the led to 0
      // make sound on buzzer in next cycle :)
      led.set(0);

      isAlarm = 1;
      // set count = 0
      count = 0;
      startTime = 0;
    }

  }

  if (isAlarm)
  {
       tone(SPEAKERPIN, 400, 200);
       delay(200);
       noTone(SPEAKERPIN);
  }

}
