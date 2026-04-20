# how to back up klipper image


## making a backup
put sd card into the PC, running linux

```
dd if=/dev/sdX of=klipper_rpi2.img bs=4M status=progress conv=fsync
```

## compress the img

```
gzip klipper_rpi2.img
```
directly while imaging

```
sudo dd if=/dev/sdX bs=4M status=progress | gzip > sdcard.img.gz
```

## Restore the image to another sd card

```
sudo dd if=klipper_rpi2.img of=/dev/sdX bs=4M status=progress conv=fsync
```

if compressed:

```
gunzip -c sdcard.img.gz | sudo dd of=/dev/sdX bs=4M status=progress conv=fsync
```

## Verify

Verify checksum

```
sha256sum sdcard.img
```

After restore:

```
sudo dd if=/dev/sdX bs=4M count=$(($(stat -c%s sdcard.img)/(4*1024*1024))) | sha256sum
```