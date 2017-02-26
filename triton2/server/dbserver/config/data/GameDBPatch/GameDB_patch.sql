-- MySQL dump 10.13  Distrib 5.1.34, for pc-linux-gnu (i686)
--
-- Host: localhost    Database: GameDB
-- ------------------------------------------------------
-- Server version	5.1.34-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


--
-- Table structure for table `STAT_CATALOG_DETAIL`
--


-- 2010-02-28 zhanghl
alter table UMS_ROLE_REMOVED add column country_id  int(11) NULL DEFAULT 0 ;
alter table UMS_ROLE_REMOVED add column role_deletedtime  int(11) NULL DEFAULT NULL ;
alter table UMS_ROLE_REMOVED add column role_protecttime  int(11) NULL DEFAULT 0 ;
alter table UMS_ROLE_REMOVED add column role_vip_flag  int(11) NULL DEFAULT 0 ;
alter table UMS_ROLE_REMOVED add column role_vip_time  int(11) NULL DEFAULT 0 ;

alter table UMS_ROLE_DETAIL_REMOVED add column role_charm blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_REMOVED add column role_guide blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_REMOVED add column role_times blob DEFAULT NULL;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ProcRecoverRole` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcRecoverRole`(nCharID int)
BEGIN
	DECLARE statement VARCHAR(4096);
	DECLARE tablename CHAR(48);
	DECLARE nAccountID int;
	DECLARE nSeqID int;
	DECLARE nExist  int;
	DECLARE nRemoveID int;
	DECLARE nAchieveExist int;

	SET nExist = 0;

	SELECT count(*) INTO nExist FROM UMS_ROLE_REMOVED WHERE role_id = nCharID;

	IF  nExist >0  THEN
		SELECT  account_id, seq_id INTO  nAccountID, nRemoveID FROM UMS_ROLE_REMOVED WHERE role_id = nCharID ORDER BY role_level desc , seq_id desc limit 0, 1;

		SET tablename = CONCAT( 'UMS_ROLE_DETAIL_',  LPAD( MOD( nAccountID, 16), 2, 0));

		SET @statement = CONCAT('INSERT INTO ', tablename,
			' (role_id,  save_bitflag, role_hp, role_mp, role_ap, role_basic, role_skill, role_buff, role_cdgroup, role_package, role_equipment, role_storage, role_pvp, role_title, role_activity, role_talent, role_mw, role_lifeskill, role_statistic, role_task, role_task_package, role_monster_record, role_tele_pos, role_wizard, role_friend, role_repetion, role_client )',
			' SELECT role_id,  save_bitflag, role_hp, role_mp, role_ap, role_basic, role_skill, role_buff, role_cdgroup, role_package, role_equipment, role_storage, role_pvp, role_title, role_activity, role_talent, role_mw, role_lifeskill, role_statistic, role_task, role_task_package, role_monster_record, role_tele_pos, role_wizard, role_friend, role_repetion, role_client ',
			' FROM  UMS_ROLE_DETAIL_REMOVED  WHERE role_id = ', nCharID, ' AND remove_id = ' , nRemoveID, ' ORDER BY seq_id desc limit 0, 1' );

		PREPARE SQLSA FROM @statement ;
		EXECUTE  SQLSA ;

		-- IF ROW_COUNT() = 1 THEN
			-- DELETE FROM UMS_ROLE_DETAIL_REMOVED  WHERE role_id = nCharID;

		INSERT INTO UMS_ROLE
				 ( role_id, role_status, account_id,  is_alive, role_name, gender_id, metier_id, nationality_id, face_type, line_id, map_id,  pos_x, pos_y, role_level, role_expr, role_money,
					bounded_money,  country_id, login_time, online_duration, create_time, role_password, save_status,  save_bitflag, check_sum, role_status_param,role_familyid,role_corpsid,charged_silver_yb,consumed_silver_yb,silver_yb,
					storage_money,storage_bounded_money, role_deletedtime, role_protecttime, role_vip_flag, role_vip_time	)
		SELECT  role_id, role_status, account_id, is_alive,  role_name, gender_id, metier_id, nationality_id, face_type, line_id, map_id, pos_x, pos_y, role_level, role_expr, role_money,
				bounded_money,  country_id, login_time, online_duration, create_time, role_password, save_status,  save_bitflag, check_sum , role_status_param,role_familyid,role_corpsid,charged_silver_yb,consumed_silver_yb,silver_yb,
				storage_money,storage_bounded_money,  role_deletedtime, role_protecttime, role_vip_flag, role_vip_time
			FROM UMS_ROLE_REMOVED
			WHERE role_id = nCharID AND seq_id = nRemoveID;

		-- SELECT ROW_COUNT();
		SELECT role_id, role_name, nationality_id, gender_id, face_type, role_level,metier_id, account_id  FROM UMS_ROLE WHERE role_id = nCharID;

			-- DELETE FROM UMS_ROLE_REMOVED  WHERE role_id = nCharID;
		-- END IF;

		SELECT COUNT(*) INTO nAchieveExist FROM UMS_ACHIEVE_REMOVED WHERE role_id = nCharID;

		IF nAchieveExist > 0 THEN
			INSERT INTO UMS_ACHIEVE( role_id, achieve_info )
			SELECT role_id, achieve_info FROM UMS_ACHIEVE_REMOVED WHERE role_id = nCharID;

			DELETE FROM UMS_ACHIEVE_REMOVED WHERE role_id = nCharID;
		END IF;
	ELSE
		SELECT nExist;
	END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ProcRemoveRole` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcRemoveRole`(nAccountID int, nCharID	int)
