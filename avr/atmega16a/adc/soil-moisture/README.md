### Soil moisture measurement

1. http://www.gadgetronicx.com/automatic-plant-watering-system/


#### My experiments/calibration results

1. Tap Water - 325
2. Okay moistured POT - 550 - 600
3. In Air - 1023, so exclude this value from code

#### TODOs

1. make your own alternate soil moisture device? cheaper? check the gadgettronics link for details.


### Implementations

There are two ways to implement this.

1. Simple
  * Read the soil moisture device analog in every 2 hrs or so time interval and apply the logic like start the motor
2. power save
  * Read the soil moisture device digital (adjust the potentio-meter) port at INT0 and whenever the interrupt go high - start the motor

  I might need to put logic on how much time should i run the motor?

#### Motor run time

This is interesting actually. Two ways to achieve this.

1. i can run motor for a fixed time and then check the moisture reading and if its below threshold, no need to run it again?
2. i can keep checking moisture value (in every 10 seconds) when pump is running and whenever the value
is below threshold, stop the motor.

Need to experiment.

