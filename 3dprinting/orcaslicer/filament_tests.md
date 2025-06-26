# Filament Test guide

Following orca slicer calibration tests are conducted:

## Wol3d PLA Pro+ Red

### Pressure/Linear Advance test

- ![LA line test - orca slicer](image-3.png)

   By looking at the image, it seems that 0.062 is a sweet spot
   Saved this settings for wol3d red pro+ into orca slicer


### Retraction Test

This test is done to avoid stringing;
- it turned out to be great from 0.3 to 1.5; set it to `1.0` mm seems to be sweet spot

### tolerance test

- 0.2 seems to be fine; 0.25 gap will be sweet spot
- 0.3 seems to be loose
- 0.4 mm is ultra loose

### max volumentric speed

generally this is provided by the vendor; 

I reduced it to 12mm and it yearns better results.

### flow ratio

reduced it to 0.96 from 0.98.. less overflow; 

Refer to (detailed flow ration)[./flow-ratio-calibration.md]

## Numaker PETG violet

### tolerance test

- 0.2 mm seems to be sweet spot
- 0.3 mm for more loose
- 0.4 mm is quite loose


## Numakers 

Following orca slicer calibration tests are conducted:

### PETG 

#### tolerance test

- 0.2 seems to be fine; 0.25 gap will be sweet spot
- 0.3 seems to be fine as well
- 0.4 mm is ultra loose

### ASA black

Calibration tests were not done

#### Pressure advance

did the lines test
- value set is 0.02;

#### Flow ratio

This is also provided by numakers which is 0.95
flow ratio is set to 0.926; 0.95 will work too?


#### volumentric speed limit

numakers recommends it to be 12

## Dream Polymers

### TPU 95A black

#### Pressure advance test
 
- set the value to 0.04

