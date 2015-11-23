#!/bin/bash

echo "Syncing files..."
echo $$

rsync -urltv --delete -e ssh ./root/textinstallation/ root@192.168.123.1:/root/textinstallation
# check for errors in main lua script
ssh root@192.168.123.1 /root/textinstallation/main.lua
[ "$?" == "0" ] && {
  ssh root@192.168.123.1 /root/textinstallation/scripts/setup.sh
  ssh root@192.168.123.1 /etc/init.d/textinstallation restart
}

echo "Syncing Done!"