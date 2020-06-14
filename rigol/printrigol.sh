#!/bin/bash
# alias printrigol='pipenv run ~/rigol/printrigol.sh'

cd ~/rigol/rigol-grab/
pipenv run  python rigol_grab.py --auto_view --port 169.254.239.114 &
sleep 6
exit



