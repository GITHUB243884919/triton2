#!/bin/sh  

user='common'

./stop.sh

 TRITON_HOME=~/triton2
target='proxyd'
echo $target
cd $TRITON_HOME/server/proxyserver/bin
./$target 
sleep 2

target='AHNetServer'
echo $target
cd $TRITON_HOME/server/ApexItemServer
#./$target
sleep 2

target='logsvrd -D'
echo $target
cd $TRITON_HOME/server/logserver/bin
#./tcpsvrd
sleep  2

./$target 

sleep 2

#export TRITON_HOME=/home/lihui/work/triton2/server/loginserver/
target='loginserver -D'
echo $target
cd $TRITON_HOME/server/loginserver/bin
./tcpsvrd
sleep  2

./$target 

sleep 2

#export TRITON_HOME=/home/lihui/work/triton2/server/dbserver/
target='dbsvrd -D'
echo $target
cd $TRITON_HOME/server/dbserver/bin
./$target

sleep 2

cp $TRITON_HOME/server/gameserver/config/data/template/* $TRITON_HOME/server/gateserver/config/data/template/ -rf
#export TRITON_HOME=/home/lihui/work/triton2/server/gateserver/

sleep 5
target='gateserver -D'
echo $target
cd $TRITON_HOME/server/gateserver/bin
./tcpsvrd
sleep 5

./$target 1>$TRITON_HOME/server/gateserver/bin/stdout.txt 2>$TRITON_HOME/server/gateserver/bin/stderror.txt
sleep 5


#export TRITON_HOME=/home/lihui/work/triton2/server/gameserver/
target='sceneserver -D -1' 
echo $target 
cd $TRITON_HOME/server/gameserver/bin
./tcpsvrd
sleep 2

./$target
sleep 5

#target='sceneserver -D -2'
#cp $TRITON_HOME/server/gameserver/bin/sceneserver  $TRITON_HOME/server/gameserver2/bin/ -fr
#cp $TRITON_HOME/server/gameserver/bin/ApexProxy.so  $TRITON_HOME/server/gameserver2/bin/ -fr
#cp $TRITON_HOME/server/gameserver/bin/AHThunkConfig.xml  $TRITON_HOME/server/gameserver2/bin/ -fr
#rm $TRITON_HOME/server/gameserver2/config/data -fr
#cp $TRITON_HOME/server/gameserver/config/data $TRITON_HOME/server/gameserver2/config -fr


#echo $target 
#cd $TRITON_HOME/server/gameserver2/bin
#./$target 1>$TRITON_HOME/server/gameserver2/bin/stdout.txt 2>$TRITON_HOME/server/gameserver2/bin/stderror.txt

#./tcpsvrd


cp $TRITON_HOME/server/gameserver/bin/ApexProxy.so  $TRITON_HOME/server/fbgameserver/bin/ -fr
cp $TRITON_HOME/server/gameserver/bin/AHThunkConfig.xml  $TRITON_HOME/server/fbgameserver/bin/ -fr
rm $TRITON_HOME/server/fbgameserver/config/data -fr
cp $TRITON_HOME/server/gameserver/config/data $TRITON_HOME/server/fbgameserver/config -fr

target='sceneserver -D -fb'
echo $target 
cd $TRITON_HOME/server/fbgameserver/bin
./tcpsvrd
sleep 5

./$target 1>$TRITON_HOME/server/fbgameserver/bin/stdout.txt 2>$TRITON_HOME/server/fbgameserver/bin/stderror.txt
sleep 5


cp $TRITON_HOME/server/fbgameserver/bin/sceneserver  $TRITON_HOME/server/fbgameserver2/bin/ -fr
cp $TRITON_HOME/server/fbgameserver/bin/ApexProxy.so  $TRITON_HOME/server/fbgameserver2/bin/ -fr
cp $TRITON_HOME/server/fbgameserver/bin/AHThunkConfig.xml  $TRITON_HOME/server/fbgameserver2/bin/ -fr
rm $TRITON_HOME/server/fbgameserver2/config/data -fr
cp $TRITON_HOME/server/gameserver/config/data $TRITON_HOME/server/fbgameserver2/config -fr

target='sceneserver -D -fb2'
echo $target 
cd $TRITON_HOME/server/fbgameserver2/bin
./tcpsvrd
sleep 2

./$target 1>$TRITON_HOME/server/fbgameserver2/bin/stdout.txt 2>$TRITON_HOME/server/fbgameserver2/bin/stderror.txt
sleep 5


target='sceneserver -D -2'
cp $TRITON_HOME/server/gameserver/bin/sceneserver  $TRITON_HOME/server/gameserver2/bin/ -fr
cp $TRITON_HOME/server/gameserver/bin/ApexProxy.so  $TRITON_HOME/server/gameserver2/bin/ -fr
cp $TRITON_HOME/server/gameserver/bin/AHThunkConfig.xml  $TRITON_HOME/server/gameserver2/bin/ -fr
rm $TRITON_HOME/server/gameserver2/config/data -fr
cp $TRITON_HOME/server/gameserver/config/data $TRITON_HOME/server/gameserver2/config -fr
cd $TRITON_HOME/server/gameserver2/config/data/scene_config
rm sceneconfig.xml
svn up

echo $target 
cd $TRITON_HOME/server/gameserver2/bin
./tcpsvrd
sleep 5

./$target
sleep 5


target='sceneserver -D -5'
cp $TRITON_HOME/server/gameserver/bin/sceneserver  $TRITON_HOME/server/gameserver5/bin/ -fr
cp $TRITON_HOME/server/gameserver/bin/ApexProxy.so  $TRITON_HOME/server/gameserver5/bin/ -fr
cp $TRITON_HOME/server/gameserver/bin/AHThunkConfig.xml  $TRITON_HOME/server/gameserver5/bin/ -fr
rm $TRITON_HOME/server/gameserver5/config/data -fr
cp $TRITON_HOME/server/gameserver/config/data $TRITON_HOME/server/gameserver5/config -fr

echo $target 
cd $TRITON_HOME/server/gameserver5/bin
./tcpsvrd
sleep 5


./$target
sleep 5

#./$target 1>$TRITON_HOME/server/fbgameserver2/bin/stdout.txt 2>$TRITON_HOME/server/fbgameserver2/bin/stderror.txt
#sleep 2

ipcs -m | grep $user

sleep 2

ps aux | grep $user



