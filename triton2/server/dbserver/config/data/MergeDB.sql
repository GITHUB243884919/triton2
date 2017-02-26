
-- ----------------------------
-- Procedure structure for `MergeGameDB` *******MUST IN SRC DB!!!!
-- ----------------------------
DROP PROCEDURE IF EXISTS `MergeGameDB`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `MergeGameDB`(SRC_DB varchar(32), DST_DB varchar(32))
BEGIN
	#Routine body goes here...
	DECLARE TABLE_IDX INT(10) DEFAULT 0;
	DECLARE TABLE_NAME varchar(64);
	DECLARE LOG_TABLE  varchar(64);
	DECLARE WORLD_ID INT(10) DEFAULT 1;
	DECLARE COUNTRY_ID INT(10) DEFAULT 0;
	DECLARE TMP_INTVAL1 INT(10) DEFAULT 0;
	DECLARE TMP_INTVAL2 INT(10) DEFAULT 0;

	DECLARE VAR_ACCOUNT_ID			int DEFAULT 0;
	DECLARE VAR_CHARGED_GOLDEN_YB	int DEFAULT 0;
	DECLARE VAR_CONSUMED_GOLDEN_YB	int DEFAULT 0;
	DECLARE VAR_GOLDEN_YB			int DEFAULT 0;
	DECLARE done int DEFAULT 0;
	DECLARE found_num int DEFAULT 0;
	DECLARE COMPENSATION  int DEFAULT 100;


	DECLARE CUR_ACCOUNT CURSOR FOR (SELECT account_id,charged_golden_yb,consumed_golden_yb,golden_yb FROM UMS_ACCOUNT );

	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done=1;
	
	DROP TABLE IF EXISTS UMS_NPC;
	CREATE TABLE UMS_NPC(
	npc_id int, 
	npc_name varchar(32), 
	npc_money int,
	PRIMARY KEY (npc_id)
	);
	
	INSERT INTO UMS_NPC VALUES(51198, '平顶山土地', 20);
	INSERT INTO UMS_NPC VALUES(46907, '白虎岭土地', 20);
	INSERT INTO UMS_NPC VALUES(36779, '高老庄土地', 40);
	INSERT INTO UMS_NPC VALUES(34183, '黑风山山神', 20);
	INSERT INTO UMS_NPC VALUES(40229, '万寿山山神', 30);
	INSERT INTO UMS_NPC VALUES(46893, '车迟国国王', 50);
	INSERT INTO UMS_NPC VALUES(42681, '女儿国国王', 50);
	INSERT INTO UMS_NPC VALUES(39338, '宝象国国王', 80);
	INSERT INTO UMS_NPC VALUES(7373, '程咬金', 100);
	INSERT INTO UMS_NPC VALUES(60669, '积累山山神', 20);
	INSERT INTO UMS_NPC VALUES(46877, '乌鸡国国王', 50);
	INSERT INTO UMS_NPC VALUES(42675, '火焰山土地', 40);
	INSERT INTO UMS_NPC VALUES(0, '齐天大圣', 150);


	#SET @world_war_money = 100;

	SELECT "OP UMS_ACCOUNT ... ";

	SELECT "LOG:count of src UMS_ACCOUNT ... ";
	SET @SqlCmd=CONCAT('SELECT count(*) from ', SRC_DB, '.UMS_ACCOUNT' );
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt;

	SELECT "LOG:count of des UMS_ACCOUNT ... ";
	SET @SqlCmd=CONCAT('SELECT count(*) from ', DST_DB, '.UMS_ACCOUNT' );
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt;

	OPEN CUR_ACCOUNT;
	while done = 0 do
		fetch CUR_ACCOUNT into VAR_ACCOUNT_ID,VAR_CHARGED_GOLDEN_YB, VAR_CONSUMED_GOLDEN_YB, VAR_GOLDEN_YB;

		SET @SqlCmd=CONCAT('SELECT account_id from ', DST_DB, '.UMS_ACCOUNT WHERE account_id =', VAR_ACCOUNT_ID );
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;	
		SELECT FOUND_ROWS() into found_num;

		if found_num=0 then
			SET @SqlCmd=CONCAT('
				INSERT INTO ', DST_DB, '.UMS_ACCOUNT(
					account_id,
					user_name,
					passwd,
					account_status,
					adult_flag,
					user_class,
					user_point,
					trustee_passwd,
					binded_ip,
					charged_golden_yb,
					consumed_golden_yb,
					charged_silver_yb,
					consumed_silver_yb,
					golden_yb,
					silver_yb,
					url_id,
					check_sum,
					user_flag,
					update_time,
					lastlogin_ip)
				SELECT 
				account_id,
				user_name,
				passwd,
				account_status,
				adult_flag,
				user_class,
				user_point,
				trustee_passwd,
				binded_ip,
				charged_golden_yb,
				consumed_golden_yb,
				charged_silver_yb,
				consumed_silver_yb,
				golden_yb,
				silver_yb,
				url_id,
				check_sum,
				user_flag,
				update_time,
				lastlogin_ip
				FROM ',SRC_DB,'.UMS_ACCOUNT where account_id=',VAR_ACCOUNT_ID,';' );
			PREPARE stmt FROM @SqlCmd;
			EXECUTE stmt;

		else
			SET @SqlCmd2=CONCAT('UPDATE ', DST_DB, '.UMS_ACCOUNT set 
				charged_golden_yb = charged_golden_yb+',VAR_CHARGED_GOLDEN_YB,',
				consumed_golden_yb = consumed_golden_yb+',VAR_CONSUMED_GOLDEN_YB,',
				golden_yb = golden_yb+',VAR_GOLDEN_YB,'
				WHERE account_id =', VAR_ACCOUNT_ID );
			PREPARE stmt FROM @SqlCmd2;
			EXECUTE stmt;
		end if;

	end while;
	CLOSE CUR_ACCOUNT;

	SELECT "LOG:count of new des UMS_ACCOUNT ... ";
	SET @SqlCmd=CONCAT('SELECT count(*) from ', DST_DB, '.UMS_ACCOUNT' );
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt;
	################################

	SELECT "OP UMS_ROLE ... ";	
	SELECT "LOG:count of src UMS_ROLE ... ";
	SET @SqlCmd=CONCAT('SELECT count(*) from ', SRC_DB, '.UMS_ROLE' );
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt;

	SELECT "LOG:count of dst UMS_ROLE ... ";
	SET @SqlCmd=CONCAT('SELECT count(*) from ', DST_DB, '.UMS_ROLE' );
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt;

	SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.UMS_ROLE(
			role_id,                
			role_status,            
			account_id,             
			role_name,              
			is_alive,               
			gender_id,              
			metier_id,              
			nationality_id,         
			face_type,              
			line_id,                
			map_id,                 
			pos_x,                  
			pos_y,                  
			role_level,             
			role_expr,              
			role_money,             
			bounded_money,          
			country_id,             
			login_time,             
			online_duration,        
			update_time,            
			create_time,            
			role_password,          
			save_status,            
			save_bitflag,           
			check_sum,              
			role_status_param,      
			role_familyid,          
			role_corpsid,           
			charged_silver_yb,      
			consumed_silver_yb,     
			silver_yb,              
			storage_money,          
			storage_bounded_money,  
			role_deletedtime,       
			role_protecttime,       
			role_vip_flag,
			role_vip_time,
			role_marriageid,
			role_changecamp_time,
			role_swornid)        
		SELECT
		role_id,
		role_status,
		account_id,
		role_name,
		is_alive,
		gender_id,
		metier_id,
		nationality_id,
		face_type,
		line_id,
		map_id,
		pos_x,
		pos_y,
		role_level,
		role_expr,
		role_money,
		bounded_money,
		country_id,
		login_time,
		online_duration,
		update_time,
		create_time,
		role_password,
		save_status,
		save_bitflag,
		check_sum,
		role_status_param,
		role_familyid,
		role_corpsid,
		charged_silver_yb,
		consumed_silver_yb,
		silver_yb,
		storage_money,
		storage_bounded_money,
		role_deletedtime,
		role_protecttime,
		role_vip_flag,
		role_vip_time,
		role_marriageid,
		role_changecamp_time,
		role_swornid
		FROM ',SRC_DB, '.UMS_ROLE;' );

	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt;

	SELECT "LOG:count of new dst UMS_ROLE ... ";
	SET @SqlCmd=CONCAT('SELECT count(*) from ', DST_DB, '.UMS_ROLE' );
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt;

	################################

WHILE ( TABLE_IDX < 16 ) DO
		IF( TABLE_IDX  < 10 ) then
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_DETAIL_0', TABLE_IDX );
		ELSE
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_DETAIL_', TABLE_IDX );
		END IF;

SET LOG_TABLE=CONCAT('LOG:count of src ', TABLE_NAME );
SELECT LOG_TABLE;
SET @SqlCmd=CONCAT('SELECT count(*) from ', SRC_DB, '.', TABLE_NAME );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET LOG_TABLE=CONCAT('LOG:count of dst ', TABLE_NAME );
SELECT LOG_TABLE;
SET @SqlCmd=CONCAT('SELECT count(*) from ', DST_DB, '.', TABLE_NAME );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('INSERT INTO ', DST_DB, '.' , TABLE_NAME,  '(
		role_id,
		role_hp,
		role_mp,
		role_ap,
		role_basic,
		role_skill,
		role_buff,
		role_cdgroup,
		role_package,
		role_equipment,
		role_storage,
		role_title,
		role_activity,
		role_task_package,
		role_repetion,
		role_pvp,
		save_bitflag,
		check_sum)
	SELECT 
		role_id,
		role_hp,
		role_mp,
		role_ap,
		role_basic,
		role_skill,
		role_buff,
		role_cdgroup,
		role_package,
		role_equipment,
		role_storage,
		role_title,
		role_activity,
		role_task_package,
		role_repetion,
		role_pvp,
		save_bitflag,
		check_sum
	FROM ',SRC_DB,'.', TABLE_NAME, ';' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd = CONCAT( 'SELECT \'op\' ', TABLE_NAME);
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET LOG_TABLE=CONCAT('LOG:count of  new dst ', TABLE_NAME );
SELECT LOG_TABLE;
SET @SqlCmd=CONCAT('SELECT count(*) from ', DST_DB, '.', TABLE_NAME );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET TABLE_IDX=TABLE_IDX+1;

END WHILE;

SET TABLE_IDX=0;
WHILE ( TABLE_IDX < 16 ) DO
		IF( TABLE_IDX  < 10 ) then
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_DETAIL1_0', TABLE_IDX );
		ELSE
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_DETAIL1_', TABLE_IDX );
		END IF;

SET LOG_TABLE=CONCAT('LOG:count of src ', TABLE_NAME );
SELECT LOG_TABLE;
SET @SqlCmd=CONCAT('SELECT count(*) from ', SRC_DB, '.', TABLE_NAME );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET LOG_TABLE=CONCAT('LOG:count of dst ', TABLE_NAME );
SELECT LOG_TABLE;
SET @SqlCmd=CONCAT('SELECT count(*) from ', DST_DB, '.', TABLE_NAME );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('INSERT INTO ', DST_DB, '.' , TABLE_NAME,  '(
		role_id,
		role_task,
		role_talent,
		role_monster_record,
		role_friend,
		role_tele_pos,
		role_wizard,
		role_lifeskill,
		role_client,
		role_charm,
		role_guide,
		role_times,
		role_matrix,
		role_exptimeinfo,
		role_mw,
		role_statistic,
		role_treasurebowl,
		role_spiritinfo)
	SELECT 
		role_id,
		role_task,
		role_talent,
		role_monster_record,
		role_friend,
		role_tele_pos,
		role_wizard,
		role_lifeskill,
		role_client,
		role_charm,
		role_guide,
		role_times,
		role_matrix,
		role_exptimeinfo,
		role_mw,
		role_statistic,
		role_treasurebowl,
		role_spiritinfo
	FROM ',SRC_DB,'.', TABLE_NAME, ';' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd = CONCAT( 'SELECT \'op\' ', TABLE_NAME);
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET LOG_TABLE=CONCAT('LOG:count of  new dst ', TABLE_NAME );
SELECT LOG_TABLE;
SET @SqlCmd=CONCAT('SELECT count(*) from ', DST_DB, '.', TABLE_NAME );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET TABLE_IDX=TABLE_IDX+1;

