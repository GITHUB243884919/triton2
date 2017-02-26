#!/bin/sh  
user=$USER

target='globalserver' 
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -w $user | grep -v grep | awk '{print $2}'` 
echo "pid $pids" 
for pid  in $pids 
do 
   kill -9 $pid 
done 

target='tcpsvrd'
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -w $user | grep -v grep | awk '{print $2}'`
echo "pid $pids" 
for pid  in $pids
do
   kill -9 $pid
done

sleep 1

echo "Delete share memory" 
shmid=`ipcs -m | grep -w $user | awk '$6==0{printf " -m  " $2  " "}'` 

echo "$shmid" 
ipcrm $shmid 
