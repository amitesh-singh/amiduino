# klipper setup on Arch linux on rpi2
## usb permission fix



```
sudo usermod -aG uucp alarm

```

## wifi setup
I use old usb dongle which is nicely supported in arch linux 

```
sudo systemctl enable dhcpcd
```

create the `/etc/wpa_supplicant/wpa_supplicant.conf` 
```
wpa_passphrase "ssid" "password" | sudo tee /etc/wpa_supplicant/wpa_supplicant.conf
```

how to make sure wifi inteface name is `wlan0`

- Open the kernel boot configuration file (/boot/cmdline.txt).
- Add `net.ifnames=0` to the end of the line.
```
[alarm@alarmpi printer_data]$ cat /etc/systemd/system/wifi-start.service
[Unit]
Description=start wifi
After=network.target
StartLimitIntervalSec=0
[Service]
Type=simple
Restart=always
RestartSec=1
ExecStart=wpa_supplicant -c /etc/wpa_supplicant/wpa_supplicant.conf -i wlan0

[Install]
WantedBy=multi-user.target
```

```
sudo systemctl enable wifi-start
sudo systemctl start wifi-start
```
```
cat << 'EOF' > /etc/udev/rules.d/10-network.rules
SUBSYSTEM=="net", ACTION=="add", ATTR{address}=="YOUR_MAC_ADDRESS", NAME="wlan0"
EOF
```
## Install the packages beforehand

```
sudo pacman -Syu

sudo pacman -S \
    git \
    base-devel \
    python \
    python-pip \
    python-virtualenv \
    python-wheel \
    gcc \
    pkgconf \
    nginx \
    cmake \
    unzip \
    gcc 
```

`greenlet` package did not compile successfully 

install it as system level
```
sudo pacman -S python-greenlet python-can libsodium raspberrypi-utils
```
raspberrypi-utils is not vencmd, throttle stuff 


## Create the printer data directories

```
mkdir -p ~/printer_data/{config,gcodes,logs,comms,systemd}

```

## Install Klipper

```
cd ~

git clone https://github.com/amitesh-singh/klipper_ender3v3se_config klipper/

# --system-site-packages is required because of failure to install `greenlet` using pip or venv
python -m venv --system-site-packages ~/klippy-env

source ~/klippy-env/bin/activate
pip install -r ~/klipper/scripts/klippy-requirements.txt

```
## Create the klipper systemd service

```
vim /etc/systemd/system/klipper.service

[Unit]
Description=Klipper
After=network.target

[Service]
User=alarm
WorkingDirectory=/home/alarm/klipper
ExecStart=/home/alarm/klippy-env/bin/python \
    /home/alarm/klipper/klippy/klippy.py \
    /home/alarm/printer_data/config/printer.cfg \
    -l /home/alarm/printer_data/logs/klippy.log \
    -a /home/alarm/printer_data/comms/klippy.sock
Restart=always
RestartSec=5

[Install]
WantedBy=multi-user.target

```

```
sudo systemctl daemon-reload

sudo systemctl enable --now klipper

```

## Install Moonraker

```
cd ~
git clone https://github.com/Arksine/moonraker.git
python -m venv --system-site-packages ~/moonraker-env


source ~/moonraker-env/bin/activate

pip install -r ~/moonraker/scripts/moonraker-requirements.txt

```

## Create the moonraker service

```
vim /etc/systemd/system/moonraker.service

[Unit]
Description=Moonraker
After=klipper.service

[Service]
User=alarm
WorkingDirectory=/home/alarm/moonraker
ExecStart=/home/alarm/moonraker-env/bin/python \
    -m moonraker \
    -d /home/alarm/printer_data
Restart=always

[Install]
WantedBy=multi-user.target
```

```
sudo systemctl daemon-reload
sudo systemctl enable --now moonraker
```

## Configure moonraker

```
vim ~/printer_data/config/moonraker.conf

[server]
host: 0.0.0.0
port: 7125

[file_manager]

[authorization]
trusted_clients:
    127.0.0.1
    ::1

```

## Install mainsail


```
cd ~
git clone https://github.com/mainsail-crew/mainsail.git

cd ~/mainsail
npm set progress=false

# The below commands take loads of time
npm install 
npm run build

sudo cp -r dist/* /srv/http/
```

### nginx setup

#### Directly in `nginx.conf`
```
sudo mkdir -p /etc/nginx/conf.d

sudo vim /etc/nginx/nginx.conf
```
Add below entry under `http {}`

Locate the http { ... } section:

Paste the server { ... } block inside the http {} block, not outside it.

```
server {
    listen 80;

    root /srv/http;
    index index.html;

    location / {
        try_files $uri $uri/ /index.html;
    }

    # Moonraker WebSocket
    location /websocket {
        proxy_pass http://127.0.0.1:7125/websocket;

        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";

        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    }

    # Moonraker API
    location ~ ^/(printer|api|access|machine|server) {
        proxy_pass http://127.0.0.1:7125;

        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    }
}
```
```
sudo systemctl enable --now nginx

```
```
sudo nginx -t
sudo systemctl restart nginx
```

#### `/etc/nginx/conf.d`

If nginx.conf includes conf.d/*.conf, create:

```
sudo mkdir -p /etc/nginx/conf.d
sudo nano /etc/nginx/conf.d/mainsail.conf
```
Paste your entire server { ... } block into that file.

## Mainsail cfg

This is required or else we get https://docs.mainsail.xyz/configuration/mainsail-cfg/ errors 
```
cd ~
git clone https://github.com/mainsail-crew/mainsail-config/
cd ~/printer_data/config/
[alarm@alarmpi config]$ ln -s ~/mainsail-config/mainsail.cfg mainsail.cfg
```

## issue from orca

i had to add 

Open the relevant file and look for the server { ... } block. Add or edit this line inside it:
```
client_max_body_size 0
```
![alt text](image-5.png)

```
sudo systemctl reload nginx
```

## how to get orca api/key password

http://192.168.1.28/access/api_key

