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
-- Dumping routines for database 'GameDB'
--
/*!50003 DROP FUNCTION IF EXISTS `FuncChargeAmount` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 FUNCTION `FuncChargeAmount`(nSequenceID bigint,
                                          nAccountID int,
                                          nSubjectID int,
                                          nAmount int,
                                          nAuditFlag int,
                                          nChargeTime int
) RETURNS int(32)
BEGIN
	DECLARE nTotalAmount    INT;
	DECLARE nModifyAmount   INT;
	DECLARE CONTINUE HANDLER FOR 1329 BEGIN END;          
	DECLARE CONTINUE HANDLER FOR SQLSTATE "23000" BEGIN END; 
	SET nTotalAmount   = 0;
  	
  	
 	 
	
	SELECT account_id into nTotalAmount from UMS_ACCOUNT where account_id = nAccountID;
	IF FOUND_ROWS() <= 0 THEN
		return -13;
	END IF;
	SET nTotalAmount   = 0;
	
	IF nAuditFlag = 1 THEN
		INSERT INTO GMS_CHARGE_LOG ( sequence_id, account_id, subject_id, amount, audit_flag, charge_time, insert_time  )
			VALUE (nSequenceID, nAccountID, nSubjectID, nAmount, nAuditFlag, nChargeTime, sysdate() );
		IF ROW_COUNT() = -1  THEN
			RETURN -11;
		END IF;
		IF nSubjectID = 5 THEN
			UPDATE UMS_ACCOUNT
				SET golden_yb = golden_yb + nAmount, charged_golden_yb   = charged_golden_yb + nAmount
				WHERE account_id = nAccountID;
			SELECT charged_golden_yb INTO nTotalAmount FROM UMS_ACCOUNT WHERE account_id = nAccountID;
		END IF;
		IF nSubjectID = 4 THEN
			UPDATE UMS_ACCOUNT
				SET silver_yb = silver_yb + nAmount, charged_silver_yb   = charged_silver_yb + nAmount
				WHERE account_id = nAccountID;
			SELECT charged_sliver_yb INTO nTotalAmount FROM UMS_ACCOUNT WHERE account_id = nAccountID;
		END IF;
	END IF;
	
	IF nAuditFlag = 2 THEN
		INSERT INTO GMS_CHARGE_LOG ( sequence_id, account_id, subject_id, amount, audit_flag, charge_time, insert_time)
			VALUE ( nSequenceID, nAccountID, nSubjectID, nAmount, nAuditFlag, nChargeTime, sysdate());
		IF ROW_COUNT() = -1 THEN
			RETURN -11;
		END IF;
		IF nSubjectID = 5 THEN
			UPDATE UMS_ACCOUNT
				SET golden_yb = golden_yb + nAmount, charged_golden_yb = charged_golden_yb + nAmount
				WHERE account_id = nAccountID;
		END IF;
		IF nSubjectID = 4 THEN
			UPDATE UMS_ACCOUNT
				SET silver_yb = silver_yb + nAmount, charged_silver_yb = charged_silver_yb + nAmount
				WHERE account_id = nAccountID;
		END IF;
	END IF;
	
	IF nAuditFlag = 3 THEN                                  
		SELECT amount INTO nModifyAmount FROM GMS_CHARGE_LOG
			WHERE sequence_id = nSequenceID AND subject_id = nSubjectID AND account_id = nAccountID;
		
		IF FOUND_ROWS() = 0 THEN
			INSERT INTO GMS_CHARGE_LOG ( sequence_id, account_id, subject_id, amount, audit_flag, charge_time, insert_time)
				VALUE ( nSequenceID, nAccountID, nSubjectID, nAmount, nAuditFlag, nChargeTime, sysdate());
			IF ROW_COUNT() = -1 THEN
				RETURN -11;
			END IF;
			IF nSubjectID = 5 THEN
        			UPDATE UMS_ACCOUNT
					SET golden_yb = golden_yb + nAmount, charged_golden_yb = charged_golden_yb + nAmount
		 			WHERE account_id = nAccountID;
			END IF;
			IF nSubjectID = 4 THEN
        			UPDATE UMS_ACCOUNT
					SET silver_yb = silver_yb + nAmount, charged_silver_yb = charged_silver_yb + nAmount
        				WHERE account_id = nAccountID;
			END IF;
		ELSE 
			UPDATE GMS_CHARGE_LOG
				SET amount = nAmount
				WHERE sequence_id = nSequenceID AND subject_id = nSubjectID AND account_id = nAccountID;
			IF nSubjectID = 5 THEN
				UPDATE UMS_ACCOUNT
					SET golden_yb = golden_yb + (nAmount - nModifyAmount),   charged_golden_yb = charged_golden_yb + ( nAmount - nModifyAmount)
					WHERE account_id = nAccountID;
			END IF;
			IF nSubjectID = 4 THEN
				UPDATE UMS_ACCOUNT
					SET silver_yb    = silver_yb + (nAmount - nModifyAmount),   charged_silver_yb   = charged_silver_yb + (nAmount - nModifyAmount)
        				WHERE account_id = nAccountID;
			END IF;
		END IF;
	END IF;
  
	IF nAuditFlag = 4 THEN                                            
		SELECT amount INTO nModifyAmount FROM GMS_CHARGE_LOG
			WHERE sequence_id = nSequenceID AND subject_id = nSubjectID AND account_id = nAccountID;
		IF FOUND_ROWS() = 0 THEN
			RETURN -12;
		END IF;
		DELETE FROM GMS_CHARGE_LOG WHERE sequence_id = nSequenceID ;
		IF nSubjectID = 5 THEN 
			UPDATE UMS_ACCOUNT
				SET golden_yb    = golden_yb  - nModifyAmount, charged_golden_yb = charged_golden_yb - nModifyAmount
				WHERE account_id = nAccountID;
		END IF;
		IF nSubjectID = 4 THEN
			UPDATE UMS_ACCOUNT
				SET silver_yb    = silver_yb - nModifyAmount,  charged_silver_yb = charged_silver_yb - nModifyAmount
				WHERE account_id = nAccountID;
		END IF;
	END IF;
	
	RETURN nTotalAmount;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `FuncInsOrUpdAcct` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 FUNCTION `FuncInsOrUpdAcct`(nAccountID int, szUserName char(32), szPasswd char(32), nUserType int, nUserClass int, nUserPoint int, nPromoterID int, nUserFlag int) RETURNS int(32)
BEGIN
DECLARE CONTINUE HANDLER FOR SQLSTATE "23000" BEGIN
	update UMS_ACCOUNT set user_name  = szUserName,  passwd = szPasswd
		where account_id = nAccountID;
		RETURN ROW_COUNT();
                                                    END;
	insert into UMS_ACCOUNT ( account_id, user_name, passwd, account_status)
		values(nAccountID, szUserName, szPasswd,  0);
		
	RETURN ROW_COUNT();
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `FuncUserIbPayRes` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 FUNCTION `FuncUserIbPayRes`(nWorldID int, nPurcharseID int, nAccountID int, nTotalPay int, nPriceUnit int, nAuditFlag int) RETURNS int(11)
BEGIN
	DECLARE  nTotalConsumedMoney INT;
	DECLARE  nLockedConsumedMoney INT;
	SET nTotalConsumedMoney = 0;
	SELECT IF(nPriceUnit = 5, consumed_golden_yb, IF ( nPriceUnit = 4, consumed_silver_yb, 0) ) INTO nTotalConsumedMoney FROM UMS_ACCOUNT WHERE account_id = nAccountID;
	IF FOUND_ROWS() = 0 THEN
		return -13;
	END IF;

	DELETE FROM GMS_CONSUME_LOCKED WHERE rating_id = nWorldID and purchase_id = nPurcharseID ;

	IF nPriceUnit  = 4  THEN
		return  nTotalConsumedMoney;
	END IF;

	IF nAuditFlag = 1 THEN
		IF nTotalConsumedMoney != nTotalPay  THEN
			return -16;
		END IF;
	END IF;

	return  nTotalConsumedMoney;
END */;;
DELIMITER ;

