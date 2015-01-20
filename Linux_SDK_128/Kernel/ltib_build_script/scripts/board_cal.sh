#! bin/bash

#mount -t jffs2 /dev/mtdblock2 /mnt

insmod /ko_ti/compat.ko
insmod /ko_ti/cfg80211.ko
insmod /ko_ti/mac80211.ko
insmod /ko_ti/wl12xx.ko

calibrator plt autocalibrate wlan0 /ko_ti/wl12xx_sdio.ko /lib/firmware/ini/TQS_D_1.7.ini /lib/firmware/ti-connectivity/wl1271-nvs.bin 00:00:00:00:00:00

cp /lib/firmware/ti-connectivity/wl1271-nvs.bin /mnt/

cp /etc/hostapd* /mnt/
