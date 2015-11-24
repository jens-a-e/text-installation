#!/bin/ash
# Not implemented yet.
# aireplay line from docs:
# aireplay-ng -0 1 -a 00:14:6C:7E:40:80 -c 00:0F:B5:34:30:30 ath0
# Where:
# -0 means deauthentication
# 1 is the number of deauths to send (you can send multiple if you wish); 0 means send them continuously
# -a 00:14:6C:7E:40:80 is the MAC address of the access point
# -c 00:0F:B5:34:30:30 is the MAC address of the client to deauthenticate; if this is omitted then all clients are deauthenticated
# ath0 is the interface name