/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

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
/*!50003 CREATE*/  /*!50003 PROCEDURE `ProcAccountVerify3rd`(nVerifiedByErating int, nAccountID int, szUserName varchar(64), szUserPasswd varchar(32),  nUserClass int, nUserPoint int, nAdultFlag int, nUserFlag int, nUrlID int, nIP int unsigned)
BEGIN
	DECLARE tPsw CHAR(32);
	DECLARE tRet INT;
	DECLARE tExist INT;
	DECLARE tStatus INT;
	DECLARE szBindedIP CHAR(255);
	DECLARE ipv4 CHAR(20);
	DECLARE ipv3 CHAR(20);
	DECLARE ipv2 CHAR(20);
	DECLARE A CHAR(4);
	DECLARE B CHAR(4);
	DECLARE C CHAR(4);
	DECLARE D CHAR(4);

	--    Erating 认证通过
	IF nVerifiedByErating  > 0 THEN

		SELECT count(*) into tExist from UMS_ACCOUNT WHERE account_id = nAccountID;
		SET tStatus=0;
		IF tExist = 0 THEN
			INSERT INTO UMS_ACCOUNT ( account_id, user_name, account_status, passwd, user_class, user_point, adult_flag, user_flag , url_id)
			VALUES ( nAccountID, szUserName, 0,szUserPasswd,  nUserClass, nUserPoint, nAdultFlag, nUserFlag , nUrlID);
		ELSE
			-- 更新玩家的信息
			UPDATE UMS_ACCOUNT SET passwd = szUserPasswd, user_class = nUserClass, user_point = nUserPoint, adult_flag = nAdultFlag, user_flag = nUserFlag
			WHERE account_id = nAccountID;

			SELECT account_status, binded_ip INTO tStatus, szBindedIP from UMS_ACCOUNT WHERE account_id = nAccountID;
		END IF;

		SET ipv4=inet_ntoa(nIP);	

		IF tStatus & 0x10000000 = 0x10000000 AND szBindedIP != '*' THEN	
			-- 分解IP地址
			SELECT SUBSTRING_INDEX( ipv4 , '.', 1 ), SUBSTRING_INDEX(SUBSTRING_INDEX( ipv4 , '.', 2 ),'.',-1),
			SUBSTRING_INDEX(SUBSTRING_INDEX( ipv4 , '.', -2 ),'.',1), SUBSTRING_INDEX( ipv4 , '.', -1 ) 
			INTO 
			A, B, C, D;

			SET ipv3=CONCAT(A, '.', B, '.', C, '.*');
			SET ipv2=CONCAT(A, '.', B, '.*.*');

			-- 判断当前IP是否在IP列表中
			IF FIND_IN_SET(ipv4, szBindedIP) = 0 AND FIND_IN_SET(ipv3, szBindedIP) = 0 AND FIND_IN_SET(ipv2, szBindedIP) = 0 THEN
				SELECT -21022;
			ELSE
				SELECT nAccountID, tStatus ;
			END IF;
		ELSE
			SELECT nAccountID, tStatus ;
		END IF;
	ELSE
		-- 本地认证
		SELECT   passwd, account_id, account_status, user_class, user_point, adult_flag, user_flag, account_status, binded_ip
			INTO  tPsw,   tRet,  tStatus, nUserClass, nUserPoint, nAdultFlag, nUserFlag, tStatus, szBindedIP
			FROM    UMS_ACCOUNT WHERE  user_name = szUserName;

		IF FOUND_ROWS() = 0 THEN
			SELECT -21004 ;
		ELSEIF  tPsw <> szUserPasswd THEN
			SELECT -21005;
		ELSEIF tStatus  = 1 THEN
			SELECT -21007;
		ELSE

			SET ipv4=inet_ntoa(nIP);	
			IF tStatus & 0x10000000 = 0x10000000 AND szBindedIP != '*' THEN	
				-- 分解IP地址
				SELECT SUBSTRING_INDEX( ipv4 , '.', 1 ), SUBSTRING_INDEX(SUBSTRING_INDEX( ipv4 , '.', 2 ),'.',-1),
				SUBSTRING_INDEX(SUBSTRING_INDEX( ipv4 , '.', -2 ),'.',1), SUBSTRING_INDEX( ipv4 , '.', -1 ) 
				INTO 
				A, B, C, D;
	
				SET ipv3=CONCAT(A, '.', B, '.', C, '.*');
				SET ipv2=CONCAT(A, '.', B, '.*.*');
	
				-- 判断当前IP是否在IP列表中
				IF FIND_IN_SET(ipv4, szBindedIP) = 0 AND FIND_IN_SET(ipv3, szBindedIP) = 0 AND FIND_IN_SET(ipv2, szBindedIP) = 0 THEN
					SELECT -21022;
				ELSE
					SELECT tRet,  tStatus, nUserClass, nUserPoint, nAdultFlag, nUserFlag ;
				END IF;
			ELSE
				SELECT tRet,  tStatus, nUserClass, nUserPoint, nAdultFlag, nUserFlag ;
			END IF;
		END IF;
	END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

/*!50003 DROP FUNCTION IF EXISTS `ProcCreateRole` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 FUNCTION `ProcCreateRole`(vRoleID INT, vAccountID INT, vCharName CHAR(32), vGender INT, vMetier INT, vNationality INT, vFaceType INT, vLineID INT, vMapID INT, vCharLevel INT, vExp INT ) RETURNS int(11)
BEGIN
	DECLARE tRoleNumber INT;
	DECLARE nTaleID INT;
	DECLARE EXIT HANDLER FOR 1062 RETURN -12;

	SELECT   COUNT(*) INTO  tRoleNumber
		FROM  UMS_ROLE
		WHERE  account_id = vAccountID;

	SET nTaleID = MOD( vAccountID, 16);

	IF tRoleNumber = 4  THEN
  		RETURN -11;
	END IF;


	INSERT INTO UMS_ROLE(  role_id,  account_id,  role_name,  gender_id,  metier_id,  nationality_id,  face_type,  line_id,  map_id,  role_level,  role_expr, create_time )
		VALUES (  vRoleID,  vAccountID,  vCharName,  vGender,  vMetier,  vNationality,  vFaceType,  vLineID,  vMapID,  vCharLevel,  vExp,  sysdate() );

	
	IF nTaleID = 0 THEN
		INSERT INTO UMS_ROLE_DETAIL1_00 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 1 THEN
		INSERT INTO UMS_ROLE_DETAIL1_01 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 2 THEN
		INSERT INTO UMS_ROLE_DETAIL1_02 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 3 THEN
		INSERT INTO UMS_ROLE_DETAIL1_03 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 4 THEN
		INSERT INTO UMS_ROLE_DETAIL1_04 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 5 THEN
		INSERT INTO UMS_ROLE_DETAIL1_05 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 6 THEN
		INSERT INTO UMS_ROLE_DETAIL1_06 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 7 THEN
		INSERT INTO UMS_ROLE_DETAIL1_07 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 8 THEN
		INSERT INTO UMS_ROLE_DETAIL1_08 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 9 THEN
		INSERT INTO UMS_ROLE_DETAIL1_09 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 10 THEN
		INSERT INTO UMS_ROLE_DETAIL1_10 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 11 THEN
		INSERT INTO UMS_ROLE_DETAIL1_11 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 12 THEN
		INSERT INTO UMS_ROLE_DETAIL1_12 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 13 THEN
		INSERT INTO UMS_ROLE_DETAIL1_13 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 14 THEN
		INSERT INTO UMS_ROLE_DETAIL1_14 ( role_id ) VALUES ( vRoleID );
	ELSEIF nTaleID = 15 THEN
		INSERT INTO UMS_ROLE_DETAIL1_15 ( role_id ) VALUES ( vRoleID );
	END IF;
	

	RETURN vRoleID;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `BackExpiredMail_04` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 PROCEDURE `BackExpiredMail_04`()
begin
	declare  tablenum       	int(0);
	declare  destid  		int(0);
	declare srcid      		int(0);
	declare i                		int(0);
	declare mailtype	        int(0 );
	declare mailstatus           int(0);
	declare sendroleid           int(0);
	declare receveroleid       int(0);
	declare mailtitle       	 	varchar( 100 );
	declare mailbody    	  	varchar( 1000);
	declare recevername	varchar(30);
	declare sendrolename     varchar(30);
	declare itemnum		int( 0 );
	declare itemattachment  blob;
	declare money			int( 0 );
	declare sendtime             int( 0 );
	declare expiredtime        int( 0 );
	declare mark 			int(1);
	declare sqltext  		varchar(500);
	-- 淇敼瀛樺偍杩囩▼  杩囨湡浠樿垂閭欢鍙互閫€淇★細mail_type&(4+2)=0  鏀逛负 mail_type&(2+1)!=0
        declare cur   cursor  for   select  sender_role_id, recver_role_id, mail_title, mail_body, item_number, item_attachment, money_attachment , recver_role_name,sender_role_name from  UMS_ROLE_MAIL_04    where
	UNIX_TIMESTAMP( )  >=  expired_time and ( item_number >0 or money_attachment > 0 )  and mail_type&(2+1)!=0 and  mail_type&1=1 and mail_status&4=0;
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET mark = 0;
	set mark = 1;
	open cur;
  	REPEAT
 	 	FETCH cur INTO sendroleid, receveroleid, mailtitle,mailbody,itemnum,itemattachment, money,recevername,sendrolename ;
		if mark then
	 		set sendtime = UNIX_TIMESTAMP();
			set expiredtime = sendtime + 3*86400;
			insert into   UMS_ROLE_MAIL_04( recver_role_id, sender_role_name,mail_title,mail_type,mail_status,mail_body,item_attachment,send_time,expired_time, money_attachment,post_recver, sender_role_id,item_number,recver_role_name )
				 values(  sendroleid,recevername,mailtitle,4,3,mailbody,itemattachment,sendtime,expiredtime,money,0,receveroleid,itemnum,sendrolename );
		end if;
     UNTIL  mark = 0
     END REPEAT;
    CLOSE cur;
    delete from  UMS_ROLE_MAIL_04  where UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 )  and  mail_type&(2+1)!=0 and  mail_type&1=1 and mail_status&4=0;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `BackExpiredMail_01` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 PROCEDURE `BackExpiredMail_01`()
begin
	declare  tablenum       	int(0);
	declare  destid  		int(0);
	declare srcid      		int(0);
	declare i                		int(0);
	declare mailtype	        int(0 );
	declare mailstatus           int(0);
	declare sendroleid           int(0);
	declare receveroleid       int(0);
	declare mailtitle       	 	varchar( 100 );
	declare mailbody    	  	varchar( 1000);
	declare recevername	varchar(30);
	declare sendrolename     varchar(30);
	declare itemnum		int( 0 );
	declare itemattachment  blob;
	declare money			int( 0 );
	declare sendtime             int( 0 );
	declare expiredtime        int( 0 );
	declare mark 			int(1);
	declare sqltext  		varchar(500);
	-- 淇敼瀛樺偍杩囩▼  杩囨湡浠樿垂閭欢鍙互閫€淇★細mail_type&(4+2)=0  鏀逛负 mail_type&(2+1)!=0
        declare cur   cursor  for   select  sender_role_id, recver_role_id, mail_title, mail_body, item_number, item_attachment, money_attachment , recver_role_name,sender_role_name from  UMS_ROLE_MAIL_01    where
		UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 )  and mail_type&(2+1)!=0 and  mail_type&1=1 and mail_status&4=0;
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET mark = 0;
	set mark = 1;
	open cur;
  	REPEAT
 	 	FETCH cur INTO sendroleid, receveroleid, mailtitle,mailbody,itemnum,itemattachment, money,recevername,sendrolename ;
		if mark then
	 		set sendtime = UNIX_TIMESTAMP();

			set expiredtime = sendtime + 3*86400;
			insert into   UMS_ROLE_MAIL_01( recver_role_id, sender_role_name,mail_title,mail_type,mail_status,mail_body,item_attachment,send_time,expired_time, money_attachment,post_recver, sender_role_id,item_number,recver_role_name )
				 values(  sendroleid,recevername,mailtitle,4,3,mailbody,itemattachment,sendtime,expiredtime,money,0,receveroleid,itemnum,sendrolename );
		end if;
     UNTIL  mark = 0
     END REPEAT;
    CLOSE cur;
    delete from  UMS_ROLE_MAIL_01  where UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 )  and mail_type&(2+1)!=0 and  mail_type&1=1 and mail_status&4=0;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `BackExpiredMail_02` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 PROCEDURE `BackExpiredMail_02`()
begin
	declare  tablenum       	int(0);
	declare  destid  		int(0);
	declare srcid      		int(0);
	declare i                		int(0);
	declare mailtype	        int(0 );
	declare mailstatus           int(0);
	declare sendroleid           int(0);
	declare receveroleid       int(0);
	declare mailtitle       	 	varchar( 100 );
	declare mailbody    	  	varchar( 1000);
	declare recevername	varchar(30);
	declare sendrolename     varchar(30);
	declare itemnum		int( 0 );
	declare itemattachment  blob;
	declare money			int( 0 );
	declare sendtime             int( 0 );
	declare expiredtime        int( 0 );
	declare mark 			int(1);
	declare sqltext  		varchar(500);
	-- 淇敼瀛樺偍杩囩▼  杩囨湡浠樿垂閭欢鍙互閫€淇★細mail_type&(4+2)=0  鏀逛负 mail_type&(2+1)!=0
        declare cur   cursor  for   select  sender_role_id, recver_role_id, mail_title, mail_body, item_number, item_attachment, money_attachment , recver_role_name,sender_role_name from  UMS_ROLE_MAIL_02     where
		 UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 )  and mail_type&(1+2)!=0 and  mail_type&1=1 and mail_status&4=0;
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET mark = 0;
	set mark = 1;
	open cur;
  	REPEAT
 	 	FETCH cur INTO sendroleid, receveroleid, mailtitle,mailbody,itemnum,itemattachment, money,recevername,sendrolename ;
		if mark then
	 		set sendtime = UNIX_TIMESTAMP();

			set expiredtime = sendtime + 3*86400;
			insert into   UMS_ROLE_MAIL_02( recver_role_id, sender_role_name,mail_title,mail_type,mail_status,mail_body,item_attachment,send_time,expired_time, money_attachment,post_recver, sender_role_id,item_number,recver_role_name )
				 values(  sendroleid,recevername,mailtitle,4,3,mailbody,itemattachment,sendtime,expiredtime,money,0,receveroleid,itemnum,sendrolename );
		end if;
     UNTIL  mark = 0
     END REPEAT;
    CLOSE cur;
    delete from  UMS_ROLE_MAIL_02   where UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 )  and mail_type&(1+2)!=0 and  mail_type&1=1 and mail_status&4=0;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `BackExpiredMail_03` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 PROCEDURE `BackExpiredMail_03`()
begin
	declare  tablenum       	int(0);
	declare  destid  		int(0);
	declare srcid      		int(0);
	declare i                		int(0);
	declare mailtype	        int(0 );
	declare mailstatus           int(0);
	declare sendroleid           int(0);
	declare receveroleid       int(0);
	declare mailtitle       	 	varchar( 100 );
	declare mailbody    	  	varchar( 1000);
	declare recevername	varchar(30);
	declare sendrolename     varchar(30);
	declare itemnum		int( 0 );
	declare itemattachment  blob;
	declare money			int( 0 );
	declare sendtime             int( 0 );
	declare expiredtime        int( 0 );
	declare mark 			int(1);
	declare sqltext  		varchar(500);
	-- 淇敼瀛樺偍杩囩▼  杩囨湡浠樿垂閭欢鍙互閫€淇★細mail_type&(4+2)=0  鏀逛负 mail_type&(2+1)!=0
        declare cur   cursor  for   select  sender_role_id, recver_role_id, mail_title, mail_body, item_number, item_attachment, money_attachment , recver_role_name,sender_role_name from  UMS_ROLE_MAIL_03   where
		UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 )  and mail_type&(2+1)!=0 and  mail_type&1=1 and mail_status&4=0;
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET mark = 0;
	set mark = 1;
	open cur;
  	REPEAT
 	 	FETCH cur INTO sendroleid, receveroleid, mailtitle,mailbody,itemnum,itemattachment, money,recevername,sendrolename ;
		if mark then
	 		set sendtime = UNIX_TIMESTAMP();

			set expiredtime = sendtime + 3*86400;
			insert into   UMS_ROLE_MAIL_03( recver_role_id, sender_role_name,mail_title,mail_type,mail_status,mail_body,item_attachment,send_time,expired_time, money_attachment,post_recver, sender_role_id,item_number,recver_role_name )
				 values(  sendroleid,recevername,mailtitle,4,3,mailbody,itemattachment,sendtime,expiredtime,money,0,receveroleid,itemnum,sendrolename );
		end if;
     UNTIL  mark = 0
     END REPEAT;
    CLOSE cur;
    delete from  UMS_ROLE_MAIL_03 where	UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 )  and mail_type&(1+2)!=0 and  mail_type&1=1 and mail_status&4=0;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `BackExpiredMail_04` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 PROCEDURE `BackExpiredMail_04`()
begin
	declare  tablenum       	int(0);
	declare  destid  		int(0);
	declare srcid      		int(0);
	declare i                		int(0);
	declare mailtype	        int(0 );
	declare mailstatus           int(0);
	declare sendroleid           int(0);
	declare receveroleid       int(0);
	declare mailtitle       	 	varchar( 100 );
	declare mailbody    	  	varchar( 1000);
	declare recevername	varchar(30);
	declare sendrolename     varchar(30);
	declare itemnum		int( 0 );
	declare itemattachment  blob;
	declare money			int( 0 );
	declare sendtime             int( 0 );
	declare expiredtime        int( 0 );
	declare mark 			int(1);
	declare sqltext  		varchar(500);
	-- 淇敼瀛樺偍杩囩▼  杩囨湡浠樿垂閭欢鍙互閫€淇★細mail_type&(4+2)=0  鏀逛负 mail_type&(2+1)!=0
        declare cur   cursor  for   select  sender_role_id, recver_role_id, mail_title, mail_body, item_number, item_attachment, money_attachment , recver_role_name,sender_role_name from  UMS_ROLE_MAIL_04   where
		UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 )  and mail_type&(1+2)!=0 and  mail_type&1=1 and mail_status&4=0;
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET mark = 0;
	set mark = 1;
	open cur;
  	REPEAT
 	 	FETCH cur INTO sendroleid, receveroleid, mailtitle,mailbody,itemnum,itemattachment, money,recevername,sendrolename ;
		if mark then
	 		set sendtime = UNIX_TIMESTAMP();

			set expiredtime = sendtime + 3*86400;
			insert into   UMS_ROLE_MAIL_04( recver_role_id, sender_role_name,mail_title,mail_type,mail_status,mail_body,item_attachment,send_time,expired_time, money_attachment,post_recver, sender_role_id,item_number,recver_role_name )
				 values(  sendroleid,recevername,mailtitle,4,3,mailbody,itemattachment,sendtime,expiredtime,money,0,receveroleid,itemnum,sendrolename );
		end if;
     UNTIL  mark = 0
     END REPEAT;
    CLOSE cur;
    delete from  UMS_ROLE_MAIL_04 where	UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 )  and mail_type&(1+2)!=0 and  mail_type&1=1 and mail_status&4=0;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `BackExpiredMail_05` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 PROCEDURE `BackExpiredMail_05`()
begin
	declare  tablenum       	int(0);
	declare  destid  		int(0);
	declare srcid      		int(0);
	declare i                		int(0);
	declare mailtype	        int(0 );
	declare mailstatus           int(0);
	declare sendroleid           int(0);
	declare receveroleid       int(0);
	declare mailtitle       	 	varchar( 100 );
	declare mailbody    	  	varchar( 1000);
	declare recevername	varchar(30);
	declare sendrolename     varchar(30);
	declare itemnum		int( 0 );
	declare itemattachment  blob;
	declare money			int( 0 );
	declare sendtime             int( 0 );
	declare expiredtime        int( 0 );
	declare mark 			int(1);
	declare sqltext  		varchar(500);
	-- 淇敼瀛樺偍杩囩▼  杩囨湡浠樿垂閭欢鍙互閫€淇★細mail_type&(4+2)=0  鏀逛负 mail_type&(2+1)!=0
        declare cur   cursor  for   select  sender_role_id, recver_role_id, mail_title, mail_body, item_number, item_attachment, money_attachment , recver_role_name,sender_role_name from  UMS_ROLE_MAIL_05   where
		UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 )  and mail_type&(2+1)!=0 and  mail_type&1=1 and mail_status&4=0;
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET mark = 0;
	set mark = 1;
	open cur;
  	REPEAT
 	 	FETCH cur INTO sendroleid, receveroleid, mailtitle,mailbody,itemnum,itemattachment, money,recevername,sendrolename ;
		if mark then
	 		set sendtime = UNIX_TIMESTAMP();
			set expiredtime = sendtime + 3*86400;
			insert into   UMS_ROLE_MAIL_05( recver_role_id, sender_role_name,mail_title,mail_type,mail_status,mail_body,item_attachment,send_time,expired_time, money_attachment,post_recver, sender_role_id,item_number,recver_role_name )
				 values(  sendroleid,recevername,mailtitle,4,3,mailbody,itemattachment,sendtime,expiredtime,money,0,receveroleid,itemnum,sendrolename );
		end if;
     UNTIL  mark = 0
     END REPEAT;
    CLOSE cur;
    delete from  UMS_ROLE_MAIL_05 where UNIX_TIMESTAMP( ) >=  expired_time and ( item_number >0 or money_attachment > 0 ) and mail_type&(1+2)!=0 and  mail_type&1=1 and mail_status&4=0;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DelExpiredMail` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/  /*!50003 PROCEDURE `DelExpiredMail`()
BEGIN
	
	delete from  UMS_ROLE_MAIL_01  where   expired_time <=  UNIX_TIMESTAMP( )  and  mail_type&2=0 and mail_status&4= 0  and  ( mail_type&4!= 0 or mail_type&1=0  or( item_number=0  and mail_status&4= 0  and money_attachment=0 ));
	delete from  UMS_ROLE_MAIL_02  where   expired_time <=   UNIX_TIMESTAMP( )  and  mail_type&2=0 and mail_status&4= 0  and  ( mail_type&4!= 0 or mail_type&1=0 or (item_number=0  and mail_status&4= 0 and money_attachment=0 )) ;
	delete from  UMS_ROLE_MAIL_03  where   expired_time <=   UNIX_TIMESTAMP( )   and mail_type&2=0 and mail_status&4= 0  and (  mail_type&4!= 0 or mail_type&1=0 or( item_number=0  and mail_status&4= 0 and money_attachment=0 )) ;
	delete from  UMS_ROLE_MAIL_04  where   expired_time <=   UNIX_TIMESTAMP( )   and mail_type&2=0 and mail_status&4= 0  and(   mail_type&4!= 0 or mail_type&1=0  or (item_number=0  and mail_status&4= 0 and money_attachment=0 ));

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
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
		SELECT silver_yb INTO nBalance FROM UMS_ROLE WHERE role_id = nRoleID;	
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
/*!50003 CREATE*/  /*!50003 PROCEDURE `ProcRecoverRole`(nCharID int)
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
			' ( role_id,role_hp,role_mp,role_ap,role_basic,role_skill,role_buff,role_cdgroup,role_package,role_equipment,role_storage,role_pvp,role_title,role_activity,role_task_package,role_repetion,save_bitflag,check_sum )',
			' SELECT role_id,role_hp,role_mp,role_ap,role_basic,role_skill,role_buff,role_cdgroup,role_package,role_equipment,role_storage,role_pvp,role_title,role_activity,role_task_package,role_repetion,save_bitflag,check_sum ',
			' FROM  UMS_ROLE_DETAIL_REMOVED  WHERE role_id = ', nCharID, ' AND remove_id = ' , nRemoveID, ' ORDER BY seq_id desc limit 0, 1' );

		PREPARE SQLSA FROM @statement ;
		EXECUTE  SQLSA ;

		SET tablename = CONCAT( 'UMS_ROLE_DETAIL1_',  LPAD( MOD( nAccountID, 16), 2, 0));

		SET @statement = CONCAT('INSERT INTO ', tablename,
			' ( role_id,role_task,role_talent,role_monster_record,role_friend,role_tele_pos,role_wizard,role_lifeskill,role_client,role_charm,role_guide,role_times,role_matrix,role_exptimeinfo,role_mw,role_statistic ) ',
			' SELECT role_id,role_task,role_talent,role_monster_record,role_friend,role_tele_pos,role_wizard,role_lifeskill,role_client,role_charm,role_guide,role_times,role_matrix,role_exptimeinfo,role_mw,role_statistic ',
			' FROM  UMS_ROLE_DETAIL1_REMOVED  WHERE role_id = ', nCharID, ' AND remove_id = ' , nRemoveID, ' ORDER BY seq_id desc limit 0, 1' );

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
/*!50003 CREATE*/  /*!50003 PROCEDURE `ProcRemoveRole`(nAccountID int, nCharID	int)
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
			' role_id,role_hp,role_mp,role_ap,role_basic,role_skill,role_buff,role_cdgroup,role_package,role_equipment,role_storage,role_pvp,role_title,role_activity,role_task_package,role_repetion,save_bitflag,check_sum, remove_id )',
			' SELECT role_id,role_hp,role_mp,role_ap,role_basic,role_skill,role_buff,role_cdgroup,role_package,role_equipment,role_storage,role_pvp,role_title,role_activity,role_task_package,role_repetion,save_bitflag,check_sum, ', nRemoveID ,
			' FROM ', tablename, ' WHERE role_id = ', nCharID);

	PREPARE SQLSA FROM @statement ;
	EXECUTE  SQLSA ;

	SET @statement = CONCAT('DELETE FROM ', tablename, ' WHERE role_id = ', nCharID);
	PREPARE SQLSA FROM @statement ;

	EXECUTE  SQLSA ;

	SET tablename = CONCAT( 'UMS_ROLE_DETAIL1_',  LPAD( MOD( nAccountID, 16), 2, 0));
	SET @statement = CONCAT('INSERT INTO 	UMS_ROLE_DETAIL1_REMOVED ( ',
			' role_id,role_task,role_talent,role_monster_record,role_friend,role_tele_pos,role_wizard,role_lifeskill,role_client,role_charm,role_guide,role_times,role_matrix,role_exptimeinfo,role_mw,role_statistic, remove_id )',
			' SELECT role_id,role_task,role_talent,role_monster_record,role_friend,role_tele_pos,role_wizard,role_lifeskill,role_client,role_charm,role_guide,role_times,role_matrix,role_exptimeinfo,role_mw,role_statistic, ', nRemoveID ,
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
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
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
/*!50003 CREATE*/  /*!50003 PROCEDURE `ProcUseIbItem`(ullPurchaseID bigint )
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
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2009-09-24  9:37:22

 
-- ----------------------------
-- Procedure structure for ProcSaveAchieve
-- ----------------------------
DROP PROCEDURE IF EXISTS `ProcSaveAchieve`;
DELIMITER ;;
CREATE  PROCEDURE `ProcSaveAchieve`(nRoleID int, nAchieveInfo blob)
BEGIN
	DECLARE nExist int;
	SET nExist = 0;
	START   TRANSACTION;
	SELECT count(*) INTO nExist FROM UMS_ACHIEVE WHERE role_id = nRoleID;
	if nExist > 0 THEN
		update UMS_ACHIEVE set achieve_info = nAchieveInfo where role_id = nRoleID;
	else
		insert into UMS_ACHIEVE values( nRoleID, nAchieveInfo );
	END IF;

	SELECT ROW_COUNT();
	COMMIT;
END;;
DELIMITER ;


-- ---------------------------------------------------
-- Procedure structure for ProcGetActiveRoleLevelInfo
-- ---------------------------------------------------
DROP PROCEDURE IF EXISTS `ProcGetActiveRoleLevelInfo`;
DELIMITER ;;
CREATE  PROCEDURE `ProcGetActiveRoleLevelInfo`()
BEGIN
-- Routine body goes here...

	declare val int;
	declare num int;
	declare total int;
	declare world_id int;
	declare days_from int;
	declare number int;

	drop temporary table if exists level_score;
	create temporary table level_score(id int, score int, role_num int);

	set @days_from = to_days(now());

	set @world_id = 1;
	while @world_id<5 do

		set @total = 0;
		set @number = 0;

		select sum(role_level), count(*) from UMS_ROLE 
			where @days_from-to_days(update_time)<=7 
			and nationality_id=@world_id 
			and role_level>0 
			and role_level<51 
			into @val, @num;
		if @num>0 then
			set @total = @total + 1*@val;
			set @number = @number + @num;
		end if;

		select sum(role_level-50), count(*) from UMS_ROLE 
			where @days_from-to_days(update_time)<=7 
			and nationality_id=@world_id 
			and role_level>50 
			and role_level<71 
			into @val, @num;
		if @num>0 then
			set @total = @total + 3*@val + 50*@num;
			set @number = @number + @num;
		end if;

		select sum(role_level-70), count(*) from UMS_ROLE 
			where @days_from-to_days(update_time)<=7 
			and nationality_id=@world_id 
			and role_level>70 
			and role_level<91 
			into @val, @num;
		if @num>0 then
			set @total = @total + 4*@val + 110*@num;
			set @number = @number + @num;
		end if;

		select sum(role_level-90), count(*) from UMS_ROLE 
			where @days_from-to_days(update_time)<=7 
			and nationality_id=@world_id 
			and role_level>90 
			and role_level<111 
			into @val, @num;
		if @num>0 then
			set @total = @total + 5*@val + 190*@num;
			set @number = @number + @num;
		end if;

		insert into level_score(id, score, role_num) values(@world_id, @total, @number);

		set @world_id = @world_id + 1;

	end while;

	select id, score, role_num from level_score;

END;;
DELIMITER ;


/*create procedure for yuanbao stall*/
DELIMITER $$

