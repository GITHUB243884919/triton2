#!/usr/bin/ksh

if [ $# -ne 6 ] 
then
	$mysql -u$1 -p$2 -s -e  "wrong param: 1.usr 2.pwd 3.sourcedb 4.desdb 5.begindate 6.enddate"
	exit
fi

mysql=/usr/local/mysql/bin/mysql

exec 6>&1 
exec >table
$mysql -u$1 -p$2 << !
        use LogDB
        show tables;
!

#exec > tablelist.sql
exec 1>&6
for tablename in `grep -e '[0-9]_.' table`
do
        date=$( echo $tablename | cut -c1-8 )
        if $( test "$date" -ge $5 -a "$date" -le $6 )
        then	
		case $tablename
		in
		*LOG_ROLE_TASK*) echo  "merge table: $tablename"
		$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			NULL , \
			role_id,\
			op_time,\
			line_id,  \
			repetion_id, \
                        map_id, \
                        metier_id , \
                        role_level , \
                        task_id    , \
                        op_type    , \
                        op_param1  , \
                        op_param2  , \
                        op_param3    \
			from $3.$tablename;"
			;; 
		*LOG_ROLE_KILLED*) echo  "merge table: $tablename"  
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
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
		        from $3.$tablename;" 
			;; 
		*LOG_ROLE_LOGOUT*) echo  "merge table: $tablename"
		 	$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select  \
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
		 	from $3.$tablename;"  
			;; 
		*LOG_ROLE_EXP*) echo  "merge table: $tablename"    
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			null,	\
			role_id, \
			op_time, \
		        line_id, \
			repetion_id, \
			map_id, \
			metier_id, \
			role_level, \
			exp_type, \
			op_type, \
			cur_exp, \
			delta_exp, \
			op_param1, \
			op_param2, \
			op_param3 \
			from $3.$tablename;" 
			;; 
		*LOG_ROLE_ITEM*) echo  "merge table: $tablename"    
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
                        NULL            , \
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
			from $3.$tablename;"
			;; 
		*LOG_ROLE_UPGRADE*) echo  "merge table: $tablename" 
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			NULL      , \
                        role_id     , \
                        upgrade_time, \
                        metier_id   , \
                        role_level  , \
                        upgrade_type, \
                        sub_type    , \
                        curr_level  , \
                        delta_level   \
			from $3.$tablename;" 
			;; 
		*LOG_ITEM_UPGRADE*) echo  "merge table: $tablename" 
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			NULL, \
			item_id, \
			item_guid, \
			upgrade_time, \
			upgrade_type, \
			result,\
			role_id, \
			prev_level, \
			curr_level, \
			consume_item1, \
			consume_item2 \
			from $3.$tablename;" 
			;; 
		*LOG_ROLE_MONEY*) echo  "merge table: $tablename"   
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
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
		        from $3.$tablename;"   
			;; 
		*LOG_ROLE_KILLED*) echo  "merge table: $tablename"
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
		 	NULL,       \
                 	role_id       , \
                 	op_time       , \
                 	line_id       , \
                 	repetion_id  , \
                        map_id        , \
                        metier_id     , \
                        role_level    , \
                        killer_type   , \
                        killer_id     , \
                        killer_metier , \
                        killer_level    \
			from $3.$tablename;"  
			;;  
		*LOG_ORGE_DROPED*) echo  "merge table: $tablename"  
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			null,	\
			npc_id,	\
			npc_index, \
			npc_level, \
			op_time, \
			line_id, \
			repetion_id, \
			map_id, \
			drop_item_id, \
			drop_item_type, \
			drop_item_level	\
			from $3.$tablename;"
			;;    
		*LOG_ITEM_DISPEAR*) echo  "merge table: $tablename" 
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			NULL, \
			item_id, \
			item_level, \
			op_time, \
			line_id, \
			repetion_id, \
			map_id, \
			posx, \
			posy \
			from $3.$tablename;"   
			;; 
		*LOG_FAMILY*) echo  "merge table: $tablename" 
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			NULL, \
			family_id, \
			op_role_id, \
			op_time, \
			op_type, \
			attr_id, \
			param1, \
			param2, \
			param3   \
			from $3.$tablename;"  
			;;  
		*LOG_CORPS*) echo  "merge table: $tablename" 
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			NULL, \
			corps_id, \
			op_role_id, \
			op_time, \
			op_type, \
			attr_id, \
			param1, \
			param2, \
			param3 \
			from $3.$tablename;" 
			;;   
		*LOG_ROLE_MAIL*) echo  "merge table: $tablename"
			 $mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			 NULL,
			 role_id, \
			 mail_id, \
			 op_type, \
			 op_time, \
			 mail_type, \
			 mail_title, \
			 mail_totalnum, \
			 item_id1,  \
			 item_num1, \
			 item_id2, \
			 item_num2, \
			 item_id3, \
		 	 item_num3, \
			 item_id4, \
			 item_num4, \
			 item_id5, \
			 item_num5, \
			 money, \
			 moneyneedpayback, \
			 desroleid, \
			 srcname \
			 from $3.$tablename;"  
			 ;;  
		*LOG_ROLE_CHAT*) echo  "merge table: $tablename" 
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			NULL, \
			role_id, \
			chat_msg, \
			gateway_id, \
			channel,  \
			op_time,  \
			ip , \
			role_name \
			from $3.$tablename;"  
			;;  
		*LOG_REPETION*) echo  "merge table: $tablename" 
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			NULL, \
			repetion_key,  \
			type, \
			repetion_index, \
			lineid, \
			parame1, \
			parame2, \
			optime \
			from $3.$tablename;"  
			;; 
		*LOG_ITEM_GHOST*) echo  "merge table: $tablename"
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			log_id, \
			role_id, \
			log_time, \
			item_guid,
			item_stream \
			from $3.$tablename;"  
			;; 
		*LOG_LIFESKILL*) echo  "merge table: $tablename" 
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			NULL, \
			roleid, \
			optime, \
			lifeskilltype, \
			lifeskillkind, \
			lifeskilllevel, \
			specializeddegree, \
			actionkind, \
			composeid  \
			from $3.$tablename;" 
			;; 
		*LOG_BOSSDEATH*) echo  "merge table: $tablename" 
			$mysql -u$1 -p$2 -s -e  "insert into $4.$tablename select \
			log_id, \
			deathtime, \
			campid, \
			mapid, \
			posx, \
			posy, \
			tempid, \
			killerid, \
			damageinfo \
			from $3.$tablename;"
		esac
        fi
done

#/usr/local/mysql/bin/mysql -u$1 -p$2 < tablelist.sql
#rm table
