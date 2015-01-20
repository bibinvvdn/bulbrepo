#! /bin/bash
#### COMMAND LINE: sh board_client.sh <essid($1)> <Encryption:ON/OFF($2)> <password($3)>
#killall dhclient
killall udhcpc		#It will kill the udhcpc client if running#
killall wpa_supplicant	#It will kill the wpa_supplicant and free the client interface#

	if [ "$2" != off ]; then 	#for connect to the secured network#
rm /mnt/Client_open.sh
killall hostapd
ifconfig wlan0 down
killall avahi-daemon
killall avahi-dnsconfd -D
/usr/local/bin/wpa_passphrase $1 $3 > /mnt/wpa_supplicant.conf
/usr/local/bin/wpa_supplicant -Dwext -iwlan1 -c /mnt/wpa_supplicant.conf -B
sleep 6
j=`iw dev wlan1 link`
if [ "$j" == "Not connected." ]; then
sleep 6
else
/bin/echo "Client connected to secured network"
fi
#dhclient wlan1
/sbin/udhcpc -n -t 8 -i wlan1
sleep 4

#check whether client is connect/not,& if not connected then kill supplicant#
j=`iw dev wlan1 link`
if [ "$j" == "Not connected." ]; then
killall wpa_supplicant						
echo "Try again, Not connected to client"			
sleep 3								
/sbin/ifconfig wlan1 up						
else           							
echo "Associated"						
fi               						
  
/bin/sh /board_access.sh	#channel change script according to client#
/bin/sh /board_bridge.sh	#creating bridge between two interfaces
	else			#for connect to the open network#
killall hostapd
ifconfig wlan0 down
killall avahi-daemon
killall avahi-dnsconfd -D
rm /mnt/wpa_supplicant.conf
/usr/sbin/iwconfig wlan1 essid $1
sleep 6
j=`iw dev wlan1 link`
if [ "$j" == "Not connected." ]; then
sleep 6
else
/bin/echo "Client connected to open network"
fi
#dhclient wlan1
/sbin/udhcpc -n -t 8 -i wlan1
sleep 2
/bin/echo "iwconfig wlan1 essid $1" > /mnt/Client_open.sh
sleep 4

#check whether client is connect/not,& if not connected then kill supplicant#
j=`iw dev wlan1 link`
if [ "$j" == "Not connected." ]; then
killall wpa_supplicant
echo "Try again, Not connected to client"
sleep 3
/sbin/ifconfig wlan1 up
else           
echo "Associated"
fi               
  
/bin/sh /board_access.sh	#channel change script according to client#
/bin/sh /board_bridge.sh	#creating bridge between two interfaces
	fi
