-- MySQL dump 10.13  Distrib 5.1.34, for pc-linux-gnu (i686)
--
-- Host: localhost    Database: LogDB
-- ------------------------------------------------------
-- Server version	5.1.34

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
-- Dumping routines for database 'LogDB'
--
/*!50003 DROP PROCEDURE IF EXISTS `QueryCorps` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `QueryCorps`(nCorpsID int, szBeginTime char(32), szEndTime char(32), nPageID int)
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET nWhichTable = 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE corps_id = ',  nCorpsID);

	SET szFields = ' corps_id, op_role_id, op_time, op_type, attr_id, param1, param2, param3 ';

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_CORPS_',  LPAD(nWhichTable, 2, 0));

		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN
				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT  corps_id, op_role_id, FROM_UNIXTIME(op_time), op_type, attr_id, param1, param2, param3 FROM ( ',  statement, ' ) A LIMIT ', (nPageID - 1)* 100, ',  100' );
	SET @gstatement = tmp_SQL;

	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `QueryFamily` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `QueryFamily`(nFamilyID int, szBeginTime char(32), szEndTime char(32), nPageID int)
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET nWhichTable = 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE family_id = ',  nFamilyID);

	SET szFields = ' family_id, op_role_id, op_time, op_type, attr_id, param1, param2, param3 ';

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_FAMILY_',  LPAD(nWhichTable, 2, 0));

		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN
				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT  family_id, op_role_id, FROM_UNIXTIME(op_time), op_type, attr_id, param1, param2, param3 FROM ( ',  statement, ' ) A LIMIT ', (nPageID - 1)* 100, ',  100' );
	SET @gstatement = tmp_SQL;

	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `QueryRoleDeath` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `QueryRoleDeath`(nRoleID int, szBeginTime char(32), szEndTime char(32), nPageID int)
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE role_id = ',  nRoleID);

	SET szFields = ' op_time, line_id, repetion_id, map_id, metier_id, role_level, killer_type, killer_id, killer_metier, killer_level ';

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_KILLED_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;

		IF nHasTable = 0  AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_KILLED_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN
				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT FROM_UNIXTIME(op_time), line_id, repetion_id, map_id, metier_id, role_level, killer_type, killer_id, killer_metier, killer_level FROM ( ',  statement, ' ) A LIMIT ', (nPageID - 1)* 100, ',  100' );
	SET @gstatement = tmp_SQL;

	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `QueryRoleTask` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `QueryRoleTask`(nRoleID int, szBeginTime char(32), szEndTime char(32), nPageID int)
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE role_id = ',  nRoleID);

	SET szFields = ' op_time, line_id, repetion_id, map_id, metier_id, role_level, task_id, op_type ';

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_TASK_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;

		IF nHasTable = 0 AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_TASK_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN
				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT FROM_UNIXTIME(op_time), line_id, repetion_id, map_id, metier_id, role_level, task_id, op_type FROM ( ',  statement, ' ) A LIMIT ', (nPageID - 1)* 100, ',  100' );
	SET @gstatement = tmp_SQL;

	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `QueryRoleLogout` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `QueryRoleLogout`(nRoleID int, szBeginTime char(32), szEndTime char(32), nPageID int)
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE role_id = ',  nRoleID);

	SET szFields = ' line_id, repetion_id, map_id, role_level,  login_ip, login_time, logout_time, leave_reason, save_reason, save_result, kick_reason, kick_code';

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_LOGOUT_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
			
		IF nHasTable = 0  AND nWhichTable != 1  THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_LOGOUT_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;


		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN
				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  logout_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND logout_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND logout_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND logout_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT  line_id, repetion_id, map_id, role_level,  login_ip, from_unixtime(login_time), from_unixtime(logout_time), leave_reason, save_reason, save_result, kick_reason, kick_code FROM ( ',  statement, ' ) A LIMIT ', (nPageID - 1)* 100, ',  100' );
	SET @gstatement = tmp_SQL;

	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ProcGoodsHistory` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcGoodsHistory`(nItemGuID bigint, szBeginTime char(32), szEndTime char(32))
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE item_guid = ',  nItemGuID);

	SET szFields = ' op_time ,   item_num ,  item_total ,  map_id ,  op_param1 , op_param2 , op_param3, item_id, item_guid, op_type, role_id ';

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO
		SET nWhichTable = 1;
		WHILE nWhichTable <= 16 DO
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
			IF nHasTable > 0  THEN
				IF 	nIsHeader = 1 THEN
					IF next_date = begin_date  THEN
						IF next_date = end_date THEN
							SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
						ELSE
							SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
						END IF;
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
					END IF;

					SET nIsHeader = 0;
				ELSEIF next_date = end_date THEN
					SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					SET statement = tmp_SQL;
				ELSE
					SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
					SET statement = tmp_SQL;
				END IF;
			END IF;

			SET nWhichTable  = nWhichTable + 1;
		END WHILE;

		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT FROM_UNIXTIME(op_time),  item_num,  item_total,  map_id,  op_param1, op_param2, op_param3, item_id, item_guid, op_type, role_id ',   ' FROM ( ',  statement, ' ) A ' );
	SET @gstatement = tmp_SQL;

	-- select @gstatement;

	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
	DEALLOCATE PREPARE SQLSA;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ProcGoods2Npc` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcGoods2Npc`(nRoleId int,  nNpcID int, szBeginTime char(32), szEndTime char(32))
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;
	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;

		IF nHasTable = 0  AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN

				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT FROM_UNIXTIME(op_time),  item_id,  item_num, item_guid, op_type, op_param1, op_param3  FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  " AND op_type = 66 AND op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')",  ' AND op_param1 = ',  nNpcID);
					ELSE
						SET statement = CONCAT('SELECT FROM_UNIXTIME(op_time),  item_id,  item_num, item_guid, op_type, op_param1, op_param3  FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  " AND op_type = 66 AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')",  ' AND op_param1= ',  nNpcID);
					END IF;
				ELSE
					SET statement = CONCAT('SELECT FROM_UNIXTIME(op_time),  item_id,  item_num, item_guid, op_type, op_param1, op_param3  FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  ' AND op_type = 66 AND op_param1 = ',  nNpcID);
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT FROM_UNIXTIME(op_time),  item_id,  item_num, item_guid, op_type, op_param1, op_param3  FROM ' , tmp_tablename,  ' WHERE role_id = ',  nRoleID, " AND  op_type = 66 AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')",  ' AND op_param1 = ',  nNpcID);
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT FROM_UNIXTIME(op_time),  item_id,  item_num, item_guid, op_type, op_param1, op_param3  FROM ' , tmp_tablename,  ' WHERE role_id = ',  nRoleID,  ' AND op_type = 66 AND op_param1 = ',  nNpcID)  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET @gstatement = statement;
	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ProcGoodsBetweenUsers` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcGoodsBetweenUsers`(nRoleId int,  nRoleID2 int, szBeginTime char(32), szEndTime char(32))
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;
	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		
		IF nHasTable = 0  AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN

				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT role_id, FROM_UNIXTIME(op_time), line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1 FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')",  'AND op_type in (2, 3, 4, 50, 51, 52)  AND op_param1 = ',  nRoleID2);
					ELSE
						SET statement = CONCAT('SELECT role_id, FROM_UNIXTIME(op_time), line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1  FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')",  ' AND op_type in ( 2, 3, 4, 50, 51, 52) AND op_param1= ',   nRoleID2);
					END IF;
				ELSE
					SET statement = CONCAT('SELECT role_id, FROM_UNIXTIME(op_time), line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1 FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  ' AND op_type in ( 2, 3, 4, 50, 51, 52) AND op_param1 = ',   nRoleID2);
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT role_id, FROM_UNIXTIME(op_time), line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1  FROM ' , tmp_tablename,  ' WHERE role_id = ',  nRoleID, " AND  op_time < UNIX_TIMESTAMP('",  szEndTime, "')",  ' AND op_type in ( 2, 3, 4, 50, 51, 52)  AND op_param1 = ',   nRoleID2);
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT role_id, FROM_UNIXTIME(op_time), line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1  FROM ' , tmp_tablename,  ' WHERE role_id = ',  nRoleID,  ' AND op_type in ( 2, 3, 4, 50, 51, 52) AND op_param1 = ',   nRoleID2)  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET @gstatement = statement;
	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ProcGoodsFromNpc` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcGoodsFromNpc`(nRoleId int,  nNpcID int, szBeginTime char(32), szEndTime char(32))
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;
	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;

		IF nHasTable = 0  AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN

				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT FROM_UNIXTIME(op_time), item_id, item_num, item_guid, op_type, op_param3, op_param2  FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')",  'AND op_type = 12 AND op_param3 = ',  nNpcID);
					ELSE
						SET statement = CONCAT('SELECT FROM_UNIXTIME(op_time), item_id, item_num, item_guid, op_type, op_param3, op_param2  FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')",  ' AND op_type = 12 AND op_param3= ',  nNpcID);
					END IF;
				ELSE
					SET statement = CONCAT('SELECT FROM_UNIXTIME(op_time), item_id, item_num, item_guid, op_type, op_param3, op_param2 FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  ' AND op_type = 12 AND op_param3 = ',  nNpcID);
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT FROM_UNIXTIME(op_time), item_id, item_num, item_guid, op_type, op_param3, op_param2  FROM ' , tmp_tablename,  ' WHERE role_id = ',  nRoleID, " AND  op_time < UNIX_TIMESTAMP('",  szEndTime, "')",  ' AND op_type = 12 AND op_param3 = ',  nNpcID);
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT FROM_UNIXTIME(op_time), item_id, item_num, item_guid, op_type, op_param3, op_param2 FROM ' , tmp_tablename,  ' WHERE role_id = ',  nRoleID,  ' AND op_type = 12 AND op_param3 = ',  nNpcID)  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET @gstatement = statement;
	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ProcOperationDetail` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcOperationDetail`(nRoleId int,  szBeginTime char(32), szEndTime char(32), nBegin int, nPageCount int)
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;
	SET next_date =  begin_date;

	SET statement = CONCAT('SELECT  FROM_UNIXTIME(op_time), line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1, op_param2, op_param3  FROM (');

	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;

		IF nHasTable = 0  AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;

		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN

				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT  op_time, line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1, op_param2, op_param3  FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT  op_time, line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1, op_param2, op_param3  FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID,  " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT  op_time, line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1, op_param2, op_param3  FROM ', tmp_tablename,  ' WHERE role_id = ',  nRoleID);
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT op_time, line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1, op_param2, op_param3 FROM ' , tmp_tablename,  ' WHERE role_id = ',  nRoleID, " AND  op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT op_time, line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1, op_param2, op_param3  FROM ' , tmp_tablename,  ' WHERE role_id = ',  nRoleID)  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;
	SET statement = CONCAT(') LIMIT ', nBegin, ', ', nPageCount );

	SET @gstatement = statement;
	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ProcRoleLevelInfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `ProcRoleLevelInfo`(nRoleId int,  szBeginTime char(32), szEndTime char(32))
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szFields  VARCHAR(1024);
	DECLARE szWhere VARCHAR(1024);

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE role_id = ',  nRoleID, ' AND upgrade_type = 0 ' );
	SET szFields = ' role_level, upgrade_time';

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_UPGRADE_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;

		IF nHasTable = 0  AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_UPGRADE_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN

				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere,  " AND  upgrade_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND upgrade_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere,  " AND upgrade_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  upgrade_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT', szFields, ' FROM ', tmp_tablename,  szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT', szFields, ' FROM ( ',  statement, ' ) A ');
	SET @gstatement = tmp_SQL;

	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `QueryItemUpgrade` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `QueryItemUpgrade`(nRoleId int, nWhichID int,  szBeginTime char(32), szEndTime char(32),  nPageID INT)
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE role_id = ',  nRoleID);

	SET szFields = ' upgrade_time ,   upgrade_type ,  result ,  prev_level ,  curr_level , consume_item1 , consume_item2, item_id, item_guid ';

	IF nWhichID > 1000000 THEN
		SET tmp_SQL = CONCAT(szWhere,' AND item_guid = ', nWhichID);
		SET szWhere = tmp_SQL;
	ELSEIF nWhichID != 0 THEN
		SET tmp_SQL =CONCAT(szWhere, ' AND item_id = ', nWhichID);
		SET szWhere = tmp_SQL;
	END IF;

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ITEM_UPGRADE_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		
		IF nHasTable = 0  AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ITEM_UPGRADE_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN
				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  upgrade_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND upgrade_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND upgrade_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND upgrade_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT FROM_UNIXTIME(upgrade_time),   upgrade_type ,  result ,  prev_level ,  curr_level , consume_item1 , consume_item2, item_id, item_guid  FROM ( ',  statement, ' ) A LIMIT ', (nPageID - 1)* 100, ',  100' );
	SET @gstatement = tmp_SQL;

	-- SELECT statement ;
	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `QueryRoleExp` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `QueryRoleExp`(nRoleId int,  nOpType int, nExpType int,  szBeginTime char(32), szEndTime char(32),  nPageID INT)
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE role_id = ',  nRoleID);

	IF nOpType != 0 THEN
		SET tmp_SQL = CONCAT( szWhere, ' and op_type = ', nOpType);
		SET szWhere = tmp_SQL;

 	END IF;

	SET szFields = ' op_time ,   delta_exp ,  cur_exp ,  map_id ,  op_param1 , op_param2 , op_param3, op_type ';

	SET tmp_SQL = CONCAT(szWhere,' AND exp_type = ', nExpType);
	SET szWhere = tmp_SQL;

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_EXP_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		
		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN
				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT FROM_UNIXTIME(op_time),  delta_exp,  cur_exp,  map_id,  op_param1, op_param2, op_param3, op_type, ', nOpType,  ' FROM ( ',  statement, ' ) A LIMIT ', (nPageID - 1)* 100, ',  100' );
	SET @gstatement = tmp_SQL;

	--  SELECT statement ;
	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `QueryRoleItem` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `QueryRoleItem`(nRoleId int,  nOpType int, nWhichID bigint,  szBeginTime char(32), szEndTime char(32),  nPageID INT)
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE role_id = ',  nRoleID);

	IF nOpType != 0 THEN
		SET tmp_SQL = CONCAT( szWhere, ' and op_type = ', nOpType);
		SET szWhere = tmp_SQL;

 	END IF;

	SET szFields = ' op_time ,   item_num ,  item_total ,  map_id ,  op_param1 , op_param2 , op_param3, item_id, item_guid, op_type ';

	IF nWhichID > 1000000 THEN
		SET tmp_SQL = CONCAT(szWhere,' AND item_guid = ', nWhichID);
		SET szWhere = tmp_SQL;
	ELSEIF nWhichID != 0 THEN
		SET tmp_SQL =CONCAT(szWhere, ' AND item_id = ', nWhichID);
		SET szWhere = tmp_SQL;
	END IF;

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		
		IF nHasTable = 0  AND nWhichTable != 1  THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_ITEM_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN
				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT FROM_UNIXTIME(op_time),  item_num,  item_total,  map_id,  op_param1, op_param2, op_param3, item_id, item_guid, op_type, ', nOpType,  ' FROM ( ',  statement, ' ) A LIMIT ', (nPageID - 1)* 100, ',  100' );
	SET @gstatement = tmp_SQL;

	-- SELECT statement ;
	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `QueryRoleMoney` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `QueryRoleMoney`(nRoleId int,  nOpType int, nMoneyType int,  szBeginTime char(32), szEndTime char(32),  nPageID INT)
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE role_id = ',  nRoleID);

	IF nOpType != 0 THEN
		SET tmp_SQL = CONCAT( szWhere, ' and op_type = ', nOpType);
		SET szWhere = tmp_SQL;

 	END IF;

	SET szFields = ' op_time ,   delta_money ,  cur_money ,  map_id ,  op_param1 , op_param2 , op_param3, op_type ';

	SET tmp_SQL = CONCAT(szWhere,' AND money_type = ', nMoneyType);
	SET szWhere = tmp_SQL;

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_MONEY_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;

		IF nHasTable = 0  AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_MONEY_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN
				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  op_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND op_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	SET tmp_SQL = CONCAT('SELECT FROM_UNIXTIME(op_time),  delta_money,  cur_money,  map_id,  op_param1, op_param2, op_param3, op_type, ', nOpType,  ' FROM ( ',  statement, ' ) A LIMIT ', (nPageID - 1)* 100, ',  100' );
	SET @gstatement = tmp_SQL;

	-- SELECT statement ;
	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;
/*!50003 DROP PROCEDURE IF EXISTS `QueryRoleEquipGhost` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `QueryRoleEquipGhost` (nRoleID int, nWhichID bigint, szBeginTime char(32), szEndTime char(32), nPageID int )
BEGIN
	DECLARE nWhichTable INT;
	DECLARE begin_date   DATE;
	DECLARE end_date    DATE;
	DECLARE tmp_date   DATE;
	DECLARE next_date DATE;
	DECLARE tmp_tablename CHAR(48);
	DECLARE tmp_SQL  VARCHAR(65535);
	DECLARE statement VARCHAR(65535);
	DECLARE nHasTable INT;
	DECLARE nIsHeader INT;
	DECLARE szWhere VARCHAR(1024);
	DECLARE szFields  VARCHAR(1024);

	SET nWhichTable = MOD( nRoleID, 16) + 1;
	SET begin_date = DATE(szBeginTime);
	SET end_date = DATE(szEndTime);
	SET nHasTable = 0;
	SET nIsHeader = 1;

	SET szWhere = CONCAT( ' WHERE role_id = ',  nRoleID);

	SET szFields = ' log_time, item_guid, item_stream, log_id ';

	IF nWhichID > 1000000 THEN
		SET tmp_SQL = CONCAT(szWhere,' AND item_guid = ', nWhichID);
		SET szWhere = tmp_SQL;
	END IF;

	SET next_date =  begin_date;
	WHILE next_date <=  end_date  DO

		SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ITEM_GHOST_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;

		IF nHasTable = 0 AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ITEM_GHOST_',  LPAD(nWhichTable, 2, 0));
			SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;
		END IF;

		IF nHasTable > 0  THEN
			IF 	nIsHeader = 1 THEN
				IF next_date = begin_date  THEN
					IF next_date = end_date THEN
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  log_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND log_time < UNIX_TIMESTAMP('",  szEndTime, "')");
					ELSE
						SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND log_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
					END IF;
				ELSE
					SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere );
				END IF;
				SET nIsHeader = 0;
			ELSEIF next_date = end_date THEN

				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND log_time < UNIX_TIMESTAMP('",  szEndTime, "')");
				SET statement = tmp_SQL;
			ELSE
				SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
				SET statement = tmp_SQL;
			END IF;
		END IF;
		SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
		SET  next_date = tmp_date;
	END WHILE;

	-- 补偿装备选择唯一装备
	IF szEndTime = '' AND nPageID = 0 THEN
		SET tmp_tablename = CONCAT(DATE_FORMAT(begin_date, '%Y%m%d'),  '_LOG_ITEM_GHOST_',  LPAD(nWhichTable, 2, 0));
		SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = database()  AND table_name = tmp_tablename;

		IF nHasTable = 0 AND nWhichTable != 1 THEN
			SET nWhichTable = 1;
			SET tmp_tablename = CONCAT(DATE_FORMAT(begin_date, '%Y%m%d'),  '_LOG_ITEM_GHOST_',  LPAD(nWhichTable, 2, 0));
		END IF;

		SET tmp_SQL = CONCAT('SELECT role_id, FROM_UNIXTIME(log_time),   item_guid, item_stream  FROM  ', tmp_tablename, szWhere, " AND log_time = UNIX_TIMESTAMP ('",  szBeginTime, "')");
		SET @gstatement = tmp_SQL;
	ELSE
		SET tmp_SQL = CONCAT('SELECT FROM_UNIXTIME(log_time), item_guid, item_stream, log_id FROM ( ',  statement, ' ) A ORDER BY log_time desc LIMIT ', (nPageID - 1)* 60, ',  60' );
		SET @gstatement = tmp_SQL;
	END IF;

	PREPARE SQLSA FROM @gstatement ;
	EXECUTE  SQLSA ;
END */;;
DELIMITER ;

