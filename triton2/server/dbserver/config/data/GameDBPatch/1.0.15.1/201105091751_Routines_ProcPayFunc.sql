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
