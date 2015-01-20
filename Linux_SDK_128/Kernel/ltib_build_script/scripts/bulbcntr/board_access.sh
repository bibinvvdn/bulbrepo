#! /bin/bash                                                      
ifconfig wlan0 down                                                
#killall hostapd                                                   
i=`iwconfig wlan1 | grep -rin "Frequency" | tail -c 46 | head -c 5`
echo $i                          
case $i in                       
2.412)                           
echo "channel=1" > /channel.txt;;
2.417)                           
echo "channel=2" > /channel.txt;;
2.422)                           
echo "channel=3" > /channel.txt;; 
2.427)                            
echo "channel=4" > /channel.txt;; 
2.432)                            
echo "channel=5" > /channel.txt;; 
2.437)                            
echo "channel=6" > /channel.txt;;
2.442)                           
echo "channel=7" > /channel.txt;;                                                                        
2.447)                                                                                                   
echo "channel=8" > /channel.txt;;                                                                        
2.452)                                                                                                   
echo "channel=9" > /channel.txt;;                                                                    
2.457)                                             
echo "channel=10" > /channel.txt;;                 
2.462)                                             
echo "channel=11" > /channel.txt;;                 
*)                                                       
#echo "No channel selected";;                                                                            
echo "channel=1" > /channel.txt;;                                                                        
esac                                                                                                     
ssid=meshtest                                                                                            
/bin/sed -e "s/test/$ssid/" /etc/hostapd_AP1_temp1.conf > /mnt/hostapd_AP1_temp.conf            #modified

/bin/sed 7d /mnt/hostapd_AP1_temp.conf > /tmp1.conf                                                      
/bin/cp /tmp1.conf  /mnt/hostapd_AP1_temp.conf     
/bin/cat /channel.txt >> /mnt/hostapd_AP1_temp.conf
/bin/cp /mnt/hostapd_AP1_temp.conf  /mnt/hostapd_AP1.conf
ifconfig wlan0 up                                                                                        
#ifconfig wlan2 up                                                                                        
/usr/local/bin/hostapd /mnt/hostapd_AP1.conf -B                 #modified                                