DROP PROCEDURE IF EXISTS `ProcYBStallExchange`$$

CREATE  PROCEDURE `ProcYBStallExchange`(
	nSellerRoleID int, 
	nSellerAccountID int, 
	nBuyerRoleID int, 
	nBuyerAccountID int,
	nSellerRoleLevel int,
	nBuyerRoleLevel int,
	nSellerRoleMetier int,
	nBuyerRoleMetier int,
	nSellerRoleGender int,
	nBuyerRoleGender int,
	nItemID int,
	nItemNum int,
	nItemGuid bigint,
	nPrice int,
	nPriceTYpe int,
	nFee int,
	nWorldID int,
	nBuyerGameID int,
	nSellerGameID int,
	nBuyerIP int ,
	nSellerIP int,
	nIBFeeID int
)
BEGIN
	DECLARE nAmount INTEGER DEFAULT 0;
	DECLARE nUnixTime INTEGER DEFAULT 0;
	DECLARE ullExchangeID BIGINT DEFAULT 0;
	DECLARE nPriceUnit INT DEFAULT 0;
	DECLARE nPriceValue INT DEFAULT 0;
	DECLARE nDiscountPrice INT DEFAULT 0;
	DECLARE nPackageFlag INT DEFAULT 0;
	DECLARE nBalance INT DEFAULT 0;
	
	SET nAmount = nItemNum * nPrice;
	SET nUnixTime =  UNIX_TIMESTAMP();
	SET nPriceUnit = 5;
	SET nPriceValue = 1;
	SET nDiscountPrice = 1;
	SET nPackageFlag = 1;

	SELECT golden_yb INTO nBalance FROM UMS_ACCOUNT where account_id = nBuyerAccountID;
	
	IF ( nBalance >= nAmount ) THEN
		
	-- buyer
		
		UPDATE UMS_ACCOUNT SET golden_yb = golden_yb - nAmount WHERE account_id = nBuyerAccountID;
	-- seller
		
		UPDATE UMS_ACCOUNT SET golden_yb = golden_yb + nAmount - nFee, consumed_golden_yb = consumed_golden_yb + nFee WHERE account_id = nSellerAccountID;
	-- exchange yb log
		
		INSERT INTO UMS_YB_STALL_LOG( seller_role_id, buyer_role_id, 
			seller_role_level, buyer_role_level, 
			seller_role_metier, buyer_role_metier,
			seller_role_gender, buyer_role_gender,
			item_id, item_count, item_guid, 
			price, subject_id, amount, fee, 
			seller_ip, buyer_ip,
			exch_time)
			VALUES (nSellerRoleID, nBuyerRoleID, 
				nSellerRoleLevel, nBuyerRoleLevel, 
				nSellerRoleMetier,nBuyerRoleMetier,
				nSellerRoleGender,nBuyerRoleGender,
				nItemID, nItemNum, nItemGuid, 
				nPrice, nPriceType, nAmount, nFee, 
				nBuyerIP, nSellerIP, 
				nUnixTime
		);
		
		SET ullExchangeID = LAST_INSERT_ID();
	-- yb exchange and inform erating
		INSERT INTO UMS_BOURSE_LOG(bourse_buy_detail, bourse_sell_detail,  buy_account, buy_role, sell_account, sell_role, 
			yb_count, bourse_date, rating_id, world_id, buy_price, sell_price, type, buy_account_game_id, sell_account_game_id)
		values(0, ullExchangeID, nSellerAccountID, nSellerRoleID, nBuyerAccountID, nBuyerRoleID, nAmount, nUnixTime, nWorldID, nWorldID, 0, 0, 2, nSellerGameID, nBuyerGameID);
		
		SET ullExchangeID = LAST_INSERT_ID();
		insert into UMS_BOURSE_LOG_LOCKED( bourse_detail, sell_account, buy_account, yb_count, rating_id, world_id, bourse_date,
			buy_role,sell_role, type, buy_account_game_id, sell_account_game_id  ) 
		select bourse_detail, sell_account, buy_account, yb_count, rating_id, world_id, bourse_date, buy_role, sell_role, type, buy_account_game_id, sell_account_game_id 
		from UMS_BOURSE_LOG 
	        where  bourse_detail = ullExchangeID;
			
	-- consume_log
		INSERT INTO GMS_CONSUME_LOG ( purchase_id, account_id, role_id,  gender_id,  metier_id,  role_level,   ib_code, package_flag, item_count, pay_time,  user_ip, subject_id,  audit_flag,  price, discount_price, rating_id, game_id)
			VALUES (NULL,  nSellerAccountID, nSellerRoleID, nSellerRoleGender, nSellerRoleMetier, nSellerRoleLevel,  nIBFeeID, nPackageFlag, nFee,  nUnixTime, nSellerIP, nPriceUnit , 1, 1, 1, nWorldID, nSellerGameID);
		SET ullExchangeID = LAST_INSERT_ID();
		UPDATE GMS_CONSUME_LOG SET detail_id =  nWorldID * 10000000000 + ullExchangeID WHERE purchase_id = ullExchangeID AND rating_id = nWorldID;
		
		INSERT INTO GMS_CONSUME_LOCKED ( purchase_id, account_id, role_id,  gender_id,  metier_id,  role_level,   ib_code, package_flag, item_count, pay_time,  user_ip, subject_id,  audit_flag,  price, discount_price, rating_id, game_id, detail_id)
			VALUES (ullExchangeID,  nSellerAccountID, nSellerRoleID, nSellerRoleGender, nSellerRoleMetier, nSellerRoleLevel,  nIBFeeID, nPackageFlag, nFee,  nUnixTime, nSellerIP, nPriceUnit , 1, nPriceValue, nDiscountPrice, nWorldID, nSellerGameID, nWorldID * 10000000000 + ullExchangeID);
		
		SELECT 0, nBalance;
		
	ELSE
		SELECT -1, nBalance;
	END IF;
	
	-- SELECT ullExchangeID;
	
    END$$

