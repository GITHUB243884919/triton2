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
DELIMITER ;

/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2009-09-24  9:37:22

-- 2010-02-28 zhanghl
alter table UMS_ROLE_REMOVED add column country_id  int(11) NULL DEFAULT 0 ;
alter table UMS_ROLE_REMOVED add column role_deletedtime  int(11) NULL DEFAULT NULL ;
alter table UMS_ROLE_REMOVED add column role_protecttime  int(11) NULL DEFAULT 0 ;
alter table UMS_ROLE_REMOVED add column role_vip_flag  int(11) NULL DEFAULT 0 ;
alter table UMS_ROLE_REMOVED add column role_vip_time  int(11) NULL DEFAULT 0 ;

alter table UMS_ROLE_DETAIL_REMOVED add column role_matrix blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_REMOVED add column role_charm blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_REMOVED add column role_guide blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_REMOVED add column role_times blob DEFAULT NULL;