END WHILE;


################################

SELECT "OP UMS_ROLE_REMOVED ... ";	

SET @SqlCmd=CONCAT('INSERT INTO ', DST_DB,'.UMS_ROLE_REMOVED (
		role_id,
		role_status,
		account_id,
		role_name,
		is_alive,
		gender_id,
		metier_id,
		nationality_id,
		face_type,
		line_id,
		map_id,
		pos_x,
		pos_y,
		role_level,
		role_expr,
		role_money,
		bounded_money,
		login_time,
		online_duration,
		update_time,
		create_time,
		role_password,
		save_status,
		save_bitflag,
		check_sum,
		role_status_param,
		role_familyid,
		role_corpsid,
		charged_silver_yb,
		consumed_silver_yb,
		silver_yb,
		storage_money,
		storage_bounded_money )
	SELECT
	role_id,
	role_status,
	account_id,
	role_name,
	is_alive,
	gender_id,
	metier_id,
	nationality_id,
	face_type,
	line_id,
	map_id,
	pos_x,
	pos_y,
	role_level,
	role_expr,
	role_money,
	bounded_money,
	login_time,
	online_duration,
	update_time,
	create_time,
	role_password,
	save_status,
	save_bitflag,
	check_sum,
	role_status_param,
	role_familyid,
	role_corpsid,
	charged_silver_yb,
	consumed_silver_yb,
	silver_yb,
	storage_money,
	storage_bounded_money
	FROM ',SRC_DB,'.UMS_ROLE_REMOVED;');


