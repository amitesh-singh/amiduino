
//// THIS DID NOT WORK           /// Don;t know why :/

// This example blinks the LED (GPIO_13) in every 1 second
static const unsigned long CPU_MHZ = 80000000; //80Mhz -> 80*10^6 = 1 second
volatile uint8_t toggle;

void inline timer1_ISR(void)
{
   toggle = (toggle == 1) ? 0: 1;
   digitalWrite(13, toggle);

   // if you want timer1 to be called just once, don't call timer1_write the below stuff
   // Set-up the next interrupt cycle
   timer1_write(ESP.getCycleCount() + CPU_MHZ);
}

void setup()   {
   pinMode(13, OUTPUT);
   noInterrupts();
   timer1_isr_init();
   timer1_attachInterrupt(timer1_ISR);

   timer1_write(ESP.getCycleCount() + CPU_MHZ);

   interrupts();
}

void loop()
{
   delay(1000);
}
