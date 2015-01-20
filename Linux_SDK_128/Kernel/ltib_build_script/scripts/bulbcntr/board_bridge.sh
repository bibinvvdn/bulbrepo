#!/bin/bash

ifconfig wlan0 mtu 1532
ifconfig wlan1 mtu 1532
batctl if add wlan0
batctl if add wlan1
ifconfig bat0 up
brctl addbr br0
#brctl addif br0 wlan2
brctl addif br0 bat0
ifconfig br0 up
sleep 2
/sbin/udhcpc -n -t 7 -i br0
sleep 1
telnetd -l /bin/sh
cfg_mgr &
sleep 1
/sbin/udhcpc -n -t 7 -i br0
sleep 5
j=`iw dev wlan1 link`
if [ "$j" == "Not connected." ]; then
ifconfig wlan0 down
ifconfig wlan1 down
ifconfig bat0 down
ifconfig br0 down
batctl if del wlan0
brctl delif br0 bat0
brctl addif br0 wlan0
killall -9 udhcpd
killall -9 hostapd
ifconfig wlan0 hw ether D4:95:A1:5A:01:F6 up

ifconfig br0 192.168.100.1 up
/usr/local/bin/hostapd /mnt/hostapd_AP1.conf -B
sleep 2
cp /etc/udhcpd_br0.conf /mnt/udhcpd.conf
mkdir /var/lib/udhcpd
touch /var/lib/udhcpd/udhcpd.leases
/sbin/udhcpd /mnt/udhcpd.conf
cfg_mgr &
#ifconfig br0 192.168.100.1 up
#cp /etc/udhcpd_br0.conf /mnt/udhcpd.conf
#mkdir /var/lib/udhcpd
#touch /var/lib/udhcpd/udhcpd.leases
#/sbin/udhcpd /mnt/udhcpd.conf
#else
#/sbin/udhcpc -n -t 7 -i br0
fi
#sh /board_avahi.sh