BEGIN
	DECLARE statement VARCHAR(4096);
	DECLARE tablename CHAR(48);
	DECLARE nRemoveID int;
	DECLARE nAchieveExist int;

	SET tablename = CONCAT( 'UMS_ROLE_DETAIL_',  LPAD( MOD( nAccountID, 16), 2, 0));

	INSERT INTO UMS_ROLE_REMOVED (
		role_id, role_status, account_id,  is_alive, role_name, gender_id, metier_id, nationality_id, face_type, line_id, map_id,  pos_x, pos_y, role_level, role_expr, role_money,
		bounded_money, country_id, login_time, online_duration, create_time, role_password, save_status,  save_bitflag, check_sum, role_status_param,role_familyid,role_corpsid,charged_silver_yb,consumed_silver_yb,silver_yb,
		storage_money,storage_bounded_money, role_deletedtime, role_protecttime, role_vip_flag, role_vip_time
	)
	SELECT  role_id, role_status, account_id, is_alive,  role_name, gender_id, metier_id, nationality_id, face_type, line_id, map_id, pos_x, pos_y, role_level, role_expr, role_money,
		bounded_money, country_id, login_time, online_duration, create_time, role_password, save_status,  save_bitflag, check_sum, role_status_param,role_familyid,role_corpsid,charged_silver_yb,consumed_silver_yb,silver_yb,
		storage_money,storage_bounded_money, role_deletedtime, role_protecttime, role_vip_flag, role_vip_time
	FROM UMS_ROLE
	WHERE role_id = nCharID;

	SET nRemoveID = LAST_INSERT_ID();

	DELETE FROM UMS_ROLE WHERE role_id = nCharID;

	SET @statement = CONCAT('INSERT INTO 	UMS_ROLE_DETAIL_REMOVED ( ',
			' role_id,  save_bitflag, role_hp, role_mp, role_ap, role_basic, role_skill, role_buff, role_cdgroup, role_package, role_equipment, role_storage, role_pvp, role_title, role_activity, role_talent, role_mw, role_lifeskill, role_statistic, role_task, role_task_package, role_monster_record, role_tele_pos, role_wizard, role_friend, role_repetion, role_client, remove_id) ',
			' SELECT 	role_id,  save_bitflag, role_hp, role_mp, role_ap, role_basic, role_skill, role_buff, role_cdgroup, role_package, role_equipment, role_storage, role_pvp, role_title, role_activity, role_talent, role_mw, role_lifeskill, role_statistic, role_task, role_task_package, role_monster_record, role_tele_pos, role_wizard, role_friend, role_repetion, role_client, ' , nRemoveID ,
			' FROM ', tablename, ' WHERE role_id = ', nCharID);

	PREPARE SQLSA FROM @statement ;
	EXECUTE  SQLSA ;

	SET @statement = CONCAT('DELETE FROM ', tablename, ' WHERE role_id = ', nCharID);
	PREPARE SQLSA FROM @statement ;
	EXECUTE  SQLSA ;

	SELECT COUNT(*) INTO nAchieveExist FROM UMS_ACHIEVE WHERE role_id = nCharID;

	IF nAchieveExist > 0 THEN
		INSERT INTO UMS_ACHIEVE_REMOVED( role_id, achieve_info )
		SELECT role_id, achieve_info FROM UMS_ACHIEVE
		WHERE role_id = nCharID;

		DELETE FROM UMS_ACHIEVE WHERE role_id = nCharID;
	END IF;

	select ROW_COUNT();
