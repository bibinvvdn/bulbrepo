#!/bin/bash

test=`ifconfig wlan0|grep HWaddr|awk '{print $5}'| tr '[a-z]' '[A-Z]' | cut -d ':' -f1-2`
echo $test
val='D4:94'
if [ $test = $val ]
then
#Genrating the MAC_ID for WLAN1 interface

mac=`ifconfig wlan0|grep HWaddr|awk '{print $5}'| tr '[a-z]' '[A-Z]' | cut -d ':' -f1-5`
#echo $mac
maclast=`ifconfig wlan0|grep HWaddr|awk '{print $5}'| tr '[a-z]' '[A-Z]' | cut -d ':' -f6`
#echo $maclast
echo "Wlan0_Mac address= $mac:$maclast"
echo $mac:$maclast > /ssid_wlan0.txt
decmac=`echo "ibase=16; $maclast"|bc`
#echo $decmac

if [ $decmac -eq '241' ]
then
macinc='00'
else
incout1=`expr $decmac + 1 `
incout2=`expr $incout1 + 1 `
macinc=`echo "obase=16; $incout1" | bc`
if [ $incout1 -eq '241' ]
then
incout2='00'
else
macinc2=`echo "obase=16; $incout2" | bc`
fi
fi
echo "Wlan1_Mac address= $mac:$macinc"
echo $mac:$macinc > /ssid_wlan1.txt

#Genrating the MAC_ID for WLAN2 interface

mac=`ifconfig wlan0|grep HWaddr|awk '{print $5}'| tr '[a-z]' '[A-Z]' | cut -d ':' -f1`
#echo $mac
macmid=`ifconfig wlan0|grep HWaddr|awk '{print $5}'| tr '[a-z]' '[A-Z]' | cut -d ':' -f2`
#echo $macmid
maclast=`ifconfig wlan0|grep HWaddr|awk '{print $5}'| tr '[a-z]' '[A-Z]' | cut -d ':' -f3-5`
#echo $maclast
decmac=`echo "ibase=16; $macmid"|bc`
#echo $decmac

if [ $decmac -eq '241' ]

then
macinc='00'
else
incout=`expr $decmac + 1 `
macinc=`echo "obase=16; $incout" | bc`
fi
echo "Wlan2_Mac address= $mac:$macinc:$maclast:$macinc2"
echo $mac:$macinc:$maclast:$macinc2 > /ssid_wlan2.txt

else
#if mac id starting is differnt from desired then it will changed here

#Genrating the MAC_ID for WLAN1 interface

mac=`ifconfig wlan0|grep HWaddr|awk '{print $5}'| tr '[a-z]' '[A-Z]' | cut -d ':' -f3-5`
#echo $mac
maclast=`ifconfig wlan0|grep HWaddr|awk '{print $5}'| tr '[a-z]' '[A-Z]' | cut -d ':' -f6`
#echo $maclast
echo "Wlan0_Mac address= $val:$mac:$maclast"
echo $val:$mac:$maclast > /ssid_wlan0.txt
decmac=`echo "ibase=16; $maclast"|bc`
#echo $decmac

if [ $decmac -eq '241' ]
then
macinc='00'
else
incout1=`expr $decmac + 1 `
incout2=`expr $incout1 + 1 `
macinc=`echo "obase=16; $incout1" | bc`
if [ $incout1 -eq '241' ]
then
incout2='00'
else
macinc2=`echo "obase=16; $incout2" | bc`
fi
fi
echo "Wlan1_Mac address= $val:$mac:$macinc"
echo $val:$mac:$macinc > /ssid_wlan1.txt

#Genrating the MAC_ID for WLAN2 interface

mac=`echo $val |cut -d ':' -f1`
#echo $mac
macmid=`echo $val| cut -d ':' -f2`
#echo $macmid
maclast=`ifconfig wlan0|grep HWaddr|awk '{print $5}'| tr '[a-z]' '[A-Z]' | cut -d ':' -f3-5`
#echo $maclast
decmac=`echo "ibase=16; $macmid"|bc`
#echo $decmac

if [ $decmac -eq '241' ]

then
macinc='00'
else
incout=`expr $decmac + 1 `
macinc=`echo "obase=16; $incout" | bc`
fi
echo "Wlan2_Mac address= $mac:$macinc:$maclast:$macinc2"
echo $mac:$macinc:$maclast:$macinc2 > /ssid_wlan2.txt

fi
