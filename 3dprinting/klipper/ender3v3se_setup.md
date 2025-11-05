# Installation

~~https://athemis.me/projects/klipper_guide/~~
https://pblvsky.gitbook.io/ender3v3se/remote-control/klipper

In a nutshell,

we need klipper, moonraker and one of the GUIs
- fluidd
- mainsail

I chose fluidd since most of the youtubers use this


make sure, you do following on rpi

```
pip install --upgrade pip setuptools wheel

```

the below might be needed in case of moonraker installation failure. 
```
pip install markupsafe
```

Use `KIAUH` to install klipper, moonraker and fluidd/mainsail

# Slicer Setup

Used following config: https://github.com/shubham0x13/ender-3-v3-se-klipper as the base.

my config: https://github.com/amitesh-singh/klipper_ender3v3se_config

## Start GCode in Orca Slicer

```
M220 S100 ;Reset Feedrate 
M221 S100 ;Reset Flowrate 
M104 S[nozzle_temperature_initial_layer] ;Set final nozzle temp 
M190 S[bed_temperature_initial_layer_single] ;Set and wait for bed temp to stabilize 
G28 ;Home 
BED_MESH_PROFILE LOAD=default
G92 E0 ;Reset Extruder 
G1 Z2.0 F3000 ;Move Z Axis up 
G1 X+1.1 Y20 Z0.28 F5000.0 ;Move to start position 
M109 S[nozzle_temperature_initial_layer] ;Wait for nozzle temp to stabilize 

LINE_PURGE
;G1 X+1.1 Y145.0 Z0.28 F1500.0 E15 ;Draw the first line 
;G1 X1.4 Y145.0 Z0.28 F5000.0 ;Move to side a little 
;G1 X+1.4 Y20 Z0.28 F1500.0 E30 ;Draw the second line 
G92 E0  ;Reset Extruder 
G1 E-1.0000 F1800 ;Retract a bit 
G1 Z2.0 F3000 ;Move Z Axis up 
G1 E0.0000 F1800

```


## End gcode in orca slicer

```
PRINT_END

```

# klipper address

it's running at the port 81.

# pr touch support

currently using 
