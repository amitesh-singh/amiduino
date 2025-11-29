# Setting up wifi on brother HL-B2000D laser printer

Refer to this guide:
https://github.com/alexivkin/brother-in-arms

## manual process

the cups wrapper driver for HL-B2000D has been deleted from brother server

```
sudo apt install cups samba psutils

```

We need to generate two debian packages.

download the debian package from brother website for i386 platform, named `hlb2000dpdrv-4.0.0-1.i386.deb`

you have to run following commands on rpi2

```
dpkg -x hlb2000dpdrv-4.0.0-1.i386.deb hlb2000dpdrv-4.0.0-1.armhf.extracted
dpkg-deb -e hlb2000dpdrv-4.0.0-1.i386.deb hlb2000dpdrv-4.0.0-1.armhf.extracted/DEBIAN
cp hlb2000dpdrv-4.0.0-1.armhf.extracted/opt/brother/Printers/BrGenPrintML2/lpd/armv7l/rawtobr3 hlb2000dpdrv-4.0.0-1.armhf.extracted/usr/local/Brother/Printer/HLB2000D/lpd

```

Grab the Brother ARM drivers from a generic armhf archive they provide and copy the ARM code into the unpacked folders. Note that HL2270/HLB2000D does not use brprintconflsr3.

```
wget http://download.brother.com/welcome/dlf103361/brgenprintml2pdrv-4.0.0-1.armhf.deb
dpkg -x brgenprintml2pdrv-4.0.0-1.armhf.deb brgenprintml2pdrv-4.0.0-1.armhf.extracted
# cp brgenprintml2pdrv-4.0.0-1.armhf.extracted/opt/brother/Printers/BrGenPrintML2/lpd/armv7l/brprintconflsr3 hlb2000dpdrv-4.0.0-1.armhf.extracted/usr/local/Brother/Printer/HLB2000D/lpd
cp brgenprintml2pdrv-4.0.0-1.armhf.extracted/opt/brother/Printers/BrGenPrintML2/lpd/armv7l/rawtobr3 hlb2000dpdrv-4.0.0-1.armhf.extracted/usr/local/Brother/Printer/HLB2000D/lpd

```

Now repackage it,

```
cd hlb2000dpdrv-2.1.0-1.armhf.extracted
find . -type f ! -regex '.*.hg.*' ! -regex '.*?debian-binary.*' ! -regex '.*?DEBIAN.*' -printf '%P ' | xargs md5sum > DEBIAN/md5sums
cd ..
chmod 755 hlb2000dpdrv-4.0.0-1.armhf.extracted/DEBIAN/p* hlb2000dpdrv-4.0.0-1.armhf.extracted/usr/local/Brother/Printer/HLB2000D/inf/* hlb2000dpdrv-4.0.0-1.armhf.extracted/usr/local/Brother/Printer/HL2270DW/lpd/*
dpkg-deb -b hlb2000dpdrv-4.0.0-1.armhf.extracted hlb2000dpdrv-4.0.0-1.armhf.deb
```

### Repackage CUPS wrapper
somehow i could not find brlhl2000dwcups package and we are going to use this one instead.. 
```
wget https://download.brother.com/welcome/dlf006733/brhl2270dwcups_src-2.0.4-2.tar.gz
tar zxvf brhl2270dwcups_src-2.0.4-2.tar.gz
cd brhl2270dwcups_src-2.0.4-2

```

compile `brcupsconfig4`

```
gcc brcupsconfig3/brcupsconfig.c -o brcupsconfig4
```

Grab the original i386 CUPS wrapper and unpack it

again cupswrapperHLB2000D was not found 
```
wget https://download.brother.com/welcome/dlf005895/cupswrapperHL2270DW-2.0.4-2.i386.deb
dpkg -x cupswrapperHL2270DW-2.0.4-2.i386.deb cupswrapperHL2270DW-2.0.4-2.armhf.extracted
dpkg-deb -e cupswrapperHL2270DW-2.0.4-2.i386.deb cupswrapperHL2270DW-2.0.4-2.armhf.extracted/DEBIAN
sed -i 's/Architecture: i386/Architecture: armhf/' cupswrapperHL2270DW-2.0.4-2.armhf.extracted/DEBIAN/control
```

```
cp brhl2270dwcups_src-2.0.4-2/brcupsconfig4 cupswrapperHL2270DW-2.0.4-2.armhf.extracted/usr/local/Brother/Printer/HL2270DW/cupswrapper
```

Repack it

```
cd cupswrapperHL2270DW-2.0.4-2.armhf.extracted
find . -type f ! -regex '.*.hg.*' ! -regex '.*?debian-binary.*' ! -regex '.*?DEBIAN.*' -printf '%P ' | xargs md5sum > DEBIAN/md5sums
cd ..
chmod 755 cupswrapperHL2270DW-2.0.4-2.armhf.extracted/DEBIAN/p* cupswrapperHL2270DW-2.0.4-2.armhf.extracted/usr/local/Brother/Printer/HL2270DW/cupswrapper/*
dpkg-deb -b cupswrapperHL2270DW-2.0.4-2.armhf.extracted cupswrapperHL2270DW-2.0.4-2.armhf.deb
```

```
sudo dpkg -i hlb2000dpdrv-2.1.0-1.armhf.deb cupswrapperHL2270DW-2.0.4-2.armhf.deb
```

I also install hlb2000dpdrv-4.0.0-1.i386.deb
```
sudo dpkg -i 

```

## Disabling the journald logs
Also refer to:
https://github.com/amitesh-singh/amiduino/tree/master/3dprinting/octoprint#writing-to-sd-card
quite handy for powering off rpi without sd card crash
