int state = HIGH;
char readByte;

void setup()
{
  pinMode(13, OUTPUT);
  // initialize serial connection with 9600 baud rate
  Serial.begin(9600);
  Serial.println("Starting to read from Serial input -- 1 means LED on and 0 mean LED off");
}

void loop()
{
  if (Serial.available())
  {
    //convert char into int (only works with 0 - 9 input)
    // read the input from serial port
    int incomingState = (Serial.read() - '0');
    if (incomingState != state)
    {
      state = incomingState;
      digitalWrite(13, incomingState);
      Serial.println("writing to 13");
      Serial.println(incomingState);
    }
  }

}
