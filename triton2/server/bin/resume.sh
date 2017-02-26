#!/bin/sh  
target='sceneserver'
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -v grep | awk '{print $2}'`
echo "pid $pids" 
for pid  in $pids
do
        kill -USR2 $pid
done
sleep 1
echo "Resume $target" 
./$target &
