#!/bin/bash

MYSQL_HOME=/usr/local/mysql/

ROLE_ID=$1
MYSQL_USER=root
MYSQL_PASSWD=game
MYSQL_DATABASE=GameDB
MYSQL_DUMP=$MYSQL_HOME/bin/mysqldump
MYSQL_BIN=$MYSQL_HOME/bin/mysql
MYSQL_EXEC="$MYSQL_BIN -u$MYSQL_USER -p$MYSQL_PASSWD -s -D$MYSQL_DATABASE -e"
DUMP_FILE=./$ROLE_ID.sql

ACCOUNT_ID=`$MYSQL_EXEC "select account_id from UMS_ROLE where role_id=$ROLE_ID"`
if [ -z $ACCOUNT_ID ]; then
	echo "There is no the role_id $ROLE_ID" in this $MYSQL_DATABASE.
else
	TABLE_ID=`expr $ACCOUNT_ID % 16`
	$MYSQL_DUMP -u$MYSQL_USER -p$MYSQL_PASSWD $MYSQL_DATABASE UMS_ACCOUNT -t -c --skip-opt --hex-blob -w" account_id=$ACCOUNT_ID "  > $DUMP_FILE
	$MYSQL_DUMP -u$MYSQL_USER -p$MYSQL_PASSWD $MYSQL_DATABASE UMS_ROLE -t -c --skip-opt --hex-blob -w" account_id=$ACCOUNT_ID and role_id=$ROLE_ID"  >> $DUMP_FILE
	$MYSQL_DUMP -u$MYSQL_USER -p$MYSQL_PASSWD $MYSQL_DATABASE UMS_ACHIEVE -t -c --skip-opt --hex-blob -w" role_id=$ROLE_ID "  >> $DUMP_FILE
	$MYSQL_DUMP -u$MYSQL_USER -p$MYSQL_PASSWD $MYSQL_DATABASE UMS_ROLE_DETAIL_`printf "%02d" $TABLE_ID` -t -c --skip-opt --hex-blob -w" role_id=$ROLE_ID " >> $DUMP_FILE
	$MYSQL_DUMP -u$MYSQL_USER -p$MYSQL_PASSWD $MYSQL_DATABASE UMS_ROLE_DETAIL1_`printf "%02d" $TABLE_ID` -t -c --skip-opt --hex-blob -w" role_id=$ROLE_ID " >> $DUMP_FILE
fi