SELECT "OP UMS_ROLE_DETAIL_REMOVED ... ";	

SET @SqlCmd=CONCAT('INSERT INTO ', DST_DB,'.UMS_ROLE_DETAIL_REMOVED (
		role_id,
		save_bitflag,
		role_hp,
		role_mp,
		role_ap,
		role_basic,
		role_skill,
		role_buff,
		role_cdgroup,
		role_package,
		role_equipment,
		role_storage,
		role_pvp,
		role_title,
		role_activity,
		role_talent,
		role_mw,
		role_lifeskill,
		role_statistic,
		role_task,
		role_task_package,
		role_monster_record,
		role_tele_Pos,
		role_wizard,
		role_friend,
		role_repetion,
		role_client,
		remove_id,
		check_sum)
	SELECT
	role_id,
	save_bitflag,
	role_hp,
	role_mp,
	role_ap,
	role_basic,
	role_skill,
	role_buff,
	role_cdgroup,
	role_package,
	role_equipment,
	role_storage,
	role_pvp,
	role_title,
	role_activity,
	role_talent,
	role_mw,
	role_lifeskill,
	role_statistic,
	role_task,
	role_task_package,
	role_monster_record,
	role_tele_Pos,
	role_wizard,
	role_friend,
	role_repetion,
	role_client,
	remove_id,
	check_sum  
	FROM ',SRC_DB,'.UMS_ROLE_DETAIL_REMOVED;');

