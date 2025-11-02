# bed mesh/levelling

## Adaptive bed mesh

go to macros.cfg and add following line in 

```
BED_MESH_CALIBRATE ADAPTIVE=1 ADAPTIVE_MARGIN=5
```

In orca slicer, add following entries

```
PRINT_START BED=[bed_temperature_initial_layer_single] EXTRUDER=[nozzle_temperature_initial_layer]

```
In orca, both label object and exclude object boxes must be selected.
![orca settings](image-1.png)
