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
/*!50003 CREATE*/  /*!50003 PROCEDURE `ProcPayAmount`(nIbItemID int, nCount int, nPriceUnit int, nPriceValue int, nAccountID int, nRoleID int, nGenderID int, nMetierID int, nRoleLevel int, nPackageFlag int, nUserIP int, nDiscountPrice int,nWorldID int, nServerID int, nGameID int)
BEGIN
	DECLARE nConsumeMoney INTEGER DEFAULT 0;
	DECLARE nUnixTime INTEGER DEFAULT 0;
	DECLARE ullExchangeID BIGINT DEFAULT 0;
	DECLARE nBalance INTEGER DEFAULT 0;

	-- START   TRANSACTION;

	SET nConsumeMoney = nCount * nDiscountPrice;
	SET nUnixTime =  UNIX_TIMESTAMP();

	IF nPriceUnit = 5 THEN
		SELECT golden_yb INTO nBalance FROM UMS_ACCOUNT WHERE account_id=nAccountID;
	ELSEIF nPriceUnit = 4 THEN
		SELECT silver_yb INTO nBalance FROM UMS_ROLE WHERE role_id=nRoleID;	
	END IF;

	IF nBalance < nConsumeMoney THEN
		
		SELECT -1, nBalance ;
		
	ELSE
	
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

	SELECT ullExchangeID, nBalance ;
	
	END IF;
	
END */;;
DELIMITER ;

/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;