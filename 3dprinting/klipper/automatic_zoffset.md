# using the load cell on ender 3v3 se

use following fork:
https://github.com/0xD34D/klipper_ender3_v3_se

this fork implements hx711s.c and dirzctl.c, which upstream does not have.

use following command

```
PRTOUCH_PROBE_ZOFFSET

```

This will calculate z offset, then run `SAVE_CONFIG` afterwards

I've a macro too

![automatic z offset macro](image-2.png)

