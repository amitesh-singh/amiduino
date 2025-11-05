# using the load cell on ender 3v3 se

use following fork:
https://github.com/0xD34D/klipper_ender3_v3_se

AFAIK, the work has been merged into upstream klipper. TODO: find out how to 
make it work with upstream klipper.

use following command

```
PRTOUCH_PROBE_ZOFFSET

```

This will calculate z offset, then run `SAVE_CONFIG` afterwards


