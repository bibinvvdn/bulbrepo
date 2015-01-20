#! /bin/bash

start()
{
ifconfig wlan0 down
sleep 3
a=$(iwlist wlan1 scan|grep "Kochi2"|cut -d"-" -f2|head -c 6)
d="Kochi2"
if [ "$a" == "$d" ]; then
ifconfig wlan1 up
else
sleep 4
start
fi
}

while true
do
c=$(iw dev wlan1 link |grep "Connected"| cut -d" " -f1)
s="Connected"
if [ "$c" == "$s" ]; then
echo "Connected"
sleep 5
while true
do
j=$(iw dev wlan1 link |grep "Connected"| cut -d" " -f1)
if [ "$j" != "$s" ]; then
echo "disconnected"
start
break
fi
done
fi
if [ "$c" != "$s" ]; then
echo "status is not Connected"
sleep 2
fi
done