END */;;
DELIMITER ;

/*!50003 DROP PROCEDURE IF EXISTS `ProcAccountVerify3rd` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcAccountVerify3rd`(nVerifiedByErating int, nAccountID int, szUserName varchar(64), szUserPasswd varchar(32),  nUserClass int, nUserPoint int, nAdultFlag int, nUserFlag int, nUrlID int)
BEGIN
	DECLARE tPsw CHAR(32);
	DECLARE tRet INT;
	DECLARE tExist INT;
	DECLARE tStatus INT;

	--    Erating 认证通过
	IF nVerifiedByErating  > 0 THEN

		SELECT count(*) into tExist from UMS_ACCOUNT WHERE account_id = nAccountID;
		SET tStatus=0;
		IF tExist = 0 THEN
			INSERT INTO UMS_ACCOUNT ( account_id, user_name, account_status, passwd, user_class, user_point, adult_flag, user_flag , url_id)
			VALUES ( nAccountID, szUserName, 0,szUserPasswd,  nUserClass, nUserPoint, nAdultFlag, nUserFlag , nUrlID);
		ELSE
			-- 更新本地数据
			UPDATE UMS_ACCOUNT SET passwd = szUserPasswd, user_class = nUserClass, user_point = nUserPoint, adult_flag = nAdultFlag, user_flag = nUserFlag, url_id = if(url_id = 0, nUrlID, url_id)
			WHERE account_id = nAccountID;

			SELECT account_status into tStatus from UMS_ACCOUNT WHERE account_id = nAccountID;
		END IF;

		SELECT nAccountID, tStatus ;
	ELSE
		-- 没有经过ERating认证?直接进行本地验证
		SELECT   passwd, account_id, account_status, user_class, user_point, adult_flag, user_flag, account_status
			INTO  tPsw,   tRet,  tStatus, nUserClass, nUserPoint, nAdultFlag, nUserFlag, tStatus
			FROM    UMS_ACCOUNT WHERE  user_name = szUserName;

		IF FOUND_ROWS() = 0 THEN
			SELECT -21004 ;
		ELSEIF  tPsw <> szUserPasswd THEN
			SELECT -21005;
		ELSEIF tStatus  = 1 THEN
			SELECT -21007;
		ELSE
			SELECT tRet,  tStatus, nUserClass, nUserPoint, nAdultFlag, nUserFlag ;
		END IF;
	END IF;
END */;;

--2010.3.26
 alter table UMS_ROLE ADD  role_marriageid int(11) default 0;
 

