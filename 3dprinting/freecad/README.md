# FreeCAD tips

## Add-Ons

Mod Path for apt-get installed freecad or downloaded one: ~/.local/share/FreeCAD/Mod

since i use dev version: mod path is ~/snap/freecad/common/Mod
- Fused Filament Design
- KicadStepup - for importing kicad PCBs
  To make kicadstepup mod work, I had to do some changes

Add following lines in ~/.profile and restart the laptop

```
export KICAD9_3DMODEL_DIR="/usr/share/kicad/3dmodels"
export KISYS3DMOD="/usr/share/kicad/3dmodels"
```

it seems that that did not work.. i had to modify the kicad_pcb project file 

```
--- a/projects/wled-esp01-controller/wled-esp01-controller.kicad_pcb
+++ b/projects/wled-esp01-controller/wled-esp01-controller.kicad_pcb
@@ -40,7 +40,7 @@
                (aux_axis_origin 99.5 92.25)
                (grid_origin 99.5 92.25)
                (pcbplotparams
-                       (layerselection 0x00000000_00000000_55555555_5755f5ff)
+                       (layerselection 0x00000000_00000000_55555555_57555555)
                        (plot_on_all_layers_selection 0x00000000_00000000_00000000_00000000)
                        (disableapertmacros no)
                        (usegerberextensions no)
@@ -2153,7 +2153,7 @@
                        (uuid "c4a9e75b-ec78-48b9-a617-aaf98823103c")
                )
                (embedded_fonts no)
-               (model "${KICAD9_3DMODEL_DIR}/Connector_PinHeader_2.54mm.3dshapes/PinHeader_1x02_P2.54mm_Vertical.step"
+               (model "${KICAD6_3DMODEL_DIR}/Connector_PinHeader_2.54mm.3dshapes/PinHeader_1x02_P2.54mm_Vertical.step"
                        (offset
```


