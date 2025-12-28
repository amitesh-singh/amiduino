# Making PCBs

## Kicad
### kicad board setup

Do this before starting the work.
#### Constrains

`File -> Board Setup`
- Min clearance: 0.5 mm
- min track width: 0.5 mm
- copper to hole clearance: 0 mm
- copper to edge clearance: 0 mm

![kicad setup](image-1.png)
- Set the grid origin to bottom left side in Kicad `PCB editor`
![Set origin kicad](image.png)

#### Net Classes
- “Default”
  Clearance: 0.5 mm
  Track width: 0.77 mm

- “Large” (add new)
  Clearance: 0.5 mm
  Track width: 2.04 mm
![Net Classes](image-2.png)

#### Thermal relief 
- set it to 1.2 mm

![Thermal relief](image-3.png)

#### Pad size

depending upon the orientation 

- x - set it default. 
- y - 2.04 mm 

![pad property](image-4.png)

### Origin in freecad

set the grid and drill origin to right-bottom side.

![right bottom](image-5.png)
## pcb2gcode: exporting gerber to gcode

create a output file and dump gerber and drl files there:

`led` is the project name

```
back=led-B_Cu.gbr
drill=led.drl
outline=led-Edge_Cuts.gbr

# Generic
metric=true         # use metric units for parameters
metricoutput=true   # use metric units for output
nog64=true          # do not set an explicit g64
nom6=true           # do not emit m6
zsafe=2             # The height in mm at which the bit can move freely without obstruction
zchange=35          # Tool changing height in mm
output-dir=./

# Place a 5x7cm board in the lower right quadrant of the coordinate system
# This will allow you to probe the fixed jaw of the vise for (0,0) on the CNC.

mirror-axis=6.275    # set this to half of your board width

# Drilling
zdrill=-2.5         # drilling depth
drill-feed=200      # Vertical mm/min feed
drill-speed=24000   # Spindle RPM
onedrill=true       # Use a single drill for all holes
nog81=true          # replace G81 with G0+G1 (no G81 in GRBL)

# Milling
zwork=-0.1          # V-bit plunge depth
#mill-diameters=0.11 # 60 deg V-bit dia at -0.1 plunge depth
#mill-diameters=0.08 # 45 deg V-bit dia at -0.1 plunge depth
mill-diameters=0.05 # 30 deg V-bit dia at -0.1 plunge depth
mill-speed=24000    # Spindle RPM
mill-feed=600       # Horizontal feedrate in mm/min
mill-vertfeed=100   # Plunge rate in mm/min
voronoi=true        # cuts the milling time significantly, but check with this on and off if everything looks ok
preserve-thermal-reliefs = true # has effect only if voronoi=true

# Cutting
zcut=-2.5
cutter-diameter=2
cut-feed=80
cut-infeed=100
cut-speed=24000
cut-side=back

```

- generate_mill.sh

```
#!/bin/bash

docker run --rm -i -t -v ".:/data" ptodorov/pcb2gcode

cat back.ngc > combined.ngc
cat drill.ngc >> combined.ngc
cat outline.ngc >> combined.ngc

```

## Milling bits 

- mill PCBs with a 0.2 mm 45 degree V bit (which is effectively 0.4mm for z depth of 0.3mm)
- cut the outline with a 1.5 mm bit
- drill holes with a 1 mm drill bit
- For mounting holes/slots, use 1.5mm, 2.5mm or 3.175 mm drills


## how to simulate 

you can combine both `back.ngc` and `drill.ngc` to check if everything is working fine
- https://harvie.github.io/cnc-simulator
  set the bit diameter to 0.3, and angle to 180

- https://ncviewer.com/


- 
## Links
- https://mateusznowak.dev/articles/manufacturing-pcb-at-home/
- https://peterprototypes.com/blog/pcb2gcode/
- https://mike-ensor.medium.com/first-homemade-pcb-33392c511d83
