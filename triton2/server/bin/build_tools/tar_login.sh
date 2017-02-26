#!/bin/bash

TRITON_HOME=~/branch_1008_alpha
BUILD_HOME=~/build
SERVER_HOME=$TRITON_HOME/server

PACKAGE=$BUILD_HOME/package/login_`date +%Y%m%d_%H%M%S`.tar

echo "cp $SERVER_HOME/gameserver/config/template_server.dat $SERVER_HOME/loginserver/config/"
cp $SERVER_HOME/gameserver/config/data/template/template_server.dat $SERVER_HOME/loginserver/config/data
cp $SERVER_HOME/gameserver/config/data/lang/langTemp_CHS.xml $SERVER_HOME/loginserver/config/data/template
cp $SERVER_HOME/gameserver/config/data/lang/langTemp_CHT.xml $SERVER_HOME/loginserver/config/data/template
cp $SERVER_HOME/gameserver/config/data/lang/langTask_CHS.xml $SERVER_HOME/loginserver/config/data/template

echo "cp $SERVER_HOME/gameserver/config/data/scenetpl/*.xml $SERVER_HOME/logserver/config/map"
cp $SERVER_HOME/gameserver/config/data/scenetpl/*.xml $SERVER_HOME/loginserver/config/data/map/

cd $TRITON_HOME

rm ./server/loginserver/log/* -fr

tar --exclude=.svn --exclude=*.bak.* -cvf  $PACKAGE \
	./server/loginserver/bin/loginserver  \
	./server/loginserver/bin/loginpipefile \
	./server/loginserver/bin/cspipefile \
	./server/loginserver/bin/scpipefile \
	./server/loginserver/bin/tcppipefile \
	./server/loginserver/bin/tcplock.lock \
	./server/loginserver/bin/tcpsvrd \
	./server/loginserver/bin/loginlock.lock \
	./server/loginserver/bin/libgcc_s.so.1 \
	./server/loginserver/bin/libstdc++.so.6 \
	./server/loginserver/config/data/ \
	./server/loginserver/config/tcpsvrd.cfg \
	./server/loginserver/config/ip.xml \
	./server/loginserver/log 

gzip $PACKAGE
