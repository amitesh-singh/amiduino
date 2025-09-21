# How to add downloaded bins into the UI launcher

- on ubuntu 24.04, since official freecad is pretty old and it would
be better to download it instead.

create a file named freecad.desktop under ~/.local/share/applications

```
$~/.local/share/applications$ cat Freecad.cinnamon-generated.desktop
[Desktop Entry]
Type=Application
Encoding=UTF-8
Name=Freecad
Comment=Freecad
Exec=/home/$USER/Downloads/FreeCAD_1.0.2-conda-Linux-x86_64-py311.AppImage
Icon=/home/$USER/Downloads/freecad-icon.png
Terminal=false
StartupNotify=true
```

