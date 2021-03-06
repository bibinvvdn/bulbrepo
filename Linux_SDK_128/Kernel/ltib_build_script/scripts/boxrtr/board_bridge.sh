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
#brctl addif br0 wlan2
brctl addif br0 bat0
ifconfig br0 up
/bin/sh /boxrtr/Device_ip.sh   #configuring the IP of board from database file
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
j=`iw dev wlan1 link`
if [ "$j" == "Not connected." ]; then
    ifconfig wlan0 down
    ifconfig wlan1 down
    ifconfig bat0 down
    batctl if del wlan0
    killall -9 udhcpd
    killall -9 hostapd
    ifconfig wlan0 hw ether D4:95:A1:5A:01:F6
    brctl addbr br0
    brctl addif br0 wlan0
    ifconfig br0 192.168.100.1 up
    /usr/local/bin/hostapd /mnt/hostapd_AP1.conf -B
    sleep 2
    cp /etc/udhcpd_br0.conf /mnt/udhcpd.conf
    mkdir /var/lib/udhcpd
    touch /var/lib/udhcpd/udhcpd.leases
    /sbin/udhcpd /mnt/udhcpd.conf
    cfg_mgr &
fi

sh /board_avahi.sh
sh /boxrtr/clientmon.sh&

