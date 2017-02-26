#!/bin/sh  
user=$USER

target='sceneserver' 
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -w $user | grep -v grep | awk '{print $2}'` 
echo "pid $pids" 
for pid  in $pids 
do 
        kill -9 $pid 
	sleep 2
done 


target='tcpsvrd'
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -w $user | grep -v grep | awk '{print $2}'`
echo "pid $pids" 
for pid  in $pids
do
        kill -USR2 $pid
done


target='loginserver'
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -w  $user | grep -v grep | awk '{print $2}'`
echo "pid $pids" 
for pid  in $pids
do
        kill -USR2 $pid
done


target='dbsvrd'
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -w  $user | grep -v grep | awk '{print $2}'`
echo "pid $pids" 
for pid  in $pids
do
        kill -USR2 $pid
done


target='gateserver'
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -w $user | grep -v grep | awk '{print $2}'`
echo "pid $pids" 
for pid  in $pids
do
        kill -USR2 $pid
done

target='proxyd'
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -w $user | grep -v grep | awk '{print $2}'`
echo "pid $pids" 
for pid  in $pids
do
        kill -USR2 $pid
done


target='AHNetServer'
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -w $user | grep -v grep | awk '{print $2}'`
echo "pid $pids" 
for pid  in $pids
do
        kill -9 $pid
done


target='logsvrd'
echo "Stop $target" 
pids=`ps -ef | grep $target | grep -w $user | grep -v grep | awk '{print $2}'`
echo "pid $pids" 
for pid  in $pids
do
        kill -USR2 $pid
done

sleep 1
	echo "Delete share memory" 
	shmid=`ipcs -m | grep -w $user | awk '$6==0{printf " -m  " $2  " "}'` 
	echo "$shmid" 
	ipcrm $shmid 



