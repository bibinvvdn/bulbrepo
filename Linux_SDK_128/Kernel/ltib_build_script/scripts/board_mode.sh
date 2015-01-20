#! bin/bash

#mount -t jffs2 /dev/mtdblock2 /mnt
/bin/chmod 777 /dev/mtd1

cp /mnt/wl1271-nvs.bin /lib/firmware/ti-connectivity

insmod /ko_ti/compat.ko
insmod /ko_ti/cfg80211.ko
insmod /ko_ti/mac80211.ko
insmod /ko_ti/wl12xx.ko
insmod /ko_ti/wl12xx_sdio.ko

ifconfig wlan0 up
/bin/sh /Device_ip.sh	#configuring the IP of board from database file
sleep 1
/bin/sh /start_end.sh	#configuring the start and end IP address for udhcpd server and dns from database
sleep 1
#ifconfig wlan0 192.168.100.1 up
mkdir /var/lib/udhcpd
touch /var/lib/udhcpd/udhcpd.leases
#/sbin/udhcpd

if [ -e /mnt/udhcpd.conf ]; then
/sbin/udhcpd /mnt/udhcpd.conf
else
cp /etc/udhcpd.conf /mnt/
/sbin/udhcpd /mnt/udhcpd.conf
fi

ifconfig wlan0 | grep -rin "HWaddr" | tail -c 20 | head -c 17 > mac.txt
ssid=`/bin/cat /mac.txt`
/bin/sed -e "s/test/$ssid/" /mnt/hostapd_1.conf > /mnt/hostapd.conf	#modified


#hostapd /etc/hostapd.conf -B

/usr/local/bin/hostapd /mnt/hostapd.conf -B		#modified

iw dev wlan0 interface add wlan1 type station
ifconfig wlan1 up

telnetd -l /bin/sh

cfg_mgr &
echo 0 > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness
sh board_avahi.sh
