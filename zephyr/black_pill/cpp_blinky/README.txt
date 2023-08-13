west is installed via pip for the $USER
----------------------------------------
export ZEPHYR_BASE=/home/ami/zephyrproject/zephyr

build for blue pill
--------------------
west build -p always -b stm32f103_mini

with overlay
------------
west build -p always -b stm32f103_mini --  -DDTC_OVERLAY_FILE=./boards/stm32f103_min.overlay

for posix
---------
west build -p always -b native_posix_64


flash
------
west flash does not work with stm32 linkv2
st-flash write build/zephyr/zephyr.bin 0x08000000

black pill is already there
---------------------------

west boards | grep stm32


west build -p always -b stm32_min_dev_black

