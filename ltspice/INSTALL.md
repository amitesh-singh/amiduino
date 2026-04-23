# How to install LTSpice on Ubuntu

ubuntu packages get b0rked, and it's mainly due to the missing noble-updates repo

Adding the below repo seems to resolve this problem.


```
sudo add-apt-repository -y 'deb http://archive.ubuntu.com/ubuntu noble-updates main'

sudo apt update
sudo apt upgrade

sudo dpkg --add-architecture i386
sudo apt update
sudo apt autoremove -y
sudo apt install -y wine64 wine32

wget https://ltspice.analog.com/software/LTspice64.exe


wine ./LTspice63.py

```
