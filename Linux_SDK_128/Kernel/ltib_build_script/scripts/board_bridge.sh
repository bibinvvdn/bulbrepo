#!/bin/bash


###Packet Forwarding #####

echo 1 > /proc/sys/net/ipv4/ip_forward
iptables -A FORWARD -i wlan0 -o wlan1 -j ACCEPT
iptables -A FORWARD -i wlan1 -o wlan0 -m state --state ESTABLISHED,RELATED -j ACCEPT

#iptables -t nat -A POSTROUTING -o wlan1 -j SNAT --to 192.168.4.10  # uncomment it for static ip and change the ip addr according to wlan1
iptables -t nat -A POSTROUTING -o wlan1 -j MASQUERADE     # Uncomment it for DHCP

