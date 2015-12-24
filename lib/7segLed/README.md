Seg7Led
========

This library is for making 7 segment display using LEDs.

The LEDs circuit should be made like below.
      1 <br/>
     ==== <br/>
    |   | 2 <br/>
   6| 7 | <br/>
    |===| <br/>
   5|   |3 <br/>
    |===| <br/>
      4   <br/>

Usage
=====
 * include following header in your arduino sketch.
   #include "Seg7Led.h"
 * Create Seg7Led object
<br/>
byte bytes[] = {2, 3, 4, 5, 6, 7, 8}; // Arduino digital ports number
<br/>
Seg7Led led(bytes);
<br/>
led.set(0); //display 0 on 7 segment LED
<br/>
led.set(9); // display 9 on 7 segment LED

* Sample

https://github.com/amitesh-singh/amiduino/blob/master/samples/display/Seg7Led.ino
