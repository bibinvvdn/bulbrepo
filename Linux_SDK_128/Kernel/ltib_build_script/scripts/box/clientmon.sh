flag=0
sleep 3
/sbin/udhcpc -n -t 7 -i wlan1
while [ 1 ];
do
j=`iw dev wlan1 link`
        while [ "$j" == "Not connected." ];
        do
        j=`iw dev wlan1 link`
        flag=1
        sleep 2
        done
if [ "$flag" == "1" ]; then
echo Sending DHCP Request
/sbin/udhcpc -n -t 7 -i wlan1
flag=0
else
sleep 3
fi
done

