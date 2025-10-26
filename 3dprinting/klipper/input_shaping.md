# input shaping


## x
place adxl or mpu on tool head and run following command 
```
TEST_RESONANCES AXIS=X FREQ_START=5 FREQ_END=100 OUTPUT=raw_data
```
## y
place 


 
This is the command for doing everything
```
SHAPER_CALIBRATE

```

## how to set input shaper in printer.cfg
### by command

```
SET_INPUT_SHAPER SHAPER_FREQ_X=42 SHAPER_FREQ_Y=40 DAMPING_RATIO_X=0.15 DAMPING_RATIO_Y=0.15
```
