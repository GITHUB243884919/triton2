/*alter bourse table*/
alter table UMS_BOURSE_LOG ADD  column  type int(11) default 1; 
alter table UMS_BOURSE_LOG_LOCKED ADD  column  type int(11) default 1;  

/*create yuanbao stall table*/
/*Table structure for table `UMS_YB_STALL_LOG` */
DROP TABLE IF EXISTS `UMS_YB_STALL_LOG`;
CREATE TABLE `UMS_YB_STALL_LOG` (
  `log_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `seller_role_id` int(11) NOT NULL,
  `buyer_role_id` int(11) NOT NULL,
  `item_id` int(11) NOT NULL,
  `item_count` int(11) NOT NULL,
  `item_guid` bigint(10) NOT NULL,
  `price` int(11) NOT NULL,
  `subject_id` int(11) NOT NULL,
  `amount` int(11) NOT NULL,
  `fee` int(11) NOT NULL,
  `exch_time` int(11) NOT NULL,
  `seller_role_level` smallint(11) DEFAULT NULL,
  `buyer_role_level` smallint(11) DEFAULT NULL,
  `seller_role_metier` tinyint(11) DEFAULT NULL,
  `buyer_role_metier` tinyint(4) DEFAULT NULL,
  `seller_role_gender` tinyint(4) DEFAULT NULL,
  `buyer_role_gender` tinyint(4) DEFAULT NULL,
  `seller_ip` int(10) unsigned DEFAULT NULL,
  `buyer_ip` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`log_id`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

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
	nGameID int,
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
	
	SET nAmount = nItemNum * nPrice;
	SET nUnixTime =  UNIX_TIMESTAMP();
	SET nPriceUnit = 5;
	SET nPriceValue = 1;
	SET nDiscountPrice = 1;
	SET nPackageFlag = 1;
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
-- 元宝交易所卖家失去元宝，元宝摆摊买家失去元宝，反的
	INSERT INTO UMS_BOURSE_LOG(bourse_buy_detail, bourse_sell_detail,  buy_account, buy_role, sell_account, sell_role, yb_count, bourse_date, rating_id, world_id, buy_price, sell_price, type)
	values(0, ullExchangeID, nSellerAccountID, nSellerRoleID, nBuyerAccountID, nBuyerRoleID, nAmount, nUnixTime, nWorldID, nWorldID, 0, 0, 2);
	
	SET ullExchangeID = LAST_INSERT_ID();
	insert into UMS_BOURSE_LOG_LOCKED( bourse_detail, sell_account, buy_account, yb_count, rating_id, world_id, bourse_date, buy_role,sell_role, type  ) 
	select bourse_detail, sell_account, buy_account, yb_count, rating_id, world_id, bourse_date, buy_role, sell_role, type 
	from UMS_BOURSE_LOG 
        where  bourse_detail = ullExchangeID;
		
-- consume_log
	INSERT INTO GMS_CONSUME_LOG ( purchase_id, account_id, role_id,  gender_id,  metier_id,  role_level,   ib_code, package_flag, item_count, pay_time,  user_ip, subject_id,  audit_flag,  price, discount_price, rating_id, game_id)
		VALUES (NULL,  nSellerAccountID, nSellerRoleID, nSellerRoleGender, nSellerRoleMetier, nSellerRoleLevel,  nIBFeeID, nPackageFlag, nFee,  nUnixTime, nSellerIP, nPriceUnit , 1, 1, 1, nWorldID, nGameID);
	SET ullExchangeID = LAST_INSERT_ID();
	UPDATE GMS_CONSUME_LOG SET detail_id =  nWorldID * 10000000000 + ullExchangeID WHERE purchase_id = ullExchangeID AND rating_id = nWorldID;
	
	INSERT INTO GMS_CONSUME_LOCKED ( purchase_id, account_id, role_id,  gender_id,  metier_id,  role_level,   ib_code, package_flag, item_count, pay_time,  user_ip, subject_id,  audit_flag,  price, discount_price, rating_id, game_id, detail_id)
		VALUES (ullExchangeID,  nSellerAccountID, nSellerRoleID, nSellerRoleGender, nSellerRoleMetier, nSellerRoleLevel,  nIBFeeID, nPackageFlag, nFee,  nUnixTime, nSellerIP, nPriceUnit , 1, nPriceValue, nDiscountPrice, nWorldID, nGameID, nWorldID * 10000000000 + ullExchangeID);
	
-- 
	
	-- SELECT ullExchangeID;
	
    END$$

DELIMITER ;