DELIMITER ;

-- ----------------------------
-- Procedure structure for `ProcYaoQianShuEndActivity`
-- ----------------------------
DROP PROCEDURE IF EXISTS `ProcYaoQianShuEndActivity`;
DELIMITER ;;
CREATE PROCEDURE `ProcYaoQianShuEndActivity`(nId int, tEndTime datetime)
BEGIN
	update UMS_ACTIVITY_YQS_INDEX set EndTime=tEndTime where id=nId;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `ProcYaoQianShuLoadActivity`
-- ----------------------------
DROP PROCEDURE IF EXISTS `ProcYaoQianShuLoadActivity`;
DELIMITER ;;
CREATE PROCEDURE `ProcYaoQianShuLoadActivity`(nActivityId int, nCampId int)
BEGIN
	 select * from UMS_ACTIVITY_YQS where ActivityId=nActivityId and CampID=nCampId and EndTime="00:00";
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `ProcYaoQianShuLoadPlayerRankInfo`
-- ----------------------------
DROP PROCEDURE IF EXISTS `ProcYaoQianShuLoadPlayerRankInfo`;
DELIMITER ;;
CREATE PROCEDURE `ProcYaoQianShuLoadPlayerRankInfo`()
BEGIN
	DECLARE nID int;
	select ID into nID from UMS_ACTIVITY_YQS_INDEX order by ID desc limit 1;

	select * from v_activity_yqs_playerrankinfo where ID=nID order by Contribute desc limit 50;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `ProcYaoQianShuStartActivity`
