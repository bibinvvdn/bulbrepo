#!/bin/bash
j=`/bin/cat /mnt/WFPS_Config.cfg | grep DEVICE_IP | cut -d~ -f2`
echo $j
ifconfig br0 $j

