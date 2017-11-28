### Raspberry pi 2 setup guide


#### how to setup correct datetime on rpi2.

rpi does not have actual hardware clock. so enable ntp.
`sudo timedatectl set-ntp yes`  

#### how to setup enlightenment on arch linux@rpi3
/boot/config.txt settings.  
gpu_mem=256  
dtparam=audio=on  
dtparam=i2c_arm=on  
dtparam=spi=on   
dtoverlay=vc4-kms-v3d  
dtparam=i2c1=on  
dtparam=i2c1_baudrate=50000  
disable_overscan=1  
hdmi_drive=2  
framebuffer_depth=32  
framebuffer_ignore_alpha=1  
config_hdmi_boost=2  
hdmi_group=1  
hdmi_mode=16  
avoid_warnings=2  
arm_freq=1200  

wayland compilation  
Efl build options  

         --enable-elput --enable-ecore-buffer \
         --enable-xinput22 --enable-fb --disable-tslib \
         --enable-harfbuzz --enable-wayland --enable-drm \
         --enable-systemd --enable-ibus --with-tests=regular \
         --enable-i-really-know-what-i-am-doing-and-that-this-will-probably-break-things-and-i-will-fix-them-myself-and-send-patches-abb

Enligthtenment build options  
mkdir build
cd build
meson -Dwayland=true ../
ninja
ninja install