-- ----------------------------
DROP PROCEDURE IF EXISTS `ProcYaoQianShuStartActivity`;
DELIMITER ;;
CREATE PROCEDURE `ProcYaoQianShuStartActivity`(nActivityID int, tStartTime datetime)
BEGIN
	DECLARE nID int;
	if not exists( select * from UMS_ACTIVITY_YQS_INDEX  where StartTime=tStartTime  ) then
		insert into UMS_ACTIVITY_YQS_INDEX( ActivityID, StartTime, EndTime ) values( nActivityID, tStartTime, "" );
	end if;

	select ID into nID from UMS_ACTIVITY_YQS_INDEX where StartTime=tStartTime;

	if not exists( select * from UMS_ACTIVITY_YQS_CAMPINFO where ID=nID ) then
		insert into UMS_ACTIVITY_YQS_CAMPINFO( ID, CampID, CampRank, CurrHandinDawNum, CurrGrowing, CurrLevel, SettlePhase  ) values( nID, 1, 1, 0, 0, 1, -1  );
		insert into UMS_ACTIVITY_YQS_CAMPINFO( ID, CampID, CampRank, CurrHandinDawNum, CurrGrowing, CurrLevel, SettlePhase  ) values( nID, 2, 2, 0, 0, 1, -1  );
		insert into UMS_ACTIVITY_YQS_CAMPINFO( ID, CampID, CampRank, CurrHandinDawNum, CurrGrowing, CurrLevel , SettlePhase ) values( nID, 3, 3, 0, 0, 1, -1  );
		insert into UMS_ACTIVITY_YQS_CAMPINFO( ID, CampID, CampRank, CurrHandinDawNum, CurrGrowing, CurrLevel , SettlePhase ) values( nID, 4, 4, 0, 0, 1, -1  );
	end if;

	select * from v_activity_yqs where ID=nID;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `ProcYaoQianShuUpdateUserInfo`
