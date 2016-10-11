## Notes

1. Make sure you connected GPIO_16 to RST then only ESP.deepSleep() works.

2. For amazon dash button kind of implementations, it would be best to attach a button to RST  (10 K ohms pullup to Vcc) and on power up, do the job and then go to deep sleep for forever until device is not reset manually.  
