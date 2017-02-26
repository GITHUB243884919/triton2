#!/bin/bash
#
#  - 除UMS_ROLE_DETAIL_XX之外的所有表    PART_YYYYMMDDHH24MISS.SQL 
#  - 当天变化的 UMS_ROLE_DETAIL_XX 数据	 PART_UMS_ROLE_DETAIL.SQL
#

USERNAME=root
PASSWD=game
DBNAME=GameDB

MYSQL_HOME=/usr/local/mysql/
MYSQL_BIN=$MYSQL_HOME/bin/mysql
MYSQL_DUMP=$MYSQL_HOME/bin/mysqldump
MYSQL_EXEC="$MYSQL_BIN -u$USERNAME -p$PASSWD -s -vv -D$DBNAME -e"

DATETIME=`date "+%Y%m%d%H%M%S"`
PR_FILE=FR_$DATETIME.SQL
PA_FILE=FA_$DATETIME.SQL

NUM=`$MYSQL_EXEC "show tables" | wc -l`
HEADNUM=`expr ${NUM} - 3`
TAILNUM=`expr ${NUM} - 7`
ARR1=`$MYSQL_EXEC "show tables" | head -n"$HEADNUM" | tail -n "$TAILNUM"`
ARR2=($ARR1)

DUMPOPT="-u$USERNAME -p"$PASSWD" --hex-blob --skip-opt"

i=0
while [ "$i" -lt "${#ARR2[@]}" ]
do
	TABLENAME=${ARR2[$i]}

	if [ `echo $TABLENAME | cut -c1-16` = "UMS_ROLE_DETAIL_" ]
	then
		$MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME >> $PR_FILE
	else
		$MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -e -R >> $PA_FILE
	fi

	let "i++"
done 

