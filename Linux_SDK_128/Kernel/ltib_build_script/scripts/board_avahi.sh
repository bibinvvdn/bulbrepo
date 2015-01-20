#!/bin/bash

echo avahi:x:84: >> /etc/group
echo avahi:x:84:84:avahi daemon:/usr/sbin/avahi-daemon:/bin/false >> /etc/passwd

cd /mnt

if [ -d "$PWD/build" ]; then
echo directory exist and copying library
else
echo untar the directory
tar -xvf build.tar.bz 
fi

cp -rf build/ /dev/shm

cd /dev/shm

cp /dev/shm/build/build_avahi/sbin/* /usr/sbin
cp /dev/shm/build/build_avahi/lib/* /usr/lib -rf
cp /dev/shm/build/build_expat/lib/* /usr/lib -rf
cp /dev/shm/build/build_libdaemon/lib/* /usr/lib -rf
cp /dev/shm/build/build_dbus/lib/* /usr/lib -rf
cp /dev/shm/build/build_dbus/bin/dbus-launch /usr/sbin
cp /dev/shm/build/build_dbus/bin/dbus-daemon /usr/sbin

dbus-launch

dbus-daemon --config-file=/dev/shm/build/build_dbus/etc/dbus-1/system.conf

avahi-daemon -D
avahi-dnsconfd -D

cd /

