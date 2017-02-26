#!/bin/bash

TRITON_HOME=~/branch_1008_alpha
BUILD_HOME=~/build
SERVER_HOME=$TRITON_HOME/server

PACKAGE=$BUILD_HOME/package/gate_`date +%Y%m%d_%H%M%S`.tar


cd $TRITON_HOME

rm ./server/gateserver/log/* -fr
cp ./server/gameserver/config/data/template/template_server.dat \
	 ./server/gateserver/config/data/template/

tar --exclude=.svn --exclude=*.bak.* -cvf  $PACKAGE \
	./server/gateserver/bin/gateserver  \
	./server/gateserver/bin/cspipefile \
	./server/gateserver/bin/scpipefile \
	./server/gateserver/bin/tcppipefile \
	./server/gateserver/bin/libgcc_s.so.1 \
	./server/gateserver/bin/libstdc++.so.6\
	./server/gateserver/bin/gatepipefile\
	./server/gateserver/bin/tcpsvrd \
	./server/gateserver/config/data \
	./server/gateserver/config/ip.xml \
	./server/gateserver/config/tcpsvrd.cfg \
	./server/gateserver/log

gzip $PACKAGE