-- ----------------------------
DROP PROCEDURE IF EXISTS `ProcYaoQianShuUpdateUserInfo`;
DELIMITER ;;
CREATE PROCEDURE `ProcYaoQianShuUpdateUserInfo`(nPlayerId int, nActivityId int, nContribute int, nHandInNum int)
BEGIN
	if not exists( select * from UMS_ACTIVITY_YQS_USERINFO where PlayerId=nPlayerId and  AcitivityIndex=nActivityId  ) then
		insert into UMS_ACTIVITY_YQS_USERINFO( PlayerId, AcitivityIndex, Contribute, HandInNum ) values( nPlayerId, nActivityId, nContribute, nHandInNum );
	else
		update UMS_ACTIVITY_YQS_USERINFO set Contribute=nContribute, HandInNum=nHandInNum where PlayerId=nPlayerId and AcitivityIndex=nActivityId;
	end if;
END;;
DELIMITER ;

DELIMITER ;

/*!50003 DROP PROCEDURE IF EXISTS `proc_delete_overdue` */;

DELIMITER ;;
/*!50003 CREATE*/ /*!50003 PROCEDURE `proc_delete_overdue`()
BEGIN
DECLARE TABLE_NAME CHAR(48);
DECLARE TABLE_IDX  int;

SET TABLE_IDX=0;
	WHILE ( TABLE_IDX < 16 ) DO
		IF( TABLE_IDX  < 10 ) then
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_DETAIL_0', TABLE_IDX );
		ELSE
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_DETAIL_', TABLE_IDX );
		END IF;
SET @SqlCmd =concat( 'delete from ', TABLE_NAME,
' where role_id in ( select role_id from  UMS_ROLE  where account_id in ( select account_id from UMS_ACCOUNT where  charged_golden_yb = 0 and consumed_golden_yb = 0 and charged_silver_yb=0  and golden_yb = 0 ) and role_marriageid = 0 and role_level <= 30 and login_time <= UNIX_TIMESTAMP(NOW())  - 90* 86400)');
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
SET @SqlCmd =concat( 'delete  from ', TABLE_NAME,
' where role_id in ( select role_id from  UMS_ROLE  where account_id in ( select account_id from UMS_ACCOUNT where  charged_golden_yb = 0 and consumed_golden_yb = 0 and charged_silver_yb=0  and golden_yb = 0 ) and role_marriageid = 0 and role_level <= 30 and login_time <= UNIX_TIMESTAMP(NOW())  - 90* 86400)');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;
SET TABLE_IDX=TABLE_IDX+1;
	END WHILE;

SET TABLE_IDX=1;
	WHILE ( TABLE_IDX < 5 ) DO
			SET TABLE_NAME=CONCAT( 'UMS_ROLE_MAIL_0', TABLE_IDX );
SET @SqlCmd =concat( 'delete  from ', TABLE_NAME,
' where recver_role_id  in ( select role_id from  UMS_ROLE  where account_id in ( select account_id from UMS_ACCOUNT where  charged_golden_yb = 0 and consumed_golden_yb = 0 and charged_silver_yb=0  and golden_yb = 0 )and role_marriageid = 0  and role_level <=30 and login_time <= UNIX_TIMESTAMP(NOW())  - 90 * 86400)');
		PREPARE stmt FROM @SqlCmd;
		EXECUTE stmt;
SET TABLE_IDX=TABLE_IDX+1;
	END WHILE;

delete from  UMS_ROLE  where account_id in ( select account_id from UMS_ACCOUNT where  charged_golden_yb = 0 and consumed_golden_yb = 0 and charged_silver_yb=0 and golden_yb = 0 )and role_marriageid = 0  and role_level <= 30 and login_time <= UNIX_TIMESTAMP(NOW())  - 90 * 86400;


END */;;

