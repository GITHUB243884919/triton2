#!/bin/bash

if [ $# != 6 ]; then
	echo "cannt find enough parame"
	exit;
fi;

#CONNECT_STR=$1

DBHOST=$1
DBPORT=$2
YESTERDAY=$6
# `date "+%Y%m%d"`
MYSQL=/usr/local/mysql/bin/mysql
DATABASE=$5
DBUSER=$3
DBPASSWD=$4;
#just for table num between 1~16
Begin=1
Max=16

echo -n "Create log tables begin, please wait"
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

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE INDEX IDX_${YESTERDAY}_ROLE_LOGOUT_$realtab 
		ON ${YESTERDAY}_LOG_ROLE_LOGOUT_$realtab(role_id, op_time);"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE INDEX IDX_${YESTERDAY}_ROLE_ITEM_$realtab 
		ON ${YESTERDAY}_LOG_ROLE_ITEM_$realtab(role_id, op_time, item_id, op_type);"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE INDEX IDX_${YESTERDAY}_ROLE_MONEY_$realtab 
		ON ${YESTERDAY}_LOG_ROLE_MONEY_$realtab(role_id, op_time, op_type);"


	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE INDEX IDX_${YESTERDAY}_ROLE_EXP_$realtab 
		ON ${YESTERDAY}_LOG_ROLE_EXP_$realtab(role_id, op_time, op_type);"


	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE INDEX IDX_${YESTERDAY}_ITEM_UPGRADE_$realtab 
		ON ${YESTERDAY}_LOG_ITEM_UPGRADE_$realtab(role_id, upgrade_time, item_id);"

	let "i= $i + 1"
done

	realtab='01'	

echo "create sccuess !"


