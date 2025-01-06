# how to setup stm32cube ide on arch linux

- install https://aur.archlinux.org/packages/ncurses5-compat-libs to resolve arm gdb issue related to libncurses

# saving stm32cube ide projects in github

- add Debug/ in .gitignore

## enable SWDIO/SWCLK serial debug in *.ioc

- make sure SYS->Debug enable the Serial wire PA13 (SWDIO) and PA14 (SWCLK);
- this is important or else stm32f103 won't be programmable. BOOT0 has to be shifted to 1
  to get it programmed again

