#!/bin/bash

TRITON_HOME=~/branch_1008_alpha
BUILD_HOME=~/build
SERVER_HOME=$TRITON_HOME/server

PACKAGE=$BUILD_HOME/package/log_`date +%Y%m%d_%H%M%S`.tar

cd $TRITON_HOME

cp ./server/gameserver/config/data/lang/* ./server/logserver/config/data -fr

tar --exclude=.svn --exclude=*.bak.* -cvf  $PACKAGE \
	./server/logserver/bin/logsvrd \
	./server/logserver/bin/libgcc_s.so.1 \
	./server/logserver/bin/libstdc++.so.6 \
	./server/logserver/bin/loglock.lock \
	./server/logserver/bin/logpipefile \
	./server/logserver/config/logserver.xml \
	./server/logserver/config/data/create_logtable_tomorrow.sh \
	./server/logserver/config/data/init_LogDB.sql \
	./server/logserver/config/data/LogDB.sql \
	./server/logserver/log

gzip $PACKAGE

