#!/bin/bash
#
#  - 除UMS_ROLE_DETAIL_XX之外的所有表    PART_YYYYMMDDHH24MISS.SQL 
#  - 当天变化的 UMS_ROLE_DETAIL_XX 数据	 PART_UMS_ROLE_DETAIL.SQL
#

USERNAME=root
PASSWD=game
DBNAME=LogDB

if [ $# != 1 ]; then
	echo "Usage: $0 table_header"
	exit;
fi;

MYSQL_HOME=/usr/local/mysql/
MYSQL_BIN=$MYSQL_HOME/bin/mysql
MYSQL_DUMP=$MYSQL_HOME/bin/mysqldump
MYSQL_EXEC="$MYSQL_BIN -u$USERNAME -p$PASSWD -s -vv -D$DBNAME -e"
HOST_NAME=`hostname`_LogDB
mkdir -p $HOST_NAME 
PART=$1
TABLE_SQL="show tables like '$PART%' "



NUM=`$MYSQL_EXEC "$TABLE_SQL" | wc -l`
HEADNUM=`expr ${NUM} - 3`
TAILNUM=`expr ${NUM} - 7`
ARR1=`$MYSQL_EXEC "$TABLE_SQL" | head -n"$HEADNUM" | tail -n "$TAILNUM"`
ARR2=($ARR1)

DUMPOPT="-u$USERNAME -p"$PASSWD" --hex-blob -t -q"

i=0
while [ "$i" -lt "${#ARR2[@]}" ]
do
	TABLENAME=${ARR2[$i]}

	if [ `echo $TABLENAME | cut -c9-24` = "_LOG_ITEM_GHOST_" ]
	then
		$MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME > $HOST_NAME/$TABLENAME.SQL
	else
		$MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME > $HOST_NAME/$TABLENAME.TMP
		grep 'INSERT INTO ' $HOST_NAME/$TABLENAME.TMP | sed 's/([0-9]*,/(NULL,/g' > $HOST_NAME/$TABLENAME.SQL
		rm -rf $HOST_NAME/$TABLENAME.TMP
	fi

	let "i++"
done 

