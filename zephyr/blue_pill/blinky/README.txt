west is installed via pip for the $USER
----------------------------------------
export ZEPHYR_BASE=/home/ami/zephyrproject/zephyr

build for blue pill
--------------------
west build -p always -b stm32f103_mini

or

west build -p always -b stm32_min_dev_blue


flash
-----
st-flash write build/zephyr/zephyr.bin 0x08000000

or alias flash_bluepill