DROP TABLE IF EXISTS `UMS_MARRIAGE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_MARRIAGE` (
  `server_sequence` int(11) NOT NULL AUTO_INCREMENT,
  `groom_id` int(11) NOT NULL,
  `bride_id` int(11) NOT NULL,
  `groom_name` varchar(32) NOT NULL,
  `bride_name` varchar(32) NOT NULL,
  `marriage_status` int(11) DEFAULT NULL,
  `wedding_time` int(11) DEFAULT NULL,
  `wedding_timelength` int(11) DEFAULT NULL,
  `camp_sequence` int(11) DEFAULT NULL,
  `camp_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`server_sequence`)
) ENGINE=MyISAM AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;



DROP TABLE IF EXISTS `UMS_CAMP`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_CAMP` (
  `campid` int(11) NOT NULL,
  `marriednum` int(11) DEFAULT NULL,
  PRIMARY KEY (`campid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

INSERT INTO UMS_WORLD( world_id, country_id, official ) VALUES( 1, 4, NULL );
INSERT INTO UMS_WORLD( world_id, country_id, official ) VALUES( 2, 4, NULL );
INSERT INTO UMS_WORLD( world_id, country_id, official ) VALUES( 3, 4, NULL );
INSERT INTO UMS_WORLD( world_id, country_id, official ) VALUES( 4, 4, NULL );

create unique index world_country on UMS_WORLD(world_id,country_id);

DROP TABLE IF EXISTS `GMS_IBSTORE_PROMOTION`;
create table GMS_IBSTORE_PROMOTION
(
	goods_id int primary key,
	locked_count int not null default 0,
	elapsed_time int not null default 0,
	update_time timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

alter table UMS_WORLD modify column version int not null default 1;

alter table GMS_CONSUME_LOG add column detail_id bigint default 0;
alter table GMS_CONSUME_LOG add column game_id int default 4;
update GMS_CONSUME_LOG set detail_id = rating_id * 10000000000 + purchase_id;
create index IDX_CONSUME_LOG on GMS_CONSUME_LOG ( game_id, detail_id);

alter table GMS_CONSUME_LOCKED add column game_id int default 4;
alter table GMS_CONSUME_LOCKED add column detail_id bigint default 0;
update GMS_CONSUME_LOCKED set detail_id = rating_id * 10000000000 + purchase_id;

alter table GMS_IBITEM_USED_LOCKED add column game_id int default 4;
alter table GMS_IBITEM_USED_LOG add column game_id int default 4;
alter table GMS_IBITEM_USED_LOCKED modify column failure_times int not null default 0;

/*!50003 DROP PROCEDURE IF EXISTS `ProcPayAmount` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcPayAmount`(nIbItemID int, nCount int, nPriceUnit int, nPriceValue int, nAccountID int, nRoleID int, nGenderID int, nMetierID int, nRoleLevel int, nPackageFlag int, nUserIP int, nDiscountPrice int,nWorldID int, nServerID int, nGameID int)
BEGIN
	DECLARE nConsumeMoney INTEGER DEFAULT 0;
	DECLARE nUnixTime INTEGER DEFAULT 0;
	DECLARE ullExchangeID BIGINT DEFAULT 0;

	-- START   TRANSACTION;

	SET nConsumeMoney = nCount * nDiscountPrice;
	SET nUnixTime =  UNIX_TIMESTAMP();

	IF nPriceUnit = 5 THEN
		UPDATE UMS_ACCOUNT SET golden_yb = golden_yb - nConsumeMoney , consumed_golden_yb = consumed_golden_yb + nConsumeMoney WHERE account_id = nAccountID;
	ELSEIF nPriceUnit = 4 THEN
		UPDATE UMS_ROLE SET silver_yb = silver_yb - nConsumeMoney , consumed_silver_yb = consumed_silver_yb + nConsumeMoney WHERE role_id = nRoleID;
	END IF;

	INSERT INTO GMS_CONSUME_LOG ( purchase_id, account_id, role_id,  gender_id,  metier_id,  role_level,   ib_code, package_flag, item_count, pay_time,  user_ip, subject_id,  audit_flag,  price, discount_price, rating_id, game_id)
		VALUES (NULL,  nAccountID, nRoleID, nGenderID, nMetierID, nRoleLevel,  nIbItemID, nPackageFlag, nCount,  nUnixTime, nUserIP, nPriceUnit , 1, nPriceValue, nDiscountPrice, nWorldID, nGameID);

	SET ullExchangeID = LAST_INSERT_ID();

	UPDATE GMS_CONSUME_LOG SET detail_id =  nWorldID * 10000000000 + ullExchangeID WHERE purchase_id = ullExchangeID AND rating_id = nWorldID;
	
	INSERT INTO GMS_CONSUME_LOCKED ( purchase_id, account_id, role_id,  gender_id,  metier_id,  role_level,   ib_code, package_flag, item_count, pay_time,  user_ip, subject_id,  audit_flag,  price, discount_price, rating_id, game_id, detail_id)
		VALUES (ullExchangeID,  nAccountID, nRoleID, nGenderID, nMetierID, nRoleLevel,  nIbItemID, nPackageFlag, nCount,  nUnixTime, nUserIP, nPriceUnit , 1, nPriceValue, nDiscountPrice, nWorldID, nGameID, nWorldID * 10000000000 + ullExchangeID);

	-- COMMIT;

	SELECT ullExchangeID;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

/*!50003 DROP FUNCTION IF EXISTS `ProcAccountVerify` */;
/*!50003 DROP PROCEDURE IF EXISTS `ProcAccountVerify2nd` */;


/*!50003 DROP PROCEDURE IF EXISTS `ProcUseIbItem` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcUseIbItem`(ullPurchaseID bigint )
BEGIN
	INSERT INTO GMS_IBITEM_USED_LOG (purchase_id, account_id, role_id, gender_id, metier_id, role_level, use_time, last_fail_time, failure_times, game_id)
		SELECT purchase_id, account_id, role_id, gender_id, metier_id, role_level, use_time, last_fail_time, failure_times, game_id 
		FROM GMS_IBITEM_USED_LOCKED WHERE purchase_id = ullPurchaseID;

	DELETE 	FROM GMS_IBITEM_USED_LOCKED WHERE purchase_id = ullPurchaseID;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

alter table UMS_MARRIAGE add column marriage_time  int default 0;

DROP TABLE IF EXISTS `UMS_WORLDWAR`;
CREATE TABLE `UMS_WORLDWAR` (
  `world_id` int(11) NOT NULL,
  `totallevel` int(11) NOT NULL DEFAULT '0',
  `totalnum` int(11) NOT NULL DEFAULT '0',
  `campid` int(11) NOT NULL DEFAULT '0',
  `bidmoney` int(11) NOT NULL DEFAULT '0',
  `bidcampid` int(11) NOT NULL DEFAULT '0',
  `basicaward` int(11) NOT NULL DEFAULT '0',
  `bestaward` int(11) NOT NULL DEFAULT '0',
  `source0` int(11) NOT NULL DEFAULT '0',
  `source1` int(11) NOT NULL DEFAULT '0',
  `source2` int(11) NOT NULL DEFAULT '0',
  `source3` int(11) NOT NULL DEFAULT '0',
  `source4` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`world_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO UMS_WORLDWAR( world_id ) VALUES( 1 );
INSERT INTO UMS_WORLDWAR( world_id ) VALUES( 2 );
INSERT INTO UMS_WORLDWAR( world_id ) VALUES( 3 );
INSERT INTO UMS_WORLDWAR( world_id ) VALUES( 4 );

DROP TABLE IF EXISTS `UMS_WORLDCHAR`;
CREATE TABLE `UMS_WORLDCHAR` (
  `world_id` int(11) NOT NULL,
  `charid` int(11) NOT NULL DEFAULT '0',
  `honor` int(11) NOT NULL DEFAULT '0',
  `killnum` int(11) NOT NULL DEFAULT '0',
  `rank` int(11) NOT NULL DEFAULT '0',
  `metier` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 0, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 1, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 1, 2, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 0, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 1, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 2, 2, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 0, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 1, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 3, 2, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 1 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 2 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 3 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 4 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 5 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 0, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 1, 6 );
INSERT INTO UMS_WORLDCHAR( world_id, rank, metier ) VALUES( 4, 2, 6 );

DROP TABLE IF EXISTS `UMS_WORLDGLOBAL`;
CREATE TABLE `UMS_WORLDGLOBAL` (
  `war_serial` int(11) NOT NULL DEFAULT '0',
  `war_status` int(11) NOT NULL,
  `war_titletime` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO UMS_WORLDGLOBAL VALUES( 1, 0 );