#!/bin/bash

j=`/bin/cat /mnt/WFPS_Config.cfg | grep DEVICE_IP | cut -d~ -f2`
echo $j
echo $j > /IP_conf1.txt
/bin/sed 's/..$//' /IP_conf1.txt > /IP_conf.txt


k=`/bin/cat /IP_conf.txt`

/bin/sed 5d /etc/udhcpd.conf > /etc/udhcpd_tmp1.conf

/bin/sed '5 i\start          '"$k"'.2' /etc/udhcpd_tmp1.conf > /etc/udhcpd_tmp2.conf

/bin/sed 6d /etc/udhcpd_tmp2.conf > /etc/udhcpd_tmp1.conf

/bin/sed '6 i\end            '"$k"'.200' /etc/udhcpd_tmp1.conf > /etc/udhcpd_tmp.conf

a=`/bin/cat /etc/udhcpd.conf | awk 'NR==86 {print $3}'`
#echo $a
b=`/bin/cat /mnt/WFPS_Config.cfg | grep PRIMARY_DNS | cut -d~ -f2`
#echo $b
/bin/sed -e "s/$a/$b/" /etc/udhcpd_tmp.conf > /etc/udhcpd_tmp1.conf


c=`/bin/cat /etc/udhcpd.conf | awk 'NR==88 {print $3}'`
echo $c
/bin/sed -e "s/$c/$j/" /etc/udhcpd_tmp1.conf > /mnt/udhcpd.conf

