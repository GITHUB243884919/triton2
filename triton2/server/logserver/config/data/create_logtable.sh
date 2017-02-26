#!/bin/bash

if [ $# != 6 ]; then
	echo "cannt find enough parame"
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
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_EXP_$realtab(\
			log_id               int(11) not null auto_increment, \
			role_id              int(11) unsigned, \
			op_time              int(11) unsigned, \
			line_id              int(2), \
			repetion_id          bigint, \
			map_id               int(3), \
			metier_id            int(1), \
			role_level           int(3), \
			exp_type             int(2), \
			op_type              int(2), \
			cur_exp              bigint, \
			delta_exp            int(11) unsigned, \
			op_param1            int(11), \
			op_param2            int(11), \
			op_param3            bigint, \
			INDEX (role_id, op_time), \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"



	let "i= $i + 1"
done

	realtab='01'	

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_LOGOUT_$realtab(\
			log_id               int(11) not null auto_increment, \
			role_id              int(11) unsigned, \
			op_time              int(11) unsigned, \
			line_id              int(2),  \
			repetion_id          bigint,  \
			map_id               int(3),  \
			role_level           int(3),  \
			metier_id            int(1),  \
			login_ip             int(11) unsigned, \
			login_time           int(11) unsigned, \
			logout_time          int(11) unsigned, \
			leave_reason         int(2),  \
			save_reason          int(2),  \
			save_result          int(2),  \
			kick_reason          int,     \
			kick_code            int,	\
			INDEX (role_id, op_time), \
			primary key (log_id))	ENGINE=InnoDB DEFAULT CHARSET=utf8;"


	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_ITEM_$realtab(\
			log_id               int(11) not null auto_increment, \
			role_id              int(11) unsigned, \
			op_time              int(11) unsigned, \
			line_id              int(2), \
			repetion_id          bigint, \
			map_id               int(3), \
			metier_id            int(1), \
			role_level           int(3), \
			item_id              int(11) unsigned, \
			item_num             int(3), \
			item_guid            bigint, \
			item_total           int(4), \
			op_type              int(2) comment 'code_type = 9',\
			op_param1            int(11),\
			op_param2            int(11),\
			op_param3            bigint, \
			INDEX (role_id, op_time, item_id, op_type), \
			INDEX (item_guid),\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"



	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_MONEY_$realtab(\
			log_id               int(11) not null auto_increment, \
			role_id              int(11) unsigned, \
			op_time              int(11) unsigned, \
			line_id              int(2), \
			repetion_id          bigint, \
			map_id               int(3), \
			metier_id            int(1), \
			role_level           int(3), \
			money_type           int(2), \
			op_type              int(2), \
			cur_money            int(11), \
			delta_money          int(11), \
			op_param1            int(11), \
			op_param2            int(11), \
			op_param3            bigint, \
			INDEX (role_id, op_time), \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"


	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_UPGRADE_$realtab(\
			log_id               int(11) not null auto_increment, \
			role_id              int(11) unsigned, \
			upgrade_time         int(11) unsigned, \
			metier_id            int(1), \
			role_level           int(3), \
			upgrade_type         int(3), \
			sub_type             int(3), \
			curr_level           int(3), \
			delta_level          int(3), \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_TASK_$realtab(\
			log_id               int(11) not null auto_increment,\
			role_id              int(11) unsigned,	\
			op_time              int(11) unsigned,	\
			line_id              int(2),	\
			repetion_id          bigint,	\
			map_id               int(3),	\
			metier_id            int(1),	\
			role_level           int(3),	\
			task_id              int(11),	\
			op_type              int(2),	\
			op_param1            int(11),	\
			op_param2            int(11),	\
			op_param3            bigint,	\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_KILLED_$realtab(\
			log_id               int(11) not null auto_increment, \
			role_id              int(11) unsigned,	\
			op_time              int(11) unsigned,	\
			line_id              int(2),	\
			repetion_id          bigint,	\
			map_id               int(3),	\
			metier_id            int(1),	\
			role_level           int(3),	\
			killer_type          int(2),	\
			killer_id            int(11) unsigned,	\
			killer_metier        int(1),	\
			killer_level         int(3),	\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ITEM_UPGRADE_$realtab(\
			log_id               int(11) not null auto_increment,
			item_id              int(11) unsigned,
			item_guid            bigint,
			upgrade_time         int(11) unsigned,
			upgrade_type         int(3),
			result		     int(1),
			role_id		     int(11) unsigned,	
			prev_level           int(3),
			curr_level           int(3),
			consume_item1        int(11),
			consume_item2        int(11),
			INDEX (role_id, item_id, upgrade_time), \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e " \
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ITEM_GHOST_$realtab( \
			log_id               int(11) UNSIGNED not null,\
			role_id              int(11) not null,\
			log_time             int(11) not null,\
			item_guid            bigint, \
			item_stream			 varbinary(512), \
			primary key (role_id, item_guid, log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ITEM_DISPEAR_$realtab(\
			log_id               int(11) not null auto_increment, \
			item_id              int(11) unsigned, \
			item_level           int(3), \
			op_time              int(11) unsigned, \
			line_id              int(2), \
			repetion_id          bigint, \
			map_id               int(3), \
			posx                 int(3), \
			posy                 int(3), \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"


	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ORGE_DROPED_$realtab(\
			log_id               int(11) not null auto_increment, \
			npc_id               int(11) unsigned, \
			npc_index            int(11) unsigned, \
			npc_level            int(3) comment '某些index会随着任务等级的变化而变化', \
			op_time              int(11) unsigned, \
			line_id              int(2), \
			repetion_id          bigint, \
			map_id               int(3), \
			drop_item_id         int(11) unsigned, \
			drop_item_type       int(3), \
			drop_item_level      int(3), \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_FAMILY_$realtab(\
			log_id      int(11) not null auto_increment, \
			family_id 	int(11) unsigned,	\
			op_role_id	int(11) unsigned,	\
			op_time		int(11) unsigned,	\
			op_type		int(2)  unsigned,	\
			attr_id		int(2)  unsigned,	\
			param1 int(11) ,	\
			param2 int(11)	,	\
			param3 int(11)	, \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"
			

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_CORPS_$realtab(\
			log_id      int(11) not null auto_increment, \
			corps_id 	int(11) unsigned,	\
			op_role_id	int(11) unsigned,	\
			op_time		int(11) unsigned,	\
			op_type		int(2)  unsigned,	\
			attr_id		int(2)  unsigned,	\
			param1 int(11) ,	\
			param2 int(11)	,	\
			param3 int(11)  , \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_MAIL_$realtab(\
			log_id              int(11) not null auto_increment, \
			role_id             int(11) unsigned, \
			mail_id             int(11) unsigned, \
			op_type             int(11)  , \
			op_time             int(11) unsigned, \
			mail_type	    int(11), \
			mail_title	    varchar(32), \
			mail_totalnum	    int(11), \
			item_id1	    int(11), \
			item_num1           int(11), \
			item_id2	    int(11), \
			item_num2           int(11), \
			item_id3	    int(11), \
			item_num3           int(11), \
			item_id4	    int(11), \
			item_num4           int(11), \
			item_id5	    int(11), \
			item_num5           int(11), \
			money		    int(11), \
			moneyneedpayback    int(11), \
			desroleid	    int(11), \
			srcname		    varchar(32), \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_CHAT_$realtab(\
			log_id              int(11) not null auto_increment, \
			role_id             int(11) unsigned, \
			chat_msg            varchar(255),\
			gateway_id			int(11) unsigned, \
			channel				int(11) unsigned, \
			op_time				int(11) unsigned, \
			ip					int(11) unsigned, \
			role_name			varchar(32),\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_SERVER_STAT_$realtab(\
			log_id              int(11) not null auto_increment, \
			log_time	    	int(11) unsigned,
			server_id           int(2)  unsigned, \
			catelog_type        int(1)  unsigned, \
			catelog_id          int(4)  , \
			catelog_value       bigint , \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

    $MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_BUFFER_$realtab(\
   		    log_id               int(11) not null auto_increment,\
			role_id              int(11) unsigned, \
		    log_time             int(11) not null, \
			line_id				 int(11) not null, \
			map_id				 int(11) not null, \
			PersistTime          int(11)          , \
			buff_type			 int(4)  unsigned, \
			buff_id				 int(11) unsigned, \
			give_id 			 int(11) unsigned, \
			splayer_id			 int(11) unsigned, \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

    $MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_REPE_SCORE_$realtab(\
   		    log_id               int(11) not null auto_increment,\
			role_id              int(11) unsigned, \
			line_id				 int(11) not null, \
			log_time             int(11) not null, \
			map_index			 int(11) not null, \
			repetion_key		 bigint not null,\
			current_score		 int(11) unsigned, \
			delta_score 		 int(11) unsigned, \
			score_type			 int(4)  unsigned, \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"	

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_MONEY_PRODUCE_$realtab(\
		log_id               int(11) not null auto_increment,\
		log_time             int(11) not null,\
		money_type           int(1) not null,\
		produce_type         int(3) not null,\
		entity_type          int(1) not null,\
		produce_value        bigint not null,\
		primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_MONEY_RECYCLE_$realtab(\
			log_id               int(11) not null auto_increment,\
			log_time             int(11) not null,\
			money_type           int(1) not null,\
			recyle_type          int(3) not null,\
			entity_type          int(1) not null,\
			recycle_value        bigint not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_MONEY_LOOP_$realtab(\
			log_id               int(11) not null auto_increment,\
			log_time             int(11) not null,\
			money_type           int(1) not null,\
			loop_type          int(3) not null,\
			entity_type          int(1) not null,\
			loop_value        bigint not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_EXP_PRODUCE_$realtab(\
			log_id               int(11) not null auto_increment,\
			log_time             int(11) not null,\
			exp_type             int(1) not null,\
			produce_type         int(3) not null,\
			entity_type          int(1) not null,\
			produce_value        bigint not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ITEM_PRODUCE_$realtab(\
			log_id               int(11) not null auto_increment,\
			log_time             int(11) not null,\
			produce_type         int(3) not null,\
			item_type	     int(11) not null,\
			item_level	     int(11) not null,\
			sub_type             int(11) not null,\
			produce_number       bigint not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ITEM_RECYCLE_$realtab(\
			log_id               int(11) not null auto_increment,\
			log_time             int(11) not null,\
			recycle_type         int(3) not null,\
			item_type	     int(11) not null,\
			item_level	     int(11) not null,\
			sub_type             int(11) not null,\
			recycle_number       bigint not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ITEM_LOOP_$realtab(\
			log_id               int(11) not null auto_increment,\
			log_time             int(11) not null,\
			loop_type         int(3) not null,\
			item_type	     int(11) not null,\
			item_level	     int(11) not null,\
			sub_type             int(11) not null,\
			loop_number       bigint not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ACTION_MONIT_$realtab(\
			log_id               int(11) not null auto_increment,\
			log_time             int(11) not null,\
			action_type          int(3) not null,\
			entity_type          int(1) not null,\
			times                bigint not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e "\
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_MONEY_MONIT_$realtab(\
			log_id              int(11) not null auto_increment,\
			log_time            int(11) not null,\
			golden_yb			bigint	 not null default 0,\
			stock_golden_yb		bigint	 not null default 0,\
			silver_yb			bigint	 not null default 0,\
			money				bigint	 not null default 0,\
			storage_money		bigint	 not null default 0,\
			mail_money			bigint	 not null default 0,\
			stock_money			bigint	 not null default 0,\
			bind_money			bigint	 not null default 0,\
			storage_bind_money  bigint	 not null default 0,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e " \
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_REPETION_$realtab( \
			log_id               int(11) not null auto_increment,\
			repetion_key		 bigint not null,\
			type				 int(11) not null,\
			repetion_index		 int(11) not null,\
			lineid				 int(11) not null,\
			parame1				 int(11) not null,\
			parame2				 int(11) not null,\
			optime				 int(11) not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

    $MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e " \
    CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_EXCHANGE_ACCOUNT_$realtab( \
            log_id               int(11) not null auto_increment,\
            account_id           int(11) not null,\
            role_id              int(11) not null,\
            optime               int(11) not null,\
            money                int(11) unsigned not null,\
            yb                  int(11) unsigned not null,\
            operate          int(11) not null,\
            detail_id_1      int(22) not null DEFAULT 0,\
            detail_id_2      int(22) not null DEFAULT 0,\
            parame1          int(11) not null DEFAULT 0,\
            parame2          int(11) not null DEFAULT 0,\
            parame3          int(11) not null DEFAULT 0,\
            parame4          int(11) not null DEFAULT 0,\
            primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e " \
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_BOSSDEATH_$realtab( \
			log_id               int(11) not null auto_increment,\
			deathtime		 bigint not null,\
			campid			 int(11) not null,\
			mapid			 int(11) not null,\
			posx			 int(11) not null,\
			posy			 int(11) not null,\
			tempid			 int(11) not null,\
			killerid		 int(11) not null,\
			damageinfo		 varchar(230) not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e " \
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_LIFESKILL_$realtab( \
			log_id               int(11) not null auto_increment,\
			roleid				 int(11) not null,\
			optime				 int(11) not null,\
			lifeskilltype			 int(11) not null,\
			lifeskillkind			 int(11) not null,\
			lifeskilllevel			 int(11) not null,\
			specializeddegree		 int(11) not null,\
			actionkind			 int(11) not null,\
			composeid 			 int(11) not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e " \
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_MATRIX_$realtab( \
			log_id               int(11) not null auto_increment,\
			slaveid				 int(11) not null,\
			masterid			 int(11) not null,\
			optime				 int(11) not null,\
			optype				 int(11) not null,\
			param1				 int(11) not null,\
			param2				 int(11) not null,\
			param3				 int(11) not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"
			
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e " \
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_WORLD_$realtab( \
			log_id               int(11) not null auto_increment,\
			worldid				 int(11) not null,\
			optime				 int(11) not null,\
			optype				 int(11) not null,\
			param1				 int(11) not null,\
			param2				 int(11) not null,\
			param3				 int(11) not null,\
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	
	$MYSQL -h$DBHOST -P$DBPORT -u$DBUSER -p$DBPASSWD $DATABASE -B -s -e " \
	CREATE TABLE IF NOT EXISTS  ${DAY}_LOG_ROLE_DAMAGE_$realtab( \
			log_id         int(11) not null auto_increment,\
			srccharid			 int(11) not null,\
			descharid			 int(11) not null,\
			optime				 int(11) not null,\
			skillid				 int(11) not null,\
			calcdamage		 int(11) not null,\
			realdamage		 int(11) not null,\
			srcpatt				 int(11) not null,\
			srcmatt				 int(11) not null,\
			despdef				 int(11) not null,\
			desmdef				 int(11) not null,\
			despimu				 int(11) not null,\
			desmimu				 int(11) not null,\
			srcdhit				 int(11) not null,\
			desddef				 int(11) not null,\
			srctitle			 int(11) not null,\
			destitle			 int(11) not null,\
			srcres		     varchar(40), \
			desres		     varchar(40), \
			srcbuff		     varchar(200), \
			desbuff		     varchar(200), \
			primary key (log_id)) ENGINE=InnoDB DEFAULT CHARSET=utf8;"

echo "create sccuess !"


