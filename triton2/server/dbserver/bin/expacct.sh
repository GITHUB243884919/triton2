#!/bin/bash

MYSQL_HOME=/usr/local/mysql/

ACCOUNT_ID=11286
DUMP_FILE=./$ACCOUNT_ID.sql
MYSQL_USER=root
MYSQL_PASSWD=game
MYSQL_DUMP=$MYSQL_HOME/bin/mysqldump

TABLE_ID=`expr $ACCOUNT_ID % 16`

$MYSQL_DUMP -u$MYSQL_USER -p$MYSQL_PASSWD GameDB UMS_ACCOUNT -t -c --skip-opt --hex-blob -w" account_id=$ACCOUNT_ID "  > $DUMP_FILE

$MYSQL_DUMP -u$MYSQL_USER -p$MYSQL_PASSWD GameDB UMS_ROLE -t -c --skip-opt --hex-blob -w" account_id=$ACCOUNT_ID "  >> $DUMP_FILE

$MYSQL_DUMP -u$MYSQL_USER -p$MYSQL_PASSWD GameDB UMS_ROLE_DETAIL_`printf "%02d" $TABLE_ID` -t -c --skip-opt --hex-blob -w" role_id in (select role_id from UMS_ROLE where account_id=$ACCOUNT_ID )" >> $DUMP_FILE
$MYSQL_DUMP -u$MYSQL_USER -p$MYSQL_PASSWD GameDB UMS_ROLE_DETAIL1_`printf "%02d" $TABLE_ID` -t -c --skip-opt --hex-blob -w" role_id in (select role_id from UMS_ROLE where account_id=$ACCOUNT_ID )" >> $DUMP_FILE


