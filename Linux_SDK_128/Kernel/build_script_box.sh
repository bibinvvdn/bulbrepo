#!/bin/bash

#########################################################################
#									#
#	adding the scripts and binary to the rootfs in ltib		#
#									#
#########################################################################

echo 'Adding scripts and binary to ltib'
#add the calibrator binary to the rootfs/bin
sudo cp `pwd`/ltib_build_script/bin/calibrator `pwd`/ltib/rootfs/bin/

#add the iw binary to the rootf/bin
sudo cp `pwd`/ltib_build_script/bin/iw `pwd`/ltib/rootfs/bin/

#add the wpa_spplicant and passphrase to the rootfs/usr/local/bin
sudo cp `pwd`/ltib_build_script/bin/wpa* `pwd`/ltib/rootfs/usr/local/bin/
sudo cp `pwd`/ltib_build_script/bin/bc `pwd`/ltib/rootfs/bin/
sudo cp `pwd`/ltib_build_script/bin/batctl `pwd`/ltib/rootfs/bin/
sudo cp `pwd`/ltib_build_script/bin/brctl `pwd`/ltib/rootfs/bin/
#add the hostapd and dhcpd configuration file to rootfs/etc
sudo cp `pwd`/ltib_build_script/conf/hostapd* `pwd`/ltib/rootfs/etc
#sudo cp `pwd`/ltib_build_script/conf/dhcpd.conf `pwd`/ltib/rootfs/etc
sudo cp `pwd`/ltib_build_script/conf/udhcpd.conf `pwd`/ltib/rootfs/etc
sudo cp `pwd`/ltib_build_script/conf/udhcpd_br0.conf `pwd`/ltib/rootfs/etc
#add the rcS file to rootfs/etc/rc.d
sudo cp `pwd`/ltib_build_script/conf/rcS_box `pwd`/ltib/rootfs/etc/rc.d/rcS

#add the ti firmware to the rootfs/lib
sudo cp `pwd`/ltib_build_script/firmware `pwd`/ltib/rootfs/lib/ -rf

#add the driver module to the rootfs
sudo cp `pwd`/ltib_build_script/ko_ti/ `pwd`/ltib/rootfs/ -rf

#add the initialisation scripts to rootfs
sudo cp -rf `pwd`/ltib_build_script/scripts/* `pwd`/ltib/rootfs/

#add the etc/profile for enable java support
sudo cp `pwd`/ltib_build_script/conf/profile `pwd`/ltib/rootfs/etc/

#add the group and passwd file with changes for the avahi
#sudo cp `pwd`/ltib_build_script/avahi/*	`pwd`/ltib/rootfs/etc/

echo 'Adding done'
