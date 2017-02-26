#!/bin/bash

TRITON_HOME=~/branch_1008_alpha
BUILD_HOME=~/build
SERVER_HOME=$TRITON_HOME/server

PACKAGE=$BUILD_HOME/package/db_`date +%Y%m%d_%H%M%S`.tar


cd $TRITON_HOME

rm ./server/dbserver/log/* -fr

tar --exclude=.svn --exclude=*.bak.* -cvf  $PACKAGE \
	./server/dbserver/bin/dbsvrd \
	./server/dbserver/bin/dbpipefile \
	./server/dbserver/bin/dblock.lock \
	./server/dbserver/bin/libgcc_s.so.1 \
	./server/dbserver/bin/libstdc++.so.6\
	./server/dbserver/bin/dbproto \
	./server/dbserver/bin/dbgen \
	./server/dbserver/config/data/BackExpiredMail04.sh \
	./server/dbserver/config/data/BackExpiredMail01.sh \
	./server/dbserver/config/data/BackExpiredMail02.sh \
	./server/dbserver/config/data/BackExpiredMail03.sh \
	./server/dbserver/config/data/crontext \
	./server/dbserver/config/data/DelExpiredMail.sh \
	./server/dbserver/config/data/GameDB.sql \
	./server/dbserver/config/data/mailcrond.sh \
	./server/dbserver/config/dbserver.xml \
	./server/dbserver/log

gzip $PACKAGE

