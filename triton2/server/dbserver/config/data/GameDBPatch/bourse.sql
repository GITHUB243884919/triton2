DROP TABLE IF EXISTS `UMS_BOURSE_ACCOUNT`;
DROP TABLE IF EXISTS `UMS_BOURSE_BUY`;
DROP TABLE IF EXISTS `UMS_BOURSE_SELL`;
DROP TABLE IF EXISTS `UMS_BOURSE_BUY_HISTORY`;
DROP TABLE IF EXISTS `UMS_BOURSE_SELL_HISTORY`;
DROP TABLE IF EXISTS `UMS_BOURSE_HISTORY_LOCKED`; 
DROP TABLE IF EXISTS `UMS_BOURSE_HISTORY`;

DROP TABLE IF EXISTS `UMS_BOURSE_ACCOUNT`;
CREATE TABLE `UMS_BOURSE_ACCOUNT` (
  `account_id` int(11) NOT NULL,
  `currency_type` int(11) NOT NULL,
  `currency_value` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`account_id`,`currency_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `UMS_BOURSE_TRADE`;
CREATE TABLE `UMS_BOURSE_TRADE` (
  `bourse_doc_no` int(11) NOT NULL AUTO_INCREMENT, 
  `bourse_trade_detail`	bigint NOT NULL DEFAULT '0',
  `account_id` int(11) NOT NULL,
  `price` int(11) DEFAULT '0',
  `yb_count` int(11) DEFAULT '0',
  `trade_yb` int(11) DEFAULT '0',
  `trade_kind` int(11) DEFAULT '0',
  `trade_type` int(11) DEFAULT '0',
  `bourse_date` int(11) DEFAULT '0',
  `bourse_tax` int(11) DEFAULT '0',
  `role_id`	   int(11) DEFAULT '0',
  PRIMARY KEY (`bourse_doc_no`),
  KEY `IDX_BOURSE_TRADE_BY_ACCT` (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `UMS_BOURSE_LOG`;
CREATE TABLE `UMS_BOURSE_LOG` (
  `bourse_detail`	int( 11 ) NOT NULL AUTO_INCREMENT,
  `bourse_buy_detail` bigint NOT NULL,
  `bourse_sell_detail` bigint NOT NULL,
  `sell_price`	int(11) NOT NULL,
  `sell_account` int(11) NOT NULL,
  `sell_role`	int(11) NOT NULL,
  `buy_price`	int(11) NOT NULL,
  `buy_account`	int(11) NOT NULL,
  `buy_role`	int(11) NOT NULL,
  `yb_count` int(11) DEFAULT '0',
  `bourse_date` int(11) DEFAULT '0',
  `rating_id`	int(11) DEFAULT '0',
  `world_id`	int(11) DEFAULT	'0',
  PRIMARY KEY ( `bourse_detail`, `rating_id` ),
  KEY `IDX_BOURSE_LOG_BY_BUY_ACCOUNT` (`buy_account`),
  KEY `IDX_BOURSE_LOG_BY_SELL_ACCOUNT` (`sell_account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `UMS_BOURSE_TRADE_HISTORY`;
CREATE TABLE `UMS_BOURSE_TRADE_HISTORY` (
  `bourse_trade_detail` bigint NOT NULL,
  `account_id` int(11) NOT NULL,
  `price` int(11) DEFAULT '0',
  `yb_count` int(11) DEFAULT '0',
  `trade_yb` int(11) DEFAULT '0',
  `trade_kind` int(11) DEFAULT '0',
  `trade_type`  int(11) DEFAULT '0',
  `bourse_date` int(11) DEFAULT '0',
  `bourse_end_date` int(11) DEFAULT '0', 
  `bourse_tax`		int(11) DEFAULT '0',
  `role_id`	   int(11) DEFAULT '0',
  PRIMARY KEY (`bourse_trade_detail`),
  KEY `IDX_BOURSE_TRADE_HISTORY_BY_ACCT` ( `account_id` )
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `UMS_BOURSE_LOG_LOCKED`;
CREATE TABLE `UMS_BOURSE_LOG_LOCKED`(
	`bourse_detail`   int( 11 ) NOT NULL,
	`sell_account`  int(11) NOT NULL default '0',
	`sell_role`		int(11) NOT NULL default '0',
	`buy_account`   int(11) NOT NULL default '0',
	`buy_role`		int(11) NOT NULL default '0',
	`yb_count` int(11) DEFAULT '0',
	`rating_id` int(11) DEFAULT '0',
	`world_id`  int(11) DEFAULT '0',
	`bourse_date` int(11) DEFAULT '0',
	`failure_times` int( 11 ) DEFAULT '0',
	PRIMARY KEY( `bourse_detail`, `rating_id` )
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



