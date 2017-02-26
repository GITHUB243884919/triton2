
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
