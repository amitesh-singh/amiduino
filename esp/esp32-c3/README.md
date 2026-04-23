# esp32 c3 super mini

bought from chowdani chauk - 250/- INR


For serial/uart over usb  to work, enable `USB CDC On Boot` in arduino IDE.

To compile with `arduino-cli`, add `:CDCOnBoot=cdc`

```
arduino-cli compile --fqbn esp32:esp32:esp32c3:CDCOnBoot=cdc .
```
