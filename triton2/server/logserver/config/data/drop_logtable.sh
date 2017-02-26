#!/bin/bash

if [ $# != 6 ]; then
	echo "cannt find enough parame"
	exit;
fi;

#CONNECT_STR=$1

DUMP_HOME=./dump
DBHOST=$1
DBPORT=$2
DAY=$6
# `date "+%Y%m%d"`
MYSQL=/usr/local/mysql/bin/mysql
DATABASE=$5
DBUSER=$3
DBPASSWD=$4;
#just for table num between 1~16
Begin=1
Max=16
echo -n "drop log tables begin, please wait"
i=$Begin

while [ $i -le $Max ]
do
	if [ "$i" -lt "10" ]
	then
		realtab='0'$i
	else
		realtab=$i
 	fi

	echo -n "."
	$MYSQL -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ROLE_LOGOUT_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ROLE_ITEM_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ROLE_MONEY_$realtab;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ROLE_EXP_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ROLE_UPGRADE_$realtab;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ROLE_TASK_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ROLE_KILLED_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ITEM_UPGRADE_$realtab ;"
		

	let "i= $i + 1"
done

	realtab='01'	
	
	$MYSQL -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ITEM_DISPEAR_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ORGE_DROPED_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_FAMILY_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_CORPS_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ROLE_MAIL_$realtab;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ROLE_CHAT_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_SERVER_STAT_$realtab;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_MONEY_PRODUCE_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_MONEY_RECYCLE_$realtab;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_EXP_PRODUCE_$realtab  ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ITEM_PRODUCE_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ITEM_RECYCLE_$realtab ;\
	DROP TABLE IF EXISTS $DATABASE.${DAY}_LOG_ACTION_MONIT_$realtab ;"
	
echo "drop sccuess !"
