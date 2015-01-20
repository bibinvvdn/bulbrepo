#! bin/bash

mount -t jffs2 /dev/mtdblock2 /mnt
/bin/chmod 777 /dev/mtd1

cp /mnt/wl1271-nvs.bin /lib/firmware/ti-connectivity

insmod /ko_ti/compat.ko
insmod /ko_ti/cfg80211.ko
insmod /ko_ti/mac80211.ko
insmod /ko_ti/wl12xx.ko
insmod /ko_ti/wl12xx_sdio.ko

/bin/sh /box/interface_ssid.sh

ifconfig wlan0 down
j=`/bin/cat /ssid_wlan0.txt`
echo $j
ifconfig wlan0 hw ether $j up
sleep 3

iw dev wlan0 interface add wlan1 type station
j=`/bin/cat /ssid_wlan1.txt`
echo $j
ifconfig wlan1 hw ether $j up
sleep 3

iw dev wlan0 interface add wlan2 type station
j=`/bin/cat /ssid_wlan2.txt`
echo $j
#ifconfig wlan2 hw ether $j up
ifconfig wlan2 hw ether $j
sleep 3

#cfg_mgr &
echo 0 > /sys/devices/platform/pwm-backlight.3/backlight/pwm-backlight.3/brightness