#role_charm,
#role_guide,
#role_times,

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################

SET TABLE_IDX=1;

WHILE ( TABLE_IDX <= 4 ) DO

	SET TABLE_NAME=CONCAT( 'UMS_ROLE_MAIL_0', TABLE_IDX );

	SET @SqlCmd = CONCAT( 'SELECT \'op\' ', TABLE_NAME);
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt;	

	SET @SqlCmd=CONCAT('INSERT INTO ',DST_DB,'.', TABLE_NAME, '(
			mail_type,
			mail_status,
			sender_role_id,
			sender_role_name,
			recver_role_id,
			mail_title,
			mail_body,
			send_time,
			money_attachment,
			item_number,
			item_attachment,
			post_recver,
			expired_time,
			recver_role_name)
		SELECT 
		mail_type,
		mail_status,
		sender_role_id,
		sender_role_name,
		recver_role_id,
		mail_title,
		mail_body,
		send_time,
		money_attachment,
		item_number,
		item_attachment,
		post_recver,
		expired_time,
		recver_role_name
		FROM ', SRC_DB, '.', TABLE_NAME, ';');

	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt;

	SET TABLE_IDX=TABLE_IDX+1;
END WHILE;


################################

SELECT "OP UMS_FAMILY ... ";	

SET @SqlCmd=CONCAT('INSERT INTO ', DST_DB, '.UMS_FAMILY (
		family_id,
		family_name,
		family_notice,
		family_money,
		family_status,
		create_role_id,
		create_role_name,
		member_info,
		country_id,
		corps_id,
		npc_id,
		bid_money,
		bid_time,
		disband_time,
		war_rank,
		war_winnum,
		war_lostnum,
		family_starlevel,
		war_exp,
		family_glory,
		war_status,
		family_npcmoney,
		family_npcglory,
		family_npcwelfaretime,
		family_level,
		family_creattime,
		family_repetionopendate,
		family_repetionopentimes,
		family_repetionscore,
		family_shopinfo,
		family_stability,
		family_weeknum )
	SELECT 
	family_id,
	family_name,
	family_notice,
	family_money,
	family_status,
	create_role_id,
	create_role_name,
	member_info,
	country_id,
	corps_id,
	npc_id,
	bid_money,
	bid_time,
	disband_time,
	war_rank,
	war_winnum,
	war_lostnum,
	family_starlevel,
	war_exp,
	family_glory,
	war_status,
	family_npcmoney,
	family_npcglory,
	family_npcwelfaretime,
	family_level,
	family_creattime,
	family_repetionopendate,
	family_repetionopentimes,
	family_repetionscore,
	family_shopinfo,
	family_stability,
	family_weeknum
	FROM ', SRC_DB, '.UMS_FAMILY;' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

### 家族补偿
SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_FAMILY SET family_money=family_money+10 WHERE npc_id>0;' );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_FAMILY SET npc_id=0, family_money=family_money+ bid_money, bid_money=0;' );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################

SELECT "OP UMS_CORPS ... ";	




