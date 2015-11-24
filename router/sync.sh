#!/bin/bash

echo "Syncing files..."
echo $$

rsync -urltv  --exclude="./root/textinstallation/db" --delete -e ssh ./root/textinstallation/ root@192.168.123.1:/root/textinstallation
# check for errors in main lua script
ssh root@192.168.123.1 /root/textinstallation/main.lua
[ "$?" == "0" ] && {
  # ssh root@192.168.123.1 /root/textinstallation/scripts/setup.sh
  echo "Restarting remote..."
  ssh root@192.168.123.1 /etc/init.d/textinstallation restart
}

echo "Syncing Done!"