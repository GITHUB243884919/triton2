#!/bin/bash

if [ $# != 6 ]; then
	echo "Usage: $0 dbhost dbport dbuser dbpasswd database YYYYMMDD"
	exit;
fi;

#CONNECT_STR=$1

DBHOST=$1
DBPORT=$2
DAY=$6
# `date "+%Y%m%d"`
MYSQL=/usr/local/mysql/bin/mysql
DATABASE=$5
DBUSER=$3
DBPASSWD=$4;
#just for table num between 1~16
Begin=2
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
	INSERT INTO ${DAY}_LOG_ROLE_LOGOUT_01 \
	SELECT \
			NULL        , \
			role_id     , \
			op_time     , \
			line_id     , \
			repetion_id , \
			map_id      , \
			role_level  , \
			metier_id   , \
			login_ip    , \
			login_time  , \
			logout_time , \
			leave_reason, \
			save_reason , \
			save_result , \
			kick_reason , \
			kick_code     \
	FROM ${DAY}_LOG_ROLE_LOGOUT_$realtab;"
	
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	INSERT INTO ${DAY}_LOG_ROLE_ITEM_01 \
	SELECT						\
			NULL      	, \
			role_id     , \
			op_time     , \
			line_id     , \
			repetion_id , \
			map_id      , \
			metier_id   , \
			role_level  , \
			item_id     , \
			item_num    , \
			item_guid   , \
			item_total  , \
			op_type     , \
			op_param1   , \
			op_param2   , \
			op_param3     \
			FROM ${DAY}_LOG_ROLE_ITEM_$realtab;"



	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	INSERT INTO ${DAY}_LOG_ROLE_MONEY_01 \
	SELECT					 \
			NULL       , \
			role_id    , \
			op_time    , \
			line_id    , \
			repetion_id, \
			map_id     , \
			metier_id  , \
			role_level , \
			money_type , \
			op_type    , \
			cur_money  , \
			delta_money, \
			op_param1  , \
			op_param2  , \
			op_param3    \
		FROM ${DAY}_LOG_ROLE_MONEY_$realtab;"


	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	INSERT INTO ${DAY}_LOG_ROLE_UPGRADE_01 \
	SELECT \
			NULL      , \
			role_id     , \
			upgrade_time, \
			metier_id   , \
			role_level  , \
			upgrade_type, \
			sub_type    , \
			curr_level  , \
			delta_level   \
	FROM ${DAY}_LOG_ROLE_UPGRADE_$realtab; "

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	INSERT INTO ${DAY}_LOG_ROLE_TASK_01 \
	SELECT \
			NULL     , \
			role_id    , \
			op_time    , \
			line_id    , \
			repetion_id, \
			map_id     , \
			metier_id  , \
			role_level , \
			task_id    , \
			op_type    , \
			op_param1  , \
			op_param2  , \
			op_param3    \
	FROM ${DAY}_LOG_ROLE_TASK_$realtab;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	INSERT INTO ${DAY}_LOG_ROLE_KILLED_01 \
	SELECT \
			NULL        , \
			role_id       , \
			op_time       , \
			line_id       , \
			repetion_id   , \
			map_id        , \
			metier_id     , \
			role_level    , \
			killer_type   , \
			killer_id     , \
			killer_metier , \
			killer_level    \
	FROM ${DAY}_LOG_ROLE_KILLED_$realtab;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	INSERT INTO ${DAY}_LOG_ITEM_UPGRADE_01 \
	SELECT \
			NULL       , \
			item_id      , \
			item_guid    , \
			upgrade_time , \
			upgrade_type , \
			result		   , \
			role_id		   , \
			prev_level   , \
			curr_level   , \
			consume_item1, \
			consume_item2  \
	FROM ${DAY}_LOG_ITEM_UPGRADE_$realtab;"
	
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e " \
	INSERT INTO ${DAY}_LOG_ITEM_GHOST_01
	SELECT \
			NULL      ,\
			role_id     ,\
			log_time    ,\
			item_guid   ,\
			item_stream  \
	FROM ${DAY}_LOG_ITEM_GHOST_$realtab; "
	
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "DROP TABLE IF EXISTS ${DAY}_LOG_ROLE_LOGOUT_$realtab;"
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "DROP TABLE IF EXISTS ${DAY}_LOG_ROLE_ITEM_$realtab;"
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "DROP TABLE IF EXISTS ${DAY}_LOG_ROLE_MONEY_$realtab;"
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "DROP TABLE IF EXISTS ${DAY}_LOG_ROLE_UPGRADE_$realtab;"
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "DROP TABLE IF EXISTS ${DAY}_LOG_ROLE_TASK_$realtab;"
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "DROP TABLE IF EXISTS ${DAY}_LOG_ROLE_KILLED_$realtab;"
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "DROP TABLE IF EXISTS ${DAY}_LOG_ITEM_UPGRADE_$realtab;"
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "DROP TABLE IF EXISTS ${DAY}_LOG_ITEM_GHOST_$realtab;"

	let "i= $i + 1"
done

	
echo "merge sccuess !"


