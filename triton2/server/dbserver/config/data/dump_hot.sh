#!/bin/bash
#
#  - 删除表结构的时候, 一定要考虑如何增量备份
#  -
#  - 除UMS_ROLE_DETAIL_XX之外的所有表    PART_YYYYMMDDHH24MISS.SQL 
#  - 当天变化的 UMS_ROLE_DETAIL_XX 数据	 PART_UMS_ROLE_DETAIL.SQL
#

USERNAME=root
PASSWD=game
DBNAME=GameDB_HuaGS2

MYSQL_HOME=/usr/local/mysql/
MYSQL_BIN=$MYSQL_HOME/bin/mysql
MYSQL_DUMP=$MYSQL_HOME/bin/mysqldump
MYSQL_EXEC="$MYSQL_BIN -u$USERNAME -p$PASSWD -s -vv -D$DBNAME -e"

DATETIME=`date "+%Y%m%d%H%M%S"`
PR_FILE=PR_$DATETIME.SQL
PA_FILE=PA_$DATETIME.SQL

NUM=`$MYSQL_EXEC "show tables" | wc -l`
HEADNUM=`expr ${NUM} - 3`
TAILNUM=`expr ${NUM} - 7`
ARR1=`$MYSQL_EXEC "show tables" | head -n"$HEADNUM" | tail -n "$TAILNUM"`
ARR2=($ARR1)


THIS_DATE=`echo $DATETIME | cut -c1-8`
WHERE=" role_id IN (SELECT role_id FROM UMS_ROLE WHERE date(update_time) = $THIS_DATE) "
WHERE2=" account_id IN (SELECT account_id FROM UMS_ROLE WHERE date(update_time) = $THIS_DATE) "
WHERE3=" date(update_time) = $THIS_DATE "
DUMPOPT="-u$USERNAME -p"$PASSWD" --hex-blob -t --skip-opt "

i=0
while [ "$i" -lt "${#ARR2[@]}" ]
do
	TABLENAME=${ARR2[$i]}

	if [ `echo $TABLENAME | cut -c1-16` = "UMS_ROLE_DETAIL_" ]; then
		echo $MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -w "$WHERE"
		$MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -w "$WHERE" >> $PR_FILE
	elif [ `echo $TABLENAME | cut -c1-17` = "UMS_ROLE_DETAIL1_" ]; then
		echo $MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -w "$WHERE"
		$MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -w "$WHERE" >> $PR_FILE
	elif [ $TABLENAME = "UMS_ACCOUNT" ]; then
		echo DUMP $MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -w "$WHERE2" 
		$MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -w "$WHERE2" >> $PA_FILE
	elif [ $TABLENAME = "UMS_ROLE" ]; then
		echo DUMP $MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -w "$WHERE3" 
		$MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -w "$WHERE3" >> $PA_FILE
	elif [ $TABLENAME = "UMS_ACHIEVE" ]; then
		echo DUMP $MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -w "$WHERE" 
		$MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -w "$WHERE" >> $PA_FILE
	else 
		if [ $TABLENAME != "GMS_CHARGE_LOG" ] && [ $TABLENAME != "GMS_CONSUME_LOG" ] && [ $TABLENAME != "GMS_IBITEM_USED_LOCKED" ] && [ $TABLENAME != "GMS_IBITEM_USED_LOG" ] && [ $TABLENAME != "GMS_CLIENTHOST_INFO" ]; then
			echo $MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME
			$MYSQL_DUMP $DUMPOPT $DBNAME $TABLENAME -e >> $PA_FILE
		else
			echo "DO NOT DUMP TABLE $TABLENAME"
		fi
	fi

	let "i++"
done 

