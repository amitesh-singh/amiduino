# Understanding quality parameters in Orcaslicer

```
volumetric speed = linear speed * layer width * layer height * flow ratio;
```

e.g. in generic PLA profile, the max volumetric speed is set to 12 mm3/s

Lets compare this with what values we add in Quality (Line Width) and Speed


*  First Layer 
First layer:  0.46 mm 
  First Layer: 30 mm/s

Generic PLA profile: 
 volumetric speed: 12 mm3/s
 flow ratio = 0.98

 12 = linear speed * 0.2 * 0.46 ; 
 max linear speed = 130 mm/s

 so theoritically, the max first layer speed can go up to <= 130 mm/s
 For better adhesion, we set the first layer speed to be < 50 mm/s
In bambu lab, it's set to 50 mm/s 

* First layer infill
  - this is the solid fill inside the part on layer 1; same as `internal solid infill` but slower; 
  In quality, internal solid infill = 0.42; 
  
## layer height

### Layer height
The key rule in FDM printing:

Layer height should be `~25% to 75%` of nozzle diameter

```
Layer height ≈ 0.25 × nozzle  → minimum
Layer height ≈ 0.75 × nozzle  → maximum
```
e.g. 
nozzle dia = 0.4 mm

minimum layer height = 0.4 * 0.25 = 0.1 mm
max layer height = 0.4 * 0.75 = 0.3 mm

We generally use 0.2 mm layer height which is like 50% of the nozzle dia. Pretty balanced.

### First layer height

```
First layer height = 50%–75% of nozzle diameter
```
- 0.2 mm -> most common (best balance)
- 0.24 - 0.28 mm -> For stronger adhesion
- < 0.16 mm -> harder to tune and less forgiving

- ✅ Standard recommendation 
0.20 mm first layer

→ Works in almost all cases

- 🔥 If you want better bed adhesion
0.24 mm
→ Less sensitive to Z-offset

→ Good for textured PEI / uneven beds

- ⚠️ If you want ultra precision
0.16 mm
→ Only if your bed leveling is perfect
→ Not recommended for daily use

## Line Width 

Typical usable range:

~80% → 120–150% of nozzle diameter

For 0.4 mm nozzle,
0.3 - (0.48 - 0.6 mm) 
### Default
This is the base value used everywhere unless overridden.

