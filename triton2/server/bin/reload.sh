#!/bin/sh  

user='common'

target='sceneserver' 
echo "reload $target" 
pids=`ps -ef | grep $target | grep $user | grep -v grep | awk '{print $2}'` 
echo "pid $pids" 
for pid  in $pids 
do 
        kill -USR1 $pid 
done 



target='gateserver'
echo "reload $target" 
pids=`ps -ef | grep $target | grep $user | grep -v grep | awk '{print $2}'`
echo "pid $pids" 
for pid  in $pids
do
        kill -USR1 $pid
done