DELIMITER $$

DROP PROCEDURE IF EXISTS `LogDB`.`QueryIPLogoutCnt`$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `QueryIPLogoutCnt`(szBeginTime char(32), szEndTime char(32), nIP int unsigned)
BEGIN
    DECLARE nWhichTable INT;
    DECLARE begin_date   DATE;
    DECLARE end_date    DATE;
    DECLARE tmp_date	DATETIME;
    DECLARE nEndTimeT   INT UNSIGNED;
    DECLARE next_date DATE;
    DECLARE tmp_tablename CHAR(48);
    DECLARE tmp_SQL  VARCHAR(65535);
    DECLARE statement VARCHAR(65535);
    DECLARE nHasTable INT;
    DECLARE nIsHeader INT;
    DECLARE szWhere VARCHAR(1024);
    DECLARE szFields  VARCHAR(1024);
    DECLARE nTimeDiff INT;
    
    
    
    SET nTimeDiff = TIME_TO_SEC(TIMEDIFF(szEndTime,szBeginTime));
	
    -- IF nTimeDiff > 3600 THEN
      --  SET tmp_date = DATE_ADD(szBeginTime, INTERVAL 1 HOUR);
    -- ELSE
	SET tmp_date = szEndTime;
    -- END IF;
    SET begin_date = DATE(szBeginTime);
    SET end_date = DATE(tmp_date);
    SET nEndTimeT = UNIX_TIMESTAMP(tmp_date);
    SET szFields = ' role_id, role_level, login_ip, logout_time ';
    SET szWhere = ' WHERE leave_reason>1 AND login_time>0 ';
    IF nIP > 0 THEN
	    SET szWhere = CONCAT(szWhere, ' AND login_ip = ', nIP); 
	END IF;
	

    SET next_date =  begin_date;
    WHILE next_date <=  end_date  DO
        SET tmp_tablename = CONCAT(DATE_FORMAT(next_date, '%Y%m%d'),  '_LOG_ROLE_LOGOUT_01');
        SELECT count(*)  INTO nHasTable   FROM INFORMATION_SCHEMA.TABLES WHERE table_schema = 'LogDB'  AND table_name = tmp_tablename;
        IF nHasTable > 0  THEN
            IF next_date = begin_date  THEN
                IF next_date = end_date THEN
                    SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND  logout_time >= UNIX_TIMESTAMP('", szBeginTime, "') AND logout_time < ", nEndTimeT);
                ELSE
                    SET statement = CONCAT('SELECT', szFields, ' FROM ', tmp_tablename,  szWhere, " AND logout_time >= UNIX_TIMESTAMP('", szBeginTime,"')");
                END IF;
            ELSEIF next_date = end_date THEN

			    SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename,  szWhere, " AND logout_time < ",  nEndTimeT);
			    SET statement = tmp_SQL;
		    ELSE
			    SET tmp_SQL = CONCAT(statement, ' UNION ALL SELECT ', szFields, ' FROM ', tmp_tablename, szWhere )  ;
			    SET statement = tmp_SQL;
		    END IF;
	    END IF;
	    SET  tmp_date = DATE_ADD( next_date,  INTERVAL 1  DAY) ;
	    SET  next_date = tmp_date;
    END WHILE;

    SET tmp_SQL = CONCAT('SELECT login_ip, count(*), count(distinct role_id) FROM ( ',  statement, ' ) A group by login_ip order by count(*) desc ');
    SET @gstatement = tmp_SQL;
    -- SELECT tmp_SQL;
    
    PREPARE SQLSA FROM @gstatement ;
    EXECUTE  SQLSA ;