DELIMITER ;
DROP PROCEDURE IF EXISTS `ProcPayFunc` ;
DELIMITER ;;
CREATE   PROCEDURE `ProcPayFunc`(nFuncItemID int, nCount int, nPriceType int, nPriceValue int, nAccountID int, nRoleID int, nGenderID int, nMetierID int, nRoleLevel int, nUserIP int, nWorldID int, nServerID int, nGameID int)
BEGIN
	DECLARE nConsumeMoney INTEGER DEFAULT 0;
	DECLARE nUnixTime INTEGER DEFAULT 0;
	DECLARE ullExchangeID BIGINT DEFAULT 0;
	DECLARE nBalance INTEGER DEFAULT 0;

	-- START   TRANSACTION;

	SET nConsumeMoney = nCount * nPriceValue;
	SET nUnixTime =  UNIX_TIMESTAMP();

	IF nPriceType = 5 THEN
		SELECT golden_yb INTO nBalance FROM UMS_ACCOUNT WHERE account_id=nAccountID;
	ELSEIF nPriceType = 4 THEN
		SELECT silver_yb INTO nBalance FROM UMS_ROLE WHERE role_id = nRoleID;	
	END IF;

	IF nBalance < nConsumeMoney THEN
		
		SELECT -1, nBalance ;
		
	ELSE
	
	IF nPriceType = 5 THEN
		UPDATE UMS_ACCOUNT SET golden_yb = golden_yb - nConsumeMoney , consumed_golden_yb = consumed_golden_yb + nConsumeMoney WHERE account_id = nAccountID;
	ELSEIF nPriceType = 4 THEN
		UPDATE UMS_ROLE SET silver_yb = silver_yb - nConsumeMoney , consumed_silver_yb = consumed_silver_yb + nConsumeMoney WHERE role_id = nRoleID;
	END IF;

	INSERT INTO GMS_CONSUME_LOG ( purchase_id, account_id, role_id,  gender_id,  metier_id,  role_level,   ib_code, package_flag, item_count, pay_time,  user_ip, subject_id,  audit_flag,  price, discount_price, rating_id, game_id)
		VALUES (NULL,  nAccountID, nRoleID, nGenderID, nMetierID, nRoleLevel,  nFuncItemID, 0, nCount,  nUnixTime, nUserIP, nPriceType, 1, nPriceValue, nPriceValue, nWorldID, nGameID);

	SET ullExchangeID = LAST_INSERT_ID();

	UPDATE GMS_CONSUME_LOG SET detail_id =  nWorldID * 10000000000 + ullExchangeID WHERE purchase_id = ullExchangeID AND rating_id = nWorldID;
	
	INSERT INTO GMS_CONSUME_LOCKED ( purchase_id, account_id, role_id,  gender_id,  metier_id,  role_level,   ib_code, package_flag, item_count, pay_time,  user_ip, subject_id,  audit_flag,  price, discount_price, rating_id, game_id, detail_id)
		VALUES (ullExchangeID,  nAccountID, nRoleID, nGenderID, nMetierID, nRoleLevel,  nFuncItemID, 0, nCount,  nUnixTime, nUserIP, nPriceType, 1, nPriceValue, nPriceValue, nWorldID, nGameID, nWorldID * 10000000000 + ullExchangeID);

	-- COMMIT;

	SELECT ullExchangeID, nBalance ;
	
	END IF;
	
END ;;
DELIMITER ;
