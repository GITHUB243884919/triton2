export TRITON_HOME=/home/common/triton2/server
cd $TRITON_HOME/gameserver/bin
rm *pipe*
touch cspipe1 scene1 scpi1 tcppi1
touch cspipefile scenepipefile  scpipefile tcppipefile
rm  cspipe1 scene1 scpi1 tcppi1

cd $TRITON_HOME/gameserver2/bin
rm *pipe*
touch cspipe2 scene2 scpi2 tcppi2
touch cspipefile scenepipefile  scpipefile tcppipefile
rm  cspipe2 scene2 scpi2 tcppi2
cd $TRITON_HOME/gameserver5/bin
rm *pipe*
touch cspipe5 scene5 scpi5 tcppi5
touch cspipefile scenepipefile  scpipefile tcppipefile
rm  cspipe5 scene5 scpi5 tcppi5
cd $TRITON_HOME/dungeongameserver/bin
rm *pipe*
touch cspipe6 scene6 scpi6 tcppi6
touch cspipefile scenepipefile  scpipefile tcppipefile
rm  cspipe6 scene6 scpi6 tcppi6
cd $TRITON_HOME/dungeongameserver2/bin
rm *pipe*
touch cspipe7 scene7 scpi7 tcppi7
touch cspipefile scenepipefile  scpipefile tcppipefile
rm  cspipe7 scene7 scpi7 tcppi7
