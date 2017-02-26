#!/bin/sh  

user=$USER

./stop.sh

export TRITON_HOME=~/triton2/

#------------------------------------------------------------
# GlobalServer
target='globalserver'
echo $target
cd $TRITON_HOME/server/globalserver/bin
sleep 1

echo Start globalserver tcpsvr
./tcpsvrd
sleep 1

echo Start globalserver
./$target -D

ipcs -m | grep $user
ps aux | grep $user

