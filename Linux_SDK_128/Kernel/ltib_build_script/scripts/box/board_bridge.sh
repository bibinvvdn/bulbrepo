#!/bin/bash

###Packet Forwarding #####

echo 1 > /proc/sys/net/ipv4/ip_forward
iptables -A FORWARD -i wlan0 -o wlan1 -j ACCEPT
iptables -A FORWARD -i wlan1 -o wlan0 -m state --state ESTABLISHED,RELATED -j ACCEPT

#iptables -t nat -A POSTROUTING -o wlan1 -j SNAT --to 192.168.4.10  # uncomment it for static ip and change the ip addr according to
iptables -t nat -A POSTROUTING -o wlan1 -j MASQUERADE     # Uncomment it for DHCP

ifconfig wlan0 mtu 1532
batctl if add wlan0
ifconfig bat0 up
brctl addbr br0
brctl addif br0 wlan2
brctl addif br0 bat0
ifconfig br0 up
/bin/sh /box/Device_ip.sh   #configuring the IP of board from database file
sleep 1
cp /etc/udhcpd_br0.conf /etc/udhcpd.conf
/bin/sh /start_end.sh   #configuring the start and end IP address for udhcpd server and dns from database
sleep 1
mkdir /var/lib/udhcpd
touch /var/lib/udhcpd/udhcpd.leases

if [ -e /mnt/udhcpd.conf ]; then
/sbin/udhcpd /mnt/udhcpd.conf
else
cp /etc/udhcpd.conf /mnt/
/sbin/udhcpd /mnt/udhcpd.conf
fi
telnetd -l /bin/sh
cfg_mgr &
sh /board_avahi.sh
sh /box/clientmon.sh&

