-- 
-- this patch offers GameDB update from v1.0.9.15 to v1.0.10.1 
-- including new warbattle tables, new corpsbattle tables , new bourse tables, new expinfo column,
-- and onlinenum column for gs8 
--

-- onlinenum for gs8 --
insert into UMS_ONLINENO( server_id ) values( 8 );

-- war ----
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
  `submitmoney` int(11) NOT NULL DEFAULT '0',
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
  `war_titletime` int(11) NOT NULL,
  `war_winworld` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO UMS_WORLDGLOBAL( war_serial, war_status ) VALUES( 1, 0 );

alter table UMS_WORLDGLOBAL add column tax_setrate int not null default 0;
alter table UMS_WORLDGLOBAL add column tax_taxrate int not null default 5;
alter table UMS_WORLDGLOBAL add column tax_taxmoney int not null default 0;
alter table UMS_WORLDGLOBAL add column tax_corpsmoney int not null default 0;
alter table UMS_WORLDGLOBAL add column tax_settimes int not null default 0;


-- expinfo --
alter table UMS_ROLE_DETAIL_00 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_01 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_02 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_03 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_04 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_05 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_06 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_07 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_08 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_09 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_10 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_11 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_12 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_13 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_14 add column role_exptimeinfo blob DEFAULT NULL;
alter table UMS_ROLE_DETAIL_15 add column role_exptimeinfo blob DEFAULT NULL;

-- bourse --
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

alter table UMS_BOURSE_LOG_LOCKED add column sell_role  int(11) NOT NULL ;
alter table UMS_BOURSE_LOG_LOCKED add column buy_role  int(11) NOT NULL;
alter table UMS_BOURSE_LOG add column buy_role  int(11) NOT NULL;
alter table UMS_BOURSE_LOG add column sell_role  int(11) NOT NULL;




-- corps --- 
-- ------------------------------------------------
-- 修改军团数据表结构，
-- ------------------------------------------------
alter table UMS_CORPS modify column war_battleinfo blob default null;


-- ------------------------------------------------
-- 创建军团战竞价记录表
-- ------------------------------------------------
DROP TABLE IF EXISTS `UMS_CORPS_BID`;

CREATE TABLE `UMS_CORPS_BID` (
  `seq_id` int(11) NOT NULL AUTO_INCREMENT,
  `corps_id` int(11) NOT NULL,
  `npc_id` int(11) NOT NULL,
  `bid_money` int(11) NOT NULL,
  `bid_time` int(11) NOT NULL,
  PRIMARY KEY (`seq_id`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

CREATE INDEX idx_corps_npc ON UMS_CORPS_BID(corps_id, npc_id);

-- ------------------------------------------------
-- 创建NPC状态表
-- ------------------------------------------------
DROP TABLE IF EXISTS `UMS_NPC_STATUS`;

CREATE TABLE `UMS_NPC_STATUS` (
  `seq_id` int(11) NOT NULL AUTO_INCREMENT,
  `npc_id` int(11) NOT NULL,
  `map_id` int(11) NOT NULL,
  `owner_id` int(11) NOT NULL,
  `welfare_time` int(11) NOT NULL,
  `occupy_time` int(11) NOT NULL,
  `status` int(11) NOT NULL,
  PRIMARY KEY (`seq_id`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

CREATE INDEX idx_npc_status ON UMS_NPC_STATUS(npc_id);

-- ------------------------------------------------
-- 创建军团战争表
-- ------------------------------------------------
DROP TABLE IF EXISTS `UMS_CORPS_BATTLE`;

CREATE TABLE `UMS_CORPS_BATTLE` (
  `seq_id` int(11) NOT NULL AUTO_INCREMENT,
  `att_corps_id` int(11) NOT NULL,
  `def_corps_id` int(11) NOT NULL,
  `fight_type` int(11) NOT NULL,
  `npc_id` int(11) NOT NULL,
  `map_id` int(11) NOT NULL,
  `battle_key` bigint(25) NOT NULL,
  `battle_time` int(11) NOT NULL,
  PRIMARY KEY (`seq_id`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

CREATE INDEX idx_corps_battle ON UMS_CORPS_BATTLE(npc_id);
