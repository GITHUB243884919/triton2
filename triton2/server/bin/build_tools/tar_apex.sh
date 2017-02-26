#!/bin/bash

TRITON_HOME=~/branch_1008_alpha
BUILD_HOME=~/build
SERVER_HOME=$TRITON_HOME/server

PACKAGE=$BUILD_HOME/package/apex_`date +%Y%m%d_%H%M%S`.tar
APEX=./server/ApexItemServer


cd $TRITON_HOME

tar --exclude=.svn --exclude=*.bak.* -cvf  $PACKAGE \
	$APEX/config  \
	$APEX/hook_log \
	$APEX/AHLoadServer.so \
	$APEX/AHMainServer.so \
	$APEX/AHNetServer \
	$APEX/Apex2Server.so 

gzip $PACKAGE
