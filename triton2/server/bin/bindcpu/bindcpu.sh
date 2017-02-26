#!/bin/sh
user=xyjol

# bind each program to cpu0 - cpu7
# cpu0,cpu3 is for system timer use ( default is 0,3, keep it )
# cpu0,cpu1 is for ioc0 ( default 0-7 )
# cpu0,cpu1 is for eth0,eth1 ( default 0 - 7 )


ioc0s=`grep ioc0 /proc/interrupts | awk '{print $1}' | cut -d ":" -f 1`
for ioc0 in $ioc0s
do
	echo "00000003" > /proc/irq/$ioc0/smp_affinity
	echo "ioc's affinity is: " 
	cat /proc/irq/$ioc0/smp_affinity
done

eth0s=`grep eth0 /proc/interrupts | awk '{print $1}' | cut -d ":" -f 1`
for eth0 in $eth0s
do 
	echo "00000003" > /proc/irq/$eth0/smp_affinity
	echo "eth0's affinity is: " 
	cat /proc/irq/$eth0/smp_affinity
done


eth1s=`grep eth1 /proc/interrupts | awk '{print $1}' | cut -d ":" -f 1`
for eth1 in $eth1s
do 
	echo "00000003" > /proc/irq/$eth1/smp_affinity
	echo "eth1's affinity is: " 
	cat /proc/irq/$eth1/smp_affinity
done



#SERVER1 LogDB + SlaveDB + GS5 + GS6
SERVER1=0

#SERVER2 MasterDB + Login + Gate + Proxy + Apex + FB1+ FB2
SERVER2=0

#SERVER3 GS1 + GS2 +  GS3 + GS4 
SERVER3=0



target='proxyd'
echo "--- bind " $target " --- "
pids=`ps -efL | grep $target | grep -w $user | grep -v grep | awk '{print $4}'`
for pid  in $pids
do

       taskset -pc 2,4,5,6,7  $pid
done



target='gateserver'
echo "--- bind " $target " --- "
pids=`ps -efL | grep $target | grep -w $user | grep -v grep | awk '{print $4}'`
for pid  in $pids
do
       taskset -pc 2,4,5,6,7  $pid
done



target='loginserver'
echo "--- bind " $target " --- "
pids=`ps -efL | grep $target | grep -w $user | grep -v grep | awk '{print $4}'`
for pid  in $pids
do
       taskset -pc 2,4,5,6,7  $pid
done



target='tcpsvrd'
echo "--- bind " $target " --- "
pids=`ps -efL | grep $target | grep -w $user | grep -v grep | awk '{print $4}'`
for pid  in $pids
do
       taskset -pc 2,4,5,6,7  $pid
done



target='dbsvrd'
echo "--- bind " $target " --- "
pids=`ps -efL | grep $target | grep -w $user | grep -v grep | awk '{print $4}'`
for pid  in $pids
do
       taskset -pc 2,4,5,6,7  $pid
done


target='logsvrd'
echo "--- bind " $target " --- "
pids=`ps -efL | grep $target | grep -w $user | grep -v grep | awk '{print $4}'`
for pid  in $pids
do
       taskset -pc 2,4,5,6,7  $pid
done

#################################
# gameserver 1-4 keep default 
#################################

target='sceneserver -D -dun1'
echo "--- bind " $target " --- "
pids=`ps -efL | grep "sceneserver -D -dun1" | grep -w $user | grep -v grep | awk '{print $4}'`
for pid  in $pids
do
       taskset -pc 2,4,5,6,7  $pid
done

target='sceneserver -D -dun2'
echo "--- bind " $target " --- "
pids=`ps -efL | grep "sceneserver -D -dun2" | grep -w $user | grep -v grep | awk '{print $4}'`
for pid  in $pids
do
       taskset -pc 2,4,5,6,7  $pid
done

target='sceneserver -D -5'
echo "--- bind " $target " --- "
pids=`ps -efL | grep "sceneserver -D -5" | grep -w $user | grep -v grep | awk '{print $4}'`
for pid  in $pids
do
       taskset -pc 2,4,5,6,7  $pid
done

target='sceneserver -D -6'
echo "--- bind " $target " --- "
pids=`ps -efL | grep "sceneserver -D -6" | grep -w $user | grep -v grep | awk '{print $4}'`
for pid  in $pids
do
       taskset -pc 2,4,5,6,7  $pid
done

