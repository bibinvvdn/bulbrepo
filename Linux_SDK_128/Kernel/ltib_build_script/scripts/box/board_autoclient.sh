#! /bin/bash                                                                           
cd /mnt                                                                                   
if [ -e wpa_supplicant.conf ]; then     #if file exist then connect to secured network#   
                                                                                          
/usr/local/bin/wpa_supplicant -Dwext -iwlan1 -c /mnt/wpa_supplicant.conf -B               
sleep 2                                                                                   
j=`iw dev wlan1 link`                    
if [ "$j" == "Not connected." ]; then          
sleep 6                                        
else                                           
/bin/echo "Client connected to secured network"
fi                                                                                        
#/sbin/dhclient wlan1                                                                     
/sbin/udhcpc -n -t 7 -i wlan1                                                             
#loop start --check whether client is connect/not,& if not connected then kill supplicant#
j=`iw dev wlan1 link`                                                               
if [ "$j" == "Not connected." ]; then                                               
killall wpa_supplicant                   
echo "Try again, Not connected to client"
sleep 3                              
/sbin/ifconfig wlan1 up                     
else                                        
echo "Associated"                                                                   
fi                                                                                  
#end of loop                                                                              
elif [ -e Client_open.sh ]; then        #if file exist then connect to open network#      

/bin/sh /mnt/Client_open.sh                                                               
sleep 2                                                                             
j=`iw dev wlan1 link`                                                               
if [ "$j" == "Not connected." ]; then    
sleep 6                                  
else                                 
/bin/echo "Client connected to open network"
fi                                          
#/sbin/dhclient wlan1                                                               
/sbin/udhcpc -n -t 7 -i wlan1                                                       
#loop start --check whether client is connect/not,& if not connected then kill supplicant#
j=`iw dev wlan1 link`                                                                     
if [ "$j" == "Not connected." ]; then                                                     
killall wpa_supplicant                                                                    
echo "Try again, Not connected to client"                                                 
sleep 3                                  
/sbin/ifconfig wlan1 up                     
else                                        
echo "Associated"                           
fi                                          
#end of loop                                                                              
fi                                                                                        
cd /                                                                                      
/bin/sh /box/board_access.sh    #channel change script according to client#               
/bin/sh /box/board_bridge.sh        #creating bridge between two interfaces
touch /mnt/custom_script.sh
/bin/sh /mnt/custom_script.sh
