#!/bin/ash

uci import dhcp -f /root/textinstallation/etc/config/dhcp
ln -vsf /root/textinstallation/etc/init.sh /etc/init.d/textinstallation
/etc/init.d/textinstallation enable