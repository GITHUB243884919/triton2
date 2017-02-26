#!/bin/bash

BUILD_HOME=~/build
TRITON_HOME=~/branch_1008_alpha
SERVER_HOME=$TRITON_HOME/server
PACKAGE=$BUILD_HOME/package/game_`date +%Y%m%d_%H%M%S`.tar
GAMEPATH=./server/gameserver

cd $TRITON_HOME

rm $GAMEPATH/log/* -fr

tar --exclude=.svn --exclude=*.bak.* -cvf  $PACKAGE \
	$GAMEPATH/bin/ApexProxy.so  \
	$GAMEPATH/bin/AHThunkConfig.xml \
	$GAMEPATH/bin/cspipefile \
	$GAMEPATH/bin/scpipefile \
	$GAMEPATH/bin/sceneserver \
	$GAMEPATH/bin/tcppipefile \
	$GAMEPATH/bin/tcpsvrd \
	$GAMEPATH/bin/ApexProxy.so \
	$GAMEPATH/bin/libgcc_s.so.1 \
	$GAMEPATH/bin/libstdc++.so.6 \
	$GAMEPATH/bin/lib \
	$GAMEPATH/config/data \
	$GAMEPATH/config/tcpsvrd.cfg \
	$GAMEPATH/config/sceneserver.xml \
	$GAMEPATH/log

gzip $PACKAGE
