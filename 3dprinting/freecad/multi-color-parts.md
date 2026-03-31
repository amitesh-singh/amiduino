# multi color parts in freecad

This is handy if you want to print multi-color prints. 
This require the use of both `part design` and `part` workbench

Refer to https://www.youtube.com/watch?v=S2ySQaKKiHA tutorial on this.


You need to save the file as `3mf`

## Process 

### FreeCAD
Go to `Part` workbench and then select each operations (left side panel) from bottom to top
`Part -> Split -> Slice to compound`

Select the `SliceXXX` body and export it as a 3mf file.

### Orcaslicer

drag and drop the 3mf file into orca and right click and `split->parts` 
