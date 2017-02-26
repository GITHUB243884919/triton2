#!/bin/sh

user=common
flag=1

while((flag==1))
do

pid=`ps aux | grep sceneserver | grep $user | grep -v grep`

if [ "$pid" = "" ];
then
echo "sceneserver gone, restart it now"
./restart.sh
fi


sleep 10

done
~    
