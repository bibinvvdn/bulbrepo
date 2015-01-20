#! /bin/bash                                                                           
flag2=0
#scan(){
#iwlist wlan1 scan | grep Address | cut -d - -f2 | sed 's/Address://g' | sed "s/^[ \t]*//" > /macaddr.txt
#cat /macaddr.txt | grep D4:94 > /bulbaddr.txt
#bssid=`/bin/cat /bulbaddr.txt`
#if [ "$bssid" ]; then
#    flag2=0
#    iwconfig wlan1 essid meshtest ap $bssid
#else
#    echo "Mesh AP not found"
#    flag2=`expr $flag2 + 1`
#    if [ $flag2 -le 10 ]; then
#        sleep 3
#        scan
#        fi
#fi
#}
#scan
#sleep 2                                                                             
#j=`iw dev wlan1 link`                                                               
#if [ "$j" == "Not connected." ]; then    
#sleep 6                                  
#else                                 
#/bin/echo "Client connected to open network"
#fi                                          
#end of loop
touch /mnt/bulbaddr.txt
bssid=`/bin/cat /mnt/bulbaddr.txt`
connect(){
        iwconfig wlan1 essid meshtest ap $bssid
        sleep 5
        j=`iw dev wlan1 link`
        if [ "$j" == "Not connected." ]; then
        flag2=`expr $flag2 + 1`
            if [ $flag2 -le 10 ]; then
            /bin/echo "Not Connected. Trying again"
            connect
            else
                /bin/echo "Not Connected. Mesh AP not found"                
            fi
        else
        /bin/echo "Client connected to mesh network"
    fi
    }
sleep 120
connect
cd /                                                                                      
/bin/sh /bulbcntr/board_access.sh    #channel change script according to client#               
/bin/sh /bulbcntr/board_bridge.sh        #creating bridge between two interfaces
touch /mnt/custom_script.sh
/bin/sh /mnt/custom_script.sh
