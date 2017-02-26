#!/bin/sh
export TRITON_HOME=/home/liyan/work/Triton2/server
export SEED=30400



cd $TRITON_HOME/gateserver/bin
echo "$SEED"071 > cspipefile
echo "$SEED"072 > scpipefile
echo "$SEED"073 > gatepipefile
echo "$SEED"074 > tcppipefile



cd $TRITON_HOME/loginserver/bin
echo "$SEED"081 > cspipefile
echo "$SEED"082 > scpipefile
echo "$SEED"083 > loginpipefile
echo "$SEED"084 > tcppipefile

cd $TRITON_HOME/gameserver/bin
echo "$SEED"001 > cspipefile
echo "$SEED"002 > scpipefile
echo "$SEED"003 > scenepipefile
echo "$SEED"004 > tcppipefile


cd $TRITON_HOME/gameserver2/bin
echo "$SEED"011 > cspipefile
echo "$SEED"012 > scpipefile
echo "$SEED"013 > scenepipefile
echo "$SEED"014 > tcppipefile

cd $TRITON_HOME/gameserver3/bin
echo "$SEED"021 > cspipefile
echo "$SEED"022 > scpipefile
echo "$SEED"023 > scenepipefile
echo "$SEED"024 > tcppipefile

cd $TRITON_HOME/gameserver4/bin
echo "$SEED"031 > cspipefile
echo "$SEED"032 > scpipefile
echo "$SEED"033 > scenepipefile
echo "$SEED"034 > tcppipefile

cd $TRITON_HOME/gameserver5/bin
echo "$SEED"041 > cspipefile
echo "$SEED"042 > scpipefile
echo "$SEED"043 > scenepipefile
echo "$SEED"044 > tcppipefile

cd $TRITON_HOME/dungeongameserver/bin
echo "$SEED"051 > cspipefile
echo "$SEED"052 > scpipefile
echo "$SEED"053 > scenepipefile
echo "$SEED"054 > tcppipefile


cd $TRITON_HOME/dungeongameserver2/bin
echo "$SEED"061 > cspipefile
echo "$SEED"062 > scpipefile
echo "$SEED"063 > scenepipefile
echo "$SEED"064 > tcppipefile



