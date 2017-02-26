#!/bin/bash

TRITON_HOME=~/branch_1008_alpha
BUILD_HOME=~/build
SERVER_HOME=$TRITON_HOME/server

PACKAGE=$BUILD_HOME/package/proxy_`date +%Y%m%d_%H%M%S`.tar


cd $TRITON_HOME

rm ./server/proxyserver/log/* -fr

tar --exclude=.svn --exclude=*.bak.* -cf  $PACKAGE \
	./server/proxyserver/bin/* \
	./server/proxyserver/config/* \
	./server/proxyserver/log 

gzip $PACKAGE