SET @SqlCmd=CONCAT( 'INSERT INTO ',  DST_DB, '.UMS_CORPS(
		corps_id,
		corps_name,
		corps_notice,
		create_role_id,
		create_role_name,
		corps_level,
		corps_money,
		family_info,
		country_id,
		disband_time,
		corps_status,
		corps_glory,
		war_winnum,
		war_lostnum,
		war_score,
		war_starlevel,
		war_bindnpcid,
		war_bindmoney,
		war_battlestatus,
		war_campid,
		war_battlecity,
		war_battlekey,
		war_welfaregettime,
		corps_roommoney,
		war_battleinfo,
		corps_creattime,
		corps_color,
		corps_shopinfo )
	SELECT 
	corps_id,
	corps_name,
	corps_notice,
	create_role_id,
	create_role_name,
	corps_level,
	corps_money,
	family_info,
	country_id,
	disband_time,
	corps_status,
	corps_glory,
	war_winnum,
	war_lostnum,
	war_score,
	war_starlevel,
	war_bindnpcid,
	war_bindmoney,
	war_battlestatus,
	war_campid,
	war_battlecity,
	war_battlekey,
	war_welfaregettime,
	corps_roommoney,
	war_battleinfo,
	corps_creattime,
	corps_color,
	corps_shopinfo
	FROM ', SRC_DB, '.UMS_CORPS;' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('update ', DST_DB, '.UMS_CORPS a, 
	(select b.corps_id, sum(b.bid_money) sum_money from ', 
	SRC_DB, '.UMS_CORPS_BID b group by b.corps_id ) t 
	set a.corps_money = a.corps_money+t.sum_money
	where a.corps_id = t.corps_id');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('update ', DST_DB, '.UMS_CORPS a, 
	(select b.corps_id, sum(b.bid_money) sum_money from ', 
	DST_DB, '.UMS_CORPS_BID b group by b.corps_id ) t 
	set a.corps_money = a.corps_money+t.sum_money
	where a.corps_id = t.corps_id');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;
########################
#军团占领npc的补偿
########################

/*
update UMS_CORPS a, (
select owner_id, sum( b.npc_money) sum_money
from FuLS_GameDB.UMS_NPC b, UMS_NPC_STATUS c
where b.npc_id > 0  and b.npc_id = mod(c.npc_id,10000000)
group by owner_id) t
set a.corps_money = a.corps_money + t.sum_money
where a.corps_id = t.owner_id
*/

SET @SqlCmd=CONCAT('update ', DST_DB, '.UMS_CORPS a, (
select owner_id, sum( b.npc_money) sum_money
from UMS_NPC b, ', SRC_DB, '.UMS_NPC_STATUS c
where b.npc_id > 0  and b.npc_id = mod(c.npc_id,10000000)
group by owner_id) t
set a.corps_money = a.corps_money + t.sum_money
where a.corps_id = t.owner_id');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;


SET @SqlCmd=CONCAT('update ', DST_DB, '.UMS_CORPS a, (
select owner_id, sum( b.npc_money) sum_money
from UMS_NPC b, ', DST_DB, '.UMS_NPC_STATUS c
where b.npc_id > 0  and b.npc_id = mod(c.npc_id,10000000)
group by owner_id) t
set a.corps_money = a.corps_money + t.sum_money
where a.corps_id = t.owner_id');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;



SET @SqlCmd=CONCAT('truncate table ', DST_DB, '.UMS_CORPS_BID');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('truncate table ', DST_DB, '.UMS_CORPS_BATTLE ');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;
################################
SELECT "OP WORLD WAR ...";
SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLDWAR 
	SET   totallevel = 0, totalnum = 0, campid = 0, bidmoney = 0, bidcampid = 0, 
	basicaward = 0, bestaward = 0, source0 = 0, source1 = 0, source2 = 0, 
	source3 = 0, source4 = 0, submitmoney = 0 ');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLDCHAR SET   charid = 0, honor = 0, killnum = 0');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;


# 补偿齐天大圣的军团，注意修改补偿金
SET @SqlCmd=CONCAT('SELECT war_winworld*10000000 + 7373, tax_corpsmoney INTO @TMP_INTVAL1, @TMP_INTVAL2',
       		' FROM ', SRC_DB, '.UMS_WORLDGLOBAL ');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt; 

SELECT npc_money into @COMPENSATION from UMS_NPC where npc_id = 0;
SET @SqlCmd=CONCAT('update ', DST_DB, '.UMS_CORPS a, ', SRC_DB, '.UMS_NPC_STATUS b 
	set corps_money = corps_money + @COMPENSATION + @TMP_INTVAL2
	where a.corps_id = b.owner_id and b.npc_id = @TMP_INTVAL1');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('SELECT war_winworld*10000000 + 7373, tax_corpsmoney INTO @TMP_INTVAL1, @TMP_INTVAL2',
       		' FROM ', DST_DB, '.UMS_WORLDGLOBAL ');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt; 

SET @SqlCmd=CONCAT('update ', DST_DB, '.UMS_CORPS a, ', DST_DB, '.UMS_NPC_STATUS b 
	set corps_money = corps_money + @COMPENSATION + @TMP_INTVAL2
	where a.corps_id = b.owner_id and b.npc_id = @TMP_INTVAL1');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('truncate table ', DST_DB, '.UMS_WORLDGLOBAL');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('insert into  ', DST_DB, '.UMS_WORLDGLOBAL ( war_serial, war_status,tax_taxrate, tax_setrate) VALUES( 1, 1, 5, -1 ) ');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('update ', DST_DB, '.UMS_NPC_STATUS set owner_id = 0, occupy_time = 0, welfare_time =0, status = 0');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT( 'UPDATE ',  DST_DB, '.UMS_CORPS SET war_battleinfo=NULL ,  corps_money=corps_money+corps_roommoney, corps_roommoney=0;' );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################	

SELECT "OP UMS_UNION ... ";	

SET @SqlCmd=CONCAT( ' INSERT INTO ', DST_DB, '.UMS_UNION(
		union_id,
		union_head,
		union_name,
		union_notice,
		corps0,			
		corps1,			
		corps2,			
		corps3,			
		corps4,			
		corps5,			
		corps6,			
		corps7,			
		corps8)
	SELECT	
	union_id,
	union_head,
	union_name,
	union_notice,
	corps0,			
	corps1,			
	corps2,			
	corps3,			
	corps4,			
	corps5,			
	corps6,			
	corps7,			
	corps8
	FROM ', SRC_DB, '.UMS_UNION; ');

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################

SELECT "OP UMS_ACHIEVE ... ";	

SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.UMS_ACHIEVE (
		role_id,
		achieve_info)
	SELECT
	role_id,
	achieve_info
	FROM ', SRC_DB, '.UMS_ACHIEVE;' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################

SELECT "OP UMS_ACHIEVE_REMOVED ... ";	

SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.UMS_ACHIEVE_REMOVED (	
		role_id,
		achieve_info )
	SELECT
	role_id,
	achieve_info
	FROM ', SRC_DB, '.UMS_ACHIEVE_REMOVED;');

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################

SELECT "OP UMS_JUDGE_BAD ... ";	

SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.UMS_JUDGE_BAD(
		role_src,
		role_des,
		value,
		timestamp,
		type)
	SELECT
	role_src,
	role_des,
	value,
	timestamp,
	type
	FROM ', SRC_DB, '.UMS_JUDGE_BAD;' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################

SELECT "OP UMS_JUDGE_FAVOR ... ";	

SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.UMS_JUDGE_FAVOR(
		role_src,
		role_des,
		value,
		timestamp,
		type )
	SELECT
	role_src,
	role_des,
	value,
	timestamp,
	type
	FROM ', SRC_DB, '.UMS_JUDGE_FAVOR;');

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;


################################

SELECT "OP UMS_RANK_PLAYER ... ";
SET @SqlCmd=CONCAT( 'TRUNCATE ', DST_DB, '.UMS_RANK_PLAYER;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;


SELECT "OP UMS_RANK_PLAYER_OUTDATED ... ";
SET @SqlCmd=CONCAT( 'TRUNCATE ', DST_DB, '.UMS_RANK_PLAYER_OUTDATED;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;	


################################

SELECT "OP UMS_WORLD ... ";

SET @SqlCmd=CONCAT( 'update ', DST_DB, '.UMS_WORLD SET official = NULL;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;	

SET @SqlCmd=CONCAT( 'update ', DST_DB, '.UMS_WORLD SET official0 = NULL;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;	

SET @SqlCmd=CONCAT( 'update ', DST_DB, '.UMS_WORLD SET official1 = NULL;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;	

SET @SqlCmd=CONCAT( 'update ', DST_DB, '.UMS_WORLD SET official2 = NULL;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;	

SET @SqlCmd=CONCAT( 'update ', DST_DB, '.UMS_WORLD SET official3 = NULL;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;	

SET @SqlCmd=CONCAT( 'update ', DST_DB, '.UMS_WORLD SET official4 = NULL;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;	

SET @SqlCmd=CONCAT( 'update ', DST_DB, '.UMS_WORLD SET official5 = NULL;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;	

SET @SqlCmd=CONCAT( 'update ', DST_DB, '.UMS_WORLD SET official6 = NULL;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;		


WHILE ( WORLD_ID <= 4 ) DO

	SET COUNTRY_ID = 0;

	WHILE( COUNTRY_ID < 5 ) DO


		SET @SqlCmd=CONCAT('SELECT people_num INTO @TMP_INTVAL1 ', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET people_num=people_num+@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('SELECT king_power INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET king_power=0;' );
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('SELECT wizard0 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET wizard0=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND wizard0 <@TMP_INTVAL1',  ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('SELECT wizard1 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET wizard1=@TMP_INTVAL1',  ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND wizard1 < @TMP_INTVAL1',  ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('SELECT wizard2 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET wizard2=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND wizard2 < @TMP_INTVAL1', ';' );
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('SELECT wizard3 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET wizard3=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND wizard3 < @TMP_INTVAL1', ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('SELECT wizard4 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET wizard4=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND wizard4 <@TMP_INTVAL1', ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT wizard5 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET wizard5=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND wizard5 <@TMP_INTVAL1', ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT convalue0 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET convalue0=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND convalue0 <@TMP_INTVAL1', ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT convalue1 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET convalue1=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND convalue1 <@TMP_INTVAL1', ';' );
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT convalue2 INTO @TMP_INTVAL1 ', ' FROM ', SRC_DB, '.UMS_WORLD ', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET convalue2=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND convalue2 <@TMP_INTVAL1',  ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT convalue3 INTO @TMP_INTVAL1',  ' FROM ', SRC_DB, '.UMS_WORLD  ',  ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET convalue3=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND convalue3 <@TMP_INTVAL1', ';' );
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT convalue4 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ',  ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET convalue4=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND convalue4 <@TMP_INTVAL1',  ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT convalue5 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ',  ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET convalue5=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND convalue5 <@TMP_INTVAL1', ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT conwizard0 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ',  ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET conwizard0=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND conwizard0 <@TMP_INTVAL1', ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT conwizard1 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ',  ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET conwizard1=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND conwizard1 <@TMP_INTVAL1', ';' );
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT conwizard2 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ',  ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET conwizard2=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND conwizard2 <@TMP_INTVAL1', ';' );
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT conwizard3 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ',  ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt; 

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET conwizard3=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND conwizard3 <@TMP_INTVAL1', ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT conwizard4 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ',  ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET conwizard4=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND conwizard4 <@TMP_INTVAL1', ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('SELECT conwizard5 INTO @TMP_INTVAL1', ' FROM ', SRC_DB, '.UMS_WORLD ',  ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ';');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLD SET conwizard5=@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ' AND country_id=', COUNTRY_ID, ' AND conwizard5 <@TMP_INTVAL1', ';' );
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;

		SET COUNTRY_ID = COUNTRY_ID + 1;
END WHILE;

SET WORLD_ID = WORLD_ID + 1;
END WHILE;


SELECT "OP UMS_WORLDSTATUS ... ";

-- reset weak_level
SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLDSTATUS SET weak_level=0;');
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt; 

SET WORLD_ID = 1;

WHILE ( WORLD_ID <= 5 ) DO
	
	-- honer_inc
	SET @SqlCmd=CONCAT('SELECT honer_inc INTO @TMP_INTVAL1 ', ' FROM ', SRC_DB, '.UMS_WORLDSTATUS ', ' WHERE world_id= ', WORLD_ID, ';');
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt; 

	SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLDSTATUS SET honer_inc=honer_inc+@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ';');
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt; 

	-- level_score
	SET @SqlCmd=CONCAT('SELECT level_score INTO @TMP_INTVAL1 ', ' FROM ', SRC_DB, '.UMS_WORLDSTATUS ', ' WHERE world_id= ', WORLD_ID, ';');
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt; 

	SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLDSTATUS SET level_score=level_score+@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ';');
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt; 
	
	-- honer_score
	SET @SqlCmd=CONCAT('SELECT honer_score INTO @TMP_INTVAL1 ', ' FROM ', SRC_DB, '.UMS_WORLDSTATUS ', ' WHERE world_id= ', WORLD_ID, ';');
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt; 

	SET @SqlCmd=CONCAT('UPDATE ', DST_DB, '.UMS_WORLDSTATUS SET honer_score=honer_score+@TMP_INTVAL1', ' WHERE world_id= ', WORLD_ID, ';');
	PREPARE stmt FROM @SqlCmd;
	EXECUTE stmt; 

	SET WORLD_ID = WORLD_ID + 1;
	
END WHILE;


SELECT "OP UMS_TASK_GLOBAL_DATA ... ";
SET @SqlCmd=CONCAT( 'TRUNCATE ', DST_DB, '.UMS_TASK_GLOBAL_DATA;');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;	

################################

SELECT "OP GMS_CHARGE_LOG ... ";	

SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.GMS_CHARGE_LOG(
		sequence_id,
		account_id,
		rating_id,
		subject_id,
		audit_flag,
		amount,
		charge_time,
		insert_time)
	SELECT
	sequence_id,
	account_id,
	rating_id,
	subject_id,
	audit_flag,
	amount,
	charge_time,
	insert_time 
	FROM ', SRC_DB, '.GMS_CHARGE_LOG;');

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################

SELECT "OP GMS_CONSUME_LOCKED ... ";	

SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.GMS_CONSUME_LOCKED(
		purchase_id,
		account_id,
		role_id,
		gender_id,
		metier_id,
		role_level,
		rating_id,
		ib_code,
		package_flag,
		item_count,
		pay_time,
		user_ip,
		subject_id,
		audit_flag,
		price,
		discount_price,
		trans_status,
		last_fail_time,
		failure_times,
		game_id,
		detail_id)
	SELECT 
	purchase_id,
	account_id,
	role_id,
	gender_id,
	metier_id,
	role_level,
	rating_id,
	ib_code,
	package_flag,
	item_count,
	pay_time,
	user_ip,
	subject_id,
	audit_flag,
	price,
	discount_price,
	trans_status,
	last_fail_time,
	failure_times,
	game_id,
	detail_id 
	FROM ', SRC_DB, '.GMS_CONSUME_LOCKED;' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################

SELECT "OP GMS_CONSUME_LOG ... ";	

SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.GMS_CONSUME_LOG(
		account_id,
		role_id,
		gender_id,
		metier_id,
		role_level,
		rating_id,
		ib_code,
		package_flag,
		item_count,
		pay_time,
		user_ip,
		subject_id,
		audit_flag,
		price,
		discount_price,
		detail_id,
		game_id)
	SELECT 
	account_id,
	role_id,
	gender_id,
	metier_id,
	role_level,
	rating_id,
	ib_code,
	package_flag,
	item_count,
	pay_time,
	user_ip,
	subject_id,
	audit_flag,
	price,
	discount_price,
	detail_id,
	game_id 
	FROM ', SRC_DB, '.GMS_CONSUME_LOG;' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################	

SELECT "OP GMS_IBITEM_USED_LOCKED ... ";	


##SET @SqlCmd=CONCAT( 'ALTER TABLE ', DST_DB, '.GMS_IBITEM_USED_LOCKED', ' DROP PRIMARY KEY;' );
##PREPARE stmt FROM @SqlCmd;
##EXECUTE stmt;


SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.GMS_IBITEM_USED_LOCKED(
		purchase_id,
		account_id,
		role_id,
		gender_id,
		metier_id,
		role_level,
		use_time,
		last_fail_time,
		failure_times,
		game_id)
	SELECT 
	purchase_id,
	account_id,
	role_id,
	gender_id,
	metier_id,
	role_level,
	use_time,
	last_fail_time,
	failure_times,
	game_id 
	FROM ', SRC_DB, '.GMS_IBITEM_USED_LOCKED;' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

################################

SELECT "OP GMS_IBITEM_USED_LOG ... ";	

##SET @SqlCmd=CONCAT( 'ALTER TABLE ', DST_DB, '.GMS_IBITEM_USED_LOG', ' DROP PRIMARY KEY;' );
##PREPARE stmt FROM @SqlCmd;
##EXECUTE stmt;


SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.GMS_IBITEM_USED_LOG(
		purchase_id,
		account_id,
		role_id,
		gender_id,
		metier_id,
		role_level,
		use_time,
		last_fail_time,
		failure_times,
		game_id) 
	SELECT
	purchase_id,
	account_id,
	role_id,
	gender_id,
	metier_id,
	role_level,
	use_time,
	last_fail_time,
	failure_times,
	game_id 
	FROM ', SRC_DB, '.GMS_IBITEM_USED_LOG;' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;


################################

SELECT "OP UMS_MARRIAGE ... ";	

SELECT "LOG:count of src UMS_MARRIAGE ... ";
SET @SqlCmd=CONCAT('SELECT count(*) from ', SRC_DB, '.UMS_MARRIAGE' );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SELECT "LOG:count of des UMS_MARRIAGE ... ";
SET @SqlCmd=CONCAT('SELECT count(*) from ', DST_DB, '.UMS_MARRIAGE' );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT( 'INSERT INTO ', DST_DB, '.UMS_MARRIAGE(
		groom_id,
		bride_id,
		groom_name,
		bride_name,
		marriage_status,
		wedding_time,
		wedding_timelength,
		camp_sequence,
		camp_id) 
	SELECT
	groom_id,
	bride_id,
	groom_name,
	bride_name,
	marriage_status,
	wedding_time,
	wedding_timelength,
	camp_sequence,
	camp_id 
	FROM ', SRC_DB, '.UMS_MARRIAGE;' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SELECT "LOG:count of new des UMS_MARRIAGE ... ";
SET @SqlCmd=CONCAT('SELECT count(*) from ', DST_DB, '.UMS_MARRIAGE' );
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;


SET @SqlCmd=CONCAT( 'UPDATE ', DST_DB, '.UMS_ROLE r , ', DST_DB, '.UMS_MARRIAGE m ', 
	' SET r.role_marriageid = m.server_sequence WHERE (r.role_id = m.groom_id AND r.gender_id=2) ',
	' OR (r.role_id = m.bride_id and r.gender_id=3); ' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

#############处理元宝交易所#######
SELECT "OP UMS_BOURSE ...";

SET @SqlCmd=CONCAT('DROP TABLE IF EXISTS ', SRC_DB, '.UMS_BOURSE_ACCOUNT_BAK');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;
SET @SqlCmd=CONCAT('create table ', SRC_DB, '.UMS_BOURSE_ACCOUNT_BAK as select * from ', SRC_DB,'.UMS_BOURSE_ACCOUNT');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;
SET @SqlCmd=CONCAT('update ', SRC_DB,'.UMS_BOURSE_ACCOUNT_BAK a, ', DST_DB, '.UMS_BOURSE_ACCOUNT b
	set a.currency_value = a.currency_value + b.currency_value 
	where a.account_id = b.account_id and a.currency_type = b.currency_type');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;
SET @SqlCmd=CONCAT('replace into ', DST_DB, '.UMS_BOURSE_ACCOUNT 
	select account_id, currency_type, currency_value from ',
	SRC_DB, '.UMS_BOURSE_ACCOUNT_BAK');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('select max(bourse_doc_no) into @TMP_INTVAL1 from ', DST_DB, '.UMS_BOURSE_TRADE ');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;
SET @SqlCmd=CONCAT('insert into ', DST_DB, '.UMS_BOURSE_TRADE 
	select bourse_doc_no+@TMP_INTVAL1, bourse_trade_detail, account_id, price, yb_count, 
	trade_yb, trade_kind, trade_type, bourse_date, bourse_tax, role_id, account_game_id from ',
	SRC_DB, '.UMS_BOURSE_TRADE');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('insert into ', DST_DB, '.UMS_BOURSE_TRADE_HISTORY 
	select * from ',SRC_DB, '.UMS_BOURSE_TRADE_HISTORY');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;

SET @SqlCmd=CONCAT('insert into ', DST_DB, '.UMS_BOURSE_LOG 
	select * from ',SRC_DB, '.UMS_BOURSE_LOG');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;


SET @SqlCmd=CONCAT('insert into ', DST_DB, '.UMS_BOURSE_LOG_LOCKED 
	select * from ',SRC_DB, '.UMS_BOURSE_LOG_LOCKED');
PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;


SELECT "OP UMS_ROLE_SWORN ... ";	

SET @SqlCmd=CONCAT('INSERT INTO ', DST_DB, '.UMS_ROLE_SWORN (
		sworn_id,
		sworn_name,
		camp_id,
		sworn_member)
	SELECT 
	sworn_id,
		sworn_name,
		camp_id,
		sworn_member
	FROM ', SRC_DB, '.UMS_ROLE_SWORN;' );

PREPARE stmt FROM @SqlCmd;
EXECUTE stmt;


################################

SELECT "OP UMS_CAMP, do noting ... ";	



################################

SELECT "OP GMS_IBSTORE_PROMOTION, do noting ... ";	


###############################
SELECT "MergeGameDB complete!";
###############################


END;;
DELIMITER ;

