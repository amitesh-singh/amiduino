# disable usb autosuspend


had some issue lately with usb connections on rpi4

```
sudo vim /etc/systemd/system/disable-usb-autosuspend.service


[Unit]
Description=Disable USB Autosuspend
After=systemd-modules-load.service
Before=klipper.service

[Service]
Type=oneshot
ExecStart=/bin/sh -c 'echo -1 > /sys/module/usbcore/parameters/autosuspend'
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target




sudo systemctl enable disable-usb-autosuspend.service
```


