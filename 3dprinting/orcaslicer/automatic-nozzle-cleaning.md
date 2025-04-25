# Nozzle cleaning like A1 on ender 3v3 se

- print following mod: https://www.printables.com/model/625480-brush-mount-for-ender-3-v3-se
- 


## start g code

Orca slicer -> Printer -> Machine G code -> Machine start g code


```
M220 S100 ;Reset Feedrate 
M221 S100 ;Reset Flowrate 
 
M104 S[nozzle_temperature_initial_layer] ;Set final nozzle temp 
M190 S[bed_temperature_initial_layer_single] ;Set and wait for bed temp to stabilize 
G28 ;Home 
M420 S1 ; enable bed mesh levelling
G92 E0 ;Reset Extruder 
G1 Z2.0 F3000 ;Move Z Axis up 
G1 X-2.1 Y20 Z0.28 F5000.0 ;Move to start position 
M109 S[nozzle_temperature_initial_layer] ;Wait for nozzle temp to stabilize 

; for cleaning the nozzle
G1 X30 Y-12 Z6 F5000.0 ;Start posistion for wipe
G1 Z4.0 F5000.0 ;Lower to wipe height
G1 X75 Y-10 Z4.0 F5000.0 ;Wipe 1
G1 X30 Y-12 Z4.0 F5000.0 ;Wipe 2
G1 X75 Y-14 Z4.0 F5000.0 ;Wipe 3
G1 X60 Y-10 Z4.0 F500.0 ;Wipe 4
G1 Z7 F5000.0 ; Move up
G1 X-2.1 Y20 Z0.28 F5000.0 ;Move to start position 
; nozzle cleaning ended

G1 X-2.1 Y145.0 Z0.28 F1500.0 E15 ;Draw the first line 
G1 X-2.4 Y145.0 Z0.28 F5000.0 ;Move to side a little 
G1 X-2.4 Y20 Z0.28 F1500.0 E30 ;Draw the second line 
G92 E0  ;Reset Extruder 
G1 E-1.0000 F1800 ;Retract a bit 
G1 Z2.0 F3000 ;Move Z Axis up 
G1 E0.0000 F1800
```