END$$

DELIMITER ;

DROP PROCEDURE IF EXISTS `LogDB`.`ProcStatMoney`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ProcStatMoney`()
BEGIN
	DECLARE nStockMoney 			BIGINT;
	DECLARE nStockYB					BIGINT;
	DECLARE nMoney						BIGINT;
	DECLARE nBindMoney  			BIGINT;
	DECLARE nStorageMoney 		BIGINT;
	DECLARE nStorgaeBindMoney BIGINT;
	DECLARE nMailMoney 				BIGINT;
	DECLARE nSilverYB					BIGINT;
	DECLARE nGoldenYB 				BIGINT;
	DECLARE tmp_tablename			char(32);
	DECLARE tmp_SQL						VARCHAR(4096);
	DECLARE	nTimeStamp 				INT;


	SELECT sum(if(currency_type=1, IFNULL(currency_value, 0), 0)),   sum(if(currency_type=2, IFNULL(currency_value,0), 0)) 
		INTO nStockYB, nStockMoney
		FROM GameDB.UMS_BOURSE_ACCOUNT;

	
	SELECT sum(money) INTO nMailMoney FROM
	(
		select sum(IFNULL(money_attachment, 0)) money from GameDB.UMS_ROLE_MAIL_01 UNION ALL
		select sum(IFNULL(money_attachment, 0)) money from GameDB.UMS_ROLE_MAIL_02 UNION ALL
		select sum(IFNULL(money_attachment, 0)) money from GameDB.UMS_ROLE_MAIL_03 UNION ALL
		select sum(IFNULL(money_attachment, 0)) money from GameDB.UMS_ROLE_MAIL_04 
	) a ;
	
	
	SELECT sum(IFNULL(silver_yb, 0)), sum(IFNULL(storage_money, 0)), sum(IFNULL(storage_bounded_money, 0)), sum(IFNULL(role_money, 0)), sum(IFNULL(bounded_money, 0)) 
		INTO nSilverYB, nStorageMoney, nStorgaeBindMoney, nMoney, nBindMoney
		FROM GameDB.UMS_ROLE;

	SELECT sum(IFNULL(golden_yb, 0)) INTO nGoldenYB FROM GameDB.UMS_ACCOUNT;

	SET nTimeStamp = UNIX_TIMESTAMP();
	SET tmp_tablename = CONCAT(DATE_FORMAT(curdate(), '%Y%m%d'), '_LOG_MONEY_MONIT_01');

	SET tmp_SQL = CONCAT('INSERT INTO ', tmp_tablename, ' (log_id, log_time, golden_yb, stock_golden_yb, silver_yb, money, storage_money, mail_money, stock_money, bind_money, storage_bind_money) ',
		'VALUES( NULL, ', nTimeStamp, ',' , nGoldenYB, ',', nStockYB, ',', nSilverYB, ',', nMoney, ',', nStorageMoney, ',', nMailMoney, ',', nStockMoney, ',', nBindMoney, ',', nStorgaeBindMoney, ')');
	
	SET @gstatment = tmp_SQL;
	PREPARE SQLSA FROM @gstatment;
	EXECUTE SQLSA;
END;;
DELIMITER ;

/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2010-03-02 17:54:06
