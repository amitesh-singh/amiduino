```
export CROSS_COMPILE=arm-linux-gnueabi-
make qemu_arm_defconfig
make cscope # This is optional
make
```

For AArch64, replace CROSS_COMPILE with correct cross compiler and then:

```
export CROSS_COMPILE=aarch64-linux-gnu-
make qemu_arm64_defconfig
make
```
To run aarch64:

```
qemu-system-aarch64 -machine virt -cpu cortex-a57 -nographic  -bios u-boot.bin
```
