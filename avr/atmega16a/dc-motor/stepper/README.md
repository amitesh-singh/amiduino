### Notes

I have a BYJ48 Stepper Motor and a ULN2003 driver board module.  
You must power this device externally not from MCU circuit.  
This Motor has a Gear ratio of 64 , and Stride Angle 5.625° so this motor has a 4096 Steps.  

steps = Number of steps in One Revolution  * Gear ratio.  
steps= (360°/5.625°)*64"Gear ratio" = 64 * 64 =4096  

This motor has 4 coils and 5 connections to be made if you have the ULN2003 board.  

1. blue  
2. pink  
3. yellow  
4. orange  
5. RED   

#### Stepper Squence

##### clockwise

`1001,1100,0110,0011`

##### anti-clockwise

just reverse the sequence `0011 0110 1100 1001`
