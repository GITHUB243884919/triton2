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

DROP TABLE IF EXISTS `STAT_CATALOG_DETAIL`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `STAT_CATALOG_DETAIL` (
  `server_id` int(2) NOT NULL,
  `catalog_type` int(1) NOT NULL, 
  `catalog_id`  int(4) NOT NULL, 
  `catalog_value` bigint NOT NULL DEFAULT 0,
  `delta_value` bigint NOT NULL DEFAULT 0,
  PRIMARY KEY (`server_id`, `catalog_type`, `catalog_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `GMS_CHARGE_LOG`
--

DROP TABLE IF EXISTS `GMS_CHARGE_LOG`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GMS_CHARGE_LOG` (
  `sequence_id` bigint(20) NOT NULL,
  `account_id` int(11) NOT NULL,
  `rating_id` int(11) NOT NULL,
  `subject_id` tinyint(4) NOT NULL COMMENT '1 – 表示计点；\n            2 – 表示包时；\n            3 – 表示金币；\n            4 – 表示体验币或免费币；\n            5 – 表示积分',
  `audit_flag` int(11) NOT NULL COMMENT '对单标记：\n            1 - 表示此条消息需要对单，eRating 要\n            返回以 Rating_ID，User_ID，Subject_ID\n            确定的用户消费总额；\n            2 - 表示此条消息不需要对单，eRating\n            不需要返回用户消费总额；\n            3 - 表示需要 eRating 补记或更新该购买\n   ',
  `amount` int(11) NOT NULL,
  `charge_time` int(11) NOT NULL,
  `insert_time` datetime NOT NULL,
  PRIMARY KEY (`sequence_id`),
  KEY `IDX_CHARGE_LOG_BY_ACCT` (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `GMS_CLIENTHOST_INFO`
--

DROP TABLE IF EXISTS `GMS_CLIENTHOST_INFO`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GMS_CLIENTHOST_INFO` (
  `mac` varchar(64) NOT NULL,
  `hostinfo` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`mac`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `GMS_CONSUME_LOCKED`
--

DROP TABLE IF EXISTS `GMS_CONSUME_LOCKED`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GMS_CONSUME_LOCKED` (
  `purchase_id` int(11) NOT NULL,
  `account_id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
  `gender_id` tinyint(4) NOT NULL,
  `metier_id` tinyint(4) NOT NULL,
  `role_level` tinyint(4) NOT NULL,
  `rating_id` int(11) NOT NULL,
  `ib_code` char(32) CHARACTER SET latin1 NOT NULL,
  `package_flag` tinyint(4) NOT NULL COMMENT '1 – 表示普通产品。\n            2 – 表示组合产品。',
  `item_count` int(11) NOT NULL,
  `pay_time` int(11) NOT NULL,
  `user_ip` int(11) NOT NULL,
  `subject_id` tinyint(4) NOT NULL,
  `audit_flag` int(11) NOT NULL,
  `price` int(11) NOT NULL,
  `discount_price` int(11) NOT NULL,
  `trans_status` int(11) DEFAULT '0' COMMENT '0:trading 1:trade done 2:trade failed 3: trade send to erating failed',
  `last_fail_time` int(11) DEFAULT NULL,
  `failure_times` int(11) DEFAULT '0',
  `game_id` int NOT NULL DEFAULT 0,
  `detail_id` bigint NOT NULL DEFAULT 0,
  PRIMARY KEY (`purchase_id`,`rating_id`),
  KEY `IDX_CONSUME_LOCKED_BY_ACCT` (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `GMS_CONSUME_LOG`
--

DROP TABLE IF EXISTS `GMS_CONSUME_LOG`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GMS_CONSUME_LOG` (
  `purchase_id` int(11) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
  `gender_id` tinyint(4) NOT NULL,
  `metier_id` tinyint(4) NOT NULL,
  `role_level` tinyint(4) NOT NULL,
  `rating_id` int(11) NOT NULL,
  `ib_code` char(32) CHARACTER SET latin1 NOT NULL,
  `package_flag` int(11) NOT NULL,
  `item_count` int(11) NOT NULL,
  `pay_time` int(11) NOT NULL,
  `user_ip` int(11) NOT NULL,
  `subject_id` int(11) NOT NULL,
  `audit_flag` int(11) NOT NULL,
  `price` int(11) NOT NULL,
  `discount_price` int(11) NOT NULL,
  `detail_id` bigint NOT NULL default 0,
  `game_id` int NOT NULL default 4,
  PRIMARY KEY (`purchase_id`,`rating_id`),
  KEY `IDX_CONSUME_LOG_BY_ACCT` (`account_id`,`pay_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

create index IDX_CONSUME_LOG on GMS_CONSUME_LOG ( game_id, detail_id);

--
-- Table structure for table `GMS_IBITEM_USED_LOCKED`
--

DROP TABLE IF EXISTS `GMS_IBITEM_USED_LOCKED`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GMS_IBITEM_USED_LOCKED` (
  `purchase_id` bigint(15) NOT NULL,
  `account_id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
  `gender_id` tinyint(4) NOT NULL,
  `metier_id` tinyint(4) NOT NULL,
  `role_level` tinyint(4) NOT NULL,
  `use_time` int(11) NOT NULL,
  `last_fail_time` int(11) DEFAULT NULL,
  `failure_times` int(11) DEFAULT NULL default 0,
  `game_id` int NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `GMS_IBITEM_USED_LOG`
--

DROP TABLE IF EXISTS `GMS_IBITEM_USED_LOG`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GMS_IBITEM_USED_LOG` (
  `purchase_id` bigint(15) NOT NULL,
  `account_id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
  `gender_id` tinyint(4) NOT NULL,
  `metier_id` tinyint(4) NOT NULL,
  `role_level` tinyint(4) NOT NULL,
  `use_time` int(11) NOT NULL,
  `last_fail_time` int(11) DEFAULT NULL,
  `failure_times` int(11) DEFAULT NULL,
  `game_id` int NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ACCOUNT`
--

DROP TABLE IF EXISTS `UMS_ACCOUNT`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ACCOUNT` (
  `account_id` int(11) NOT NULL DEFAULT '0',
  `user_name` char(32) NOT NULL DEFAULT '',
  `passwd` char(32) NOT NULL DEFAULT '',
  `account_status` int(11) NOT NULL DEFAULT '0' COMMENT '0:normal 1:frozen 2 :mandatory',
  `adult_flag` tinyint(4) DEFAULT NULL,
  `user_class` tinyint(4) DEFAULT '0',
  `user_point` int(11) DEFAULT '0',
  `trustee_passwd` char(32) DEFAULT NULL,
  `binded_ip` char(255) DEFAULT NULL COMMENT 'can bind muti ip address, each address divid by "," (english).can use "*" wildcard.for example:ip=192.168.41.19,192.41.*.153',
  `charged_golden_yb` int(11) DEFAULT '0',
  `consumed_golden_yb` int(11) DEFAULT '0',
  `charged_silver_yb` int(11) DEFAULT '0',
  `consumed_silver_yb` int(11) DEFAULT '0',
  `golden_yb` int(11) DEFAULT '0',
  `silver_yb` int(11) DEFAULT '0',
  `url_id` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  `user_flag` int(11) DEFAULT '0',
  `update_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `lastlogin_ip` int(11) DEFAULT '0',
  PRIMARY KEY (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

create index IDX_UMS_ACCOUNT on UMS_ACCOUNT(user_name); 

-- ----------------------------
-- Table structure for `UMS_CORPS`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_CORPS`;
CREATE TABLE `UMS_CORPS` (
  `corps_id` int(11) NOT NULL AUTO_INCREMENT,
  `corps_name` char(32) NOT NULL,
  `corps_notice` varchar(256) NOT NULL,
  `create_role_id` int(11) NOT NULL,
  `create_role_name` char(32) NOT NULL,
  `corps_level` tinyint(4) NOT NULL DEFAULT '1',
  `corps_money` int(11) NOT NULL DEFAULT '0',
  `family_info` blob NOT NULL,
  `country_id` int(11) NOT NULL DEFAULT '0',
  `disband_time` int(11) DEFAULT '0',
  `corps_status` int(11) DEFAULT '0',
  `corps_glory` int(11) DEFAULT NULL,
  `war_winnum` int(11) DEFAULT '0',
  `war_lostnum` int(11) DEFAULT '0',
  `war_score` int(11) DEFAULT '0',
  `war_starlevel` int(11) DEFAULT '1',
  `war_bindnpcid` int(11) DEFAULT '0',
  `war_bindmoney` int(11) DEFAULT '0',
  `war_battlestatus` int(11) DEFAULT '0',
  `war_campid` int(11) DEFAULT '0',
  `war_battlecity` int(11) DEFAULT '0',
  `war_battlekey` int(11) DEFAULT '0',
  `war_welfaregettime` int(11) DEFAULT '-1',
  `corps_roommoney` int(11) DEFAULT '0',
  `war_battleinfo` blob DEFAULT NULL,
  `corps_creattime` int(11) DEFAULT '0',
   `corps_color` int(11) DEFAULT '0',
   `corps_shopinfo` blob,
   `corps_forbidtime` int(11) DEFAULT '0',
   `corps_refinevalue` int(11) DEFAULT '0',
   `corps_publish` blob DEFAULT NULL,
   `corpsboss_score` int(11) DEFAULT '0',
   `corpscollect_active` int(11) DEFAULT '0',
  PRIMARY KEY (`corps_id`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `UMS_FAMILY`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_FAMILY`;
CREATE TABLE `UMS_FAMILY` (
  `family_id` int(11) NOT NULL AUTO_INCREMENT,
  `family_name` char(32) NOT NULL,
  `family_notice` char(255) DEFAULT NULL,
  `family_money` int(11) DEFAULT '0',
  `family_status` int(11) DEFAULT NULL,
  `create_role_id` int(11) NOT NULL,
  `create_role_name` char(32) NOT NULL,
  `member_info` blob,
  `country_id` tinyint(4) NOT NULL,
  `corps_id` int(11) DEFAULT '0',
  `npc_id` int(11) DEFAULT NULL,
  `bid_money` int(11) DEFAULT NULL,
  `bid_time` int(11) DEFAULT NULL,
  `disband_time` int(11) DEFAULT '0',
  `war_rank` int(11) DEFAULT '1',
  `war_winnum` int(11) DEFAULT '0',
  `war_lostnum` int(11) DEFAULT '0',
  `family_starlevel` int(11) DEFAULT '1',
  `war_exp` int(11) DEFAULT '0',
  `family_glory` int(11) DEFAULT '0',
  `war_status` int(11) DEFAULT '0',
  `family_npcmoney` int(11) DEFAULT '0',
  `family_npcglory` int(11) DEFAULT '0',
  `family_npcwelfaretime` int(11) DEFAULT '-1',
  `family_level` int(11) DEFAULT '1',
  `family_creattime` int(11) DEFAULT '0',
  `family_repetionopendate` int(11) DEFAULT '0',
  `family_repetionopentimes` int(11) DEFAULT '0',
  `family_repetionscore` int(11) DEFAULT '0',
  `family_shopinfo` blob,
  `family_stability` int(11) DEFAULT '200',
  `family_weeknum` int(11) DEFAULT '0',
  PRIMARY KEY (`family_id`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;


--
-- Table structure for table `UMS_MARRIAGE`
--
DROP TABLE IF EXISTS `UMS_MARRIAGE`;
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
  `marriage_time`  int(11) NULL DEFAULT 0 ,
	`wedding_module` int(11) NULL DEFAULT 0 ,
  PRIMARY KEY (`server_sequence`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;


--
-- Table structure for table `UMS_CAMP`
--
DROP TABLE IF EXISTS `UMS_CAMP`;
CREATE TABLE `UMS_CAMP` (
  `campid` int(11) NOT NULL,
  `marriednum` int(11) DEFAULT NULL,
  PRIMARY KEY (`campid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Table structure for table `UMS_INVEST_ANSWER`
--

DROP TABLE IF EXISTS `UMS_INVEST_ANSWER`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_INVEST_ANSWER` (
  `invest_id` int(11) NOT NULL AUTO_INCREMENT,
  `invest_answer` varchar(300) DEFAULT NULL,
  `role_id` int(11) NOT NULL,
  `role_name` char(32) NOT NULL,
  `role_level` tinyint(4) NOT NULL,
  `edition` int(11) NOT NULL DEFAULT '0',
  `library_id` int(11) NOT NULL,
  PRIMARY KEY (`invest_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ONLINENO`
--

DROP TABLE IF EXISTS `UMS_ONLINENO`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ONLINENO` (
  `server_id` tinyint(4) NOT NULL,
  `online_number` int(11) NOT NULL DEFAULT '0',
  `update_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_RANK_PLAYER`
--

DROP TABLE IF EXISTS `UMS_RANK_PLAYER`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_RANK_PLAYER` (
  `rank_date` date NOT NULL,
  `rank_type` int(11) NOT NULL,
  `rank_country` tinyint(4) NOT NULL,
  `role_id` int(11) NOT NULL,
  `role_name` char(32) NOT NULL,
  `rank_value` int(11) NOT NULL,
  `role_country` tinyint(4) NOT NULL,
  `metier_id` tinyint(4) NOT NULL,
  `order_id` int(11) NOT NULL,
  `order_title` int(11) NOT NULL,
  `value2` int(21) NOT NULL,
  PRIMARY KEY (`rank_date`,`rank_type`,`rank_country`, `role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_RANK_PLAYER_OUTDATED`
--

DROP TABLE IF EXISTS `UMS_RANK_PLAYER_OUTDATED`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_RANK_PLAYER_OUTDATED` (
  `rank_date` date NOT NULL DEFAULT '0000-00-00',
  `rank_type` int(11) NOT NULL,
  `rank_country` tinyint(4) NOT NULL,
  `role_id` int(11) NOT NULL DEFAULT '0',
  `role_name` char(32) NOT NULL,
  `rank_value` int(11) NOT NULL,
  `role_country` tinyint(4) NOT NULL,
  `metier_id` tinyint(4) NOT NULL,
  `order_change` int(11) NOT NULL,
  `coutinue_onboard` int(11) NOT NULL,
  `coutinue_first` int(11) NOT NULL,
  `order_id` int(11) NOT NULL,
  `order_title` int(11) NOT NULL,
  `value2` int(21) NOT NULL,
  PRIMARY KEY (`rank_date`,`rank_type`,`rank_country`, `role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `UMS_ROLE`
--

DROP TABLE IF EXISTS `UMS_ROLE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE` (
  `role_id` int(11) NOT NULL DEFAULT '0',
  `role_status` int(11) NOT NULL DEFAULT '0' COMMENT '1.forbid talk 2.kick 3.frozen 4.mandatory 5.destroyed (move to remove table)',
  `account_id` int(11) NOT NULL DEFAULT '0',
  `role_name` char(32) NOT NULL DEFAULT '',
  `is_alive` tinyint(4) NOT NULL DEFAULT '0',
  `gender_id` tinyint(4) NOT NULL DEFAULT '2' COMMENT '2:male 3:female',
  `metier_id` tinyint(4) NOT NULL DEFAULT '7' COMMENT '0:any metier 1:warrio 2:swordsman 3:hunter 4:sorceress 5:taoist  6:magicer  7:new guy',
  `nationality_id` tinyint(4) NOT NULL DEFAULT '1',
  `face_type` tinyint(4) NOT NULL DEFAULT '1',
  `line_id` tinyint(4) NOT NULL DEFAULT '1',
  `map_id` int(11) NOT NULL DEFAULT '1',
  `pos_x` smallint(6) DEFAULT '1',
  `pos_y` smallint(6) DEFAULT '1',
  `role_level` int(11) NOT NULL DEFAULT '1',
  `role_expr` bigint(21) NOT NULL DEFAULT '1',
  `role_money` int(11) NOT NULL DEFAULT '0',
  `bounded_money` int(11) NOT NULL DEFAULT '0',
  `country_id` int NOT NULL DEFAULT '-1',
  `login_time` int(11) NOT NULL,
  `online_duration` int(11) NOT NULL DEFAULT '0',
  `update_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `create_time` datetime DEFAULT NULL,
  `role_password` char(32) DEFAULT NULL,
  `save_status` tinyint(4) DEFAULT '0',
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` char(32) DEFAULT NULL,
  `role_status_param` int(11) DEFAULT '0',
  `role_familyid` int(11) DEFAULT '0',
  `role_corpsid` int(11) DEFAULT '0',
  `charged_silver_yb` int(11) NOT NULL DEFAULT 0,
  `consumed_silver_yb` int(11) NOT NULL DEFAULT 0,
  `silver_yb` int(11) NOT NULL DEFAULT 0,
  `storage_money` int(11) NOT NULL DEFAULT 0,
  `storage_bounded_money` int(11) NOT NULL DEFAULT 0,
  `role_deletedtime` int(11) DEFAULT NULL,
  `role_protecttime` int(11) DEFAULT '0',
  `role_vip_flag` int(11) DEFAULT '0',
  `role_vip_time` int(11) DEFAULT '0',
  `role_marriageid` int(11) DEFAULT '0',
  `role_changecamp_time` int(11) DEFAULT '0',
  `role_swornid` int(11) DEFAULT '0',
  PRIMARY KEY (`role_id`),
  KEY `IDX_ROLE_BY_ACCOUT_ID` (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;



--
-- Table structure for table `UMS_ROLE_DETAIL_00`
--
DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_00`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_00` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_01`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_01` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_02`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_02`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_02` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_03`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_03`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_03` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_04`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_04`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_04` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_05`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_05`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_05` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_06`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_06`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_06` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_07`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_07`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_07` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
--
-- Table structure for table `UMS_ROLE_DETAIL_08`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_08`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_08` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_09`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_09`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_09` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_10`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_10`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_10` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_11`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_11`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_11` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_12`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_12`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_12` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_13`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_13`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_13` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_14`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_14`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_14` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_15`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_15`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_15` (
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_task_package` blob,
  `role_repetion` blob,
  `role_pvp` blob ,
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_DETAIL_REMOVED`
--

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL_REMOVED`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL_REMOVED` (
  `seq_id` int(11) NOT NULL AUTO_INCREMENT,
  `remove_id` int(11) DEFAULT 0,
  `role_id` int(11) NOT NULL,
  `role_hp` int(11) NOT NULL default 0,
  `role_mp` int(11) NOT NULL default 0,
  `role_ap` int(11) NOT NULL default 0,
  `role_basic` blob,
  `role_skill` blob,
  `role_buff` blob,
  `role_cdgroup` blob,
  `role_package` blob,
  `role_equipment` blob,
  `role_storage` blob,
  `role_pvp` blob,
  `role_title` blob,
  `role_activity` blob,
  `role_talent` blob,
  `role_mw` blob,
  `role_lifeskill` blob,
  `role_statistic` blob,
  `role_task` blob,
  `role_task_package` blob,
  `role_monster_record` blob,
  `role_tele_Pos` blob,
  `role_wizard` blob,
  `role_friend` blob,
  `role_repetion` blob,
  `role_client` blob COMMENT 'client use only',
  `save_bitflag` int(11) DEFAULT '0',
  `check_sum` int(11) NOT NULL DEFAULT '0',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  PRIMARY KEY (`seq_id`),
  KEY `IDX_ROLE_DETAIL_REMOVED` (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;



DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_00`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_00` (
	`role_id` int(11) NOT NULL,
	`role_task` blob,
	`role_talent` blob,
	`role_monster_record` blob,
	`role_friend` blob,
	`role_tele_pos` blob,
	`role_wizard` blob,
	`role_lifeskill` blob,
	`role_client` blob COMMENT 'client use only',
	`role_charm` blob,
	`role_guide` blob,
	`role_times` blob,
	`role_matrix` blob,
	`role_exptimeinfo` blob,
	`role_mw` blob,
	`role_statistic` blob,
	`role_treasurebowl` blob,
	`role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_01` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_02`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_02` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_03`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_03` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_04`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_04` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_05`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_05` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_06`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_06` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_07`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_07` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_08`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_08` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_09`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_09` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_10`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_10` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_11`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_11` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_12`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_12` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_13`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_13` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_14`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_14` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
   `role_treasurebowl` blob,
   `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_15`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_15` (
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  `role_treasurebowl` blob,
  `role_spiritinfo` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

DROP TABLE IF EXISTS `UMS_ROLE_DETAIL1_REMOVED`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_DETAIL1_REMOVED` (
  `seq_id` int(11),
  `remove_id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
	`role_task` blob,
  `role_talent` blob,
  `role_monster_record` blob,
  `role_friend` blob,
  `role_tele_pos` blob,
  `role_wizard` blob,
  `role_lifeskill` blob,
  `role_client` blob COMMENT 'client use only',
  `role_charm` blob,
  `role_guide` blob,
  `role_times` blob,
  `role_matrix` blob,
  `role_exptimeinfo` blob,
  `role_mw` blob,
  `role_statistic` blob,
  PRIMARY KEY (`seq_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_MAIL_01`
--

DROP TABLE IF EXISTS `UMS_ROLE_MAIL_01`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_MAIL_01` (
  `mail_id` int(11) NOT NULL AUTO_INCREMENT,
  `mail_type` int(11) NOT NULL,
  `mail_status` int(11) NOT NULL,
  `sender_role_id` int(11) NOT NULL,
  `sender_role_name` char(32) NOT NULL,
  `recver_role_id` int(11) NOT NULL,
  `mail_title` char(64) DEFAULT NULL,
  `mail_body` varchar(512) DEFAULT NULL,
  `send_time` int(11) NOT NULL,
  `money_attachment` int(11) DEFAULT '0',
  `item_number` tinyint(4) NOT NULL DEFAULT '0',
  `item_attachment` blob,
  `post_recver` int(11) DEFAULT NULL,
  `expired_time` int(11) NOT NULL,
  `recver_role_name` char(32) NOT NULL,
  PRIMARY KEY (`mail_id`),
  KEY `IDX_MAIL_EXPIRED_TIME` (`expired_time`),
  KEY `IDX_MAIL_RECVER_ROLE_ID` (`recver_role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_MAIL_02`
--

DROP TABLE IF EXISTS `UMS_ROLE_MAIL_02`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_MAIL_02` (
  `mail_id` int(11) NOT NULL AUTO_INCREMENT,
  `mail_type` int(11) NOT NULL,
  `mail_status` int(11) NOT NULL,
  `sender_role_id` int(11) NOT NULL,
  `sender_role_name` char(32) NOT NULL,
  `recver_role_id` int(11) NOT NULL,
  `mail_title` char(64) DEFAULT NULL,
  `mail_body` varchar(512) DEFAULT NULL,
  `send_time` int(11) NOT NULL,
  `money_attachment` int(11) DEFAULT '0',
  `item_number` tinyint(4) NOT NULL DEFAULT '0',
  `item_attachment` blob,
  `post_recver` int(11) DEFAULT NULL,
  `expired_time` int(11) NOT NULL,
  `recver_role_name` char(32) NOT NULL,
  PRIMARY KEY (`mail_id`),
  KEY `IDX_MAIL_EXPIRED_TIME` (`expired_time`),
  KEY `IDX_MAIL_RECVER_ROLE_ID` (`recver_role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_MAIL_03`
--

DROP TABLE IF EXISTS `UMS_ROLE_MAIL_03`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_MAIL_03` (
  `mail_id` int(11) NOT NULL AUTO_INCREMENT,
  `mail_type` int(11) NOT NULL,
  `mail_status` int(11) NOT NULL,
  `sender_role_id` int(11) NOT NULL,
  `sender_role_name` char(32) NOT NULL,
  `recver_role_id` int(11) NOT NULL,
  `mail_title` char(64) DEFAULT NULL,
  `mail_body` varchar(512) DEFAULT NULL,
  `send_time` int(11) NOT NULL,
  `money_attachment` int(11) DEFAULT '0',
  `item_number` tinyint(4) NOT NULL DEFAULT '0',
  `item_attachment` blob,
  `post_recver` int(11) DEFAULT NULL,
  `expired_time` int(11) NOT NULL,
  `recver_role_name` char(32) NOT NULL,
  PRIMARY KEY (`mail_id`),
  KEY `IDX_MAIL_EXPIRED_TIME` (`expired_time`),
  KEY `IDX_MAIL_RECVER_ROLE_ID` (`recver_role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;




--
-- Table structure for table `UMS_ROLE_MAIL_04`
--

DROP TABLE IF EXISTS `UMS_ROLE_MAIL_04`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UMS_ROLE_MAIL_04` (
  `mail_id` int(11) NOT NULL AUTO_INCREMENT,
  `mail_type` int(11) NOT NULL,
  `mail_status` int(11) NOT NULL,
  `sender_role_id` int(11) NOT NULL,
  `sender_role_name` char(32) NOT NULL,
  `recver_role_id` int(11) NOT NULL,
  `mail_title` char(64) DEFAULT NULL,
  `mail_body` varchar(512) DEFAULT NULL,
  `send_time` int(11) NOT NULL,
  `money_attachment` int(11) DEFAULT '0',
  `item_number` tinyint(4) NOT NULL DEFAULT '0',
  `item_attachment` blob,
  `post_recver` int(11) DEFAULT NULL,
  `expired_time` int(11) NOT NULL,
  `recver_role_name` char(32) NOT NULL,
  PRIMARY KEY (`mail_id`),
  KEY `IDX_MAIL_EXPIRED_TIME` (`expired_time`),
  KEY `IDX_MAIL_RECVER_ROLE_ID` (`recver_role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `UMS_ROLE_REMOVED`
--
DROP TABLE IF EXISTS `UMS_ROLE_REMOVED`;
CREATE TABLE `UMS_ROLE_REMOVED` (
`seq_id`  int(11) NOT NULL AUTO_INCREMENT ,
`role_id`  int(11) NOT NULL DEFAULT 0 ,
`role_status`  int(11) NOT NULL DEFAULT 0 COMMENT '1.forbid talk 2.kick 3.frozen 4.mandatory 5.destroyed (move to remove table)' ,
`account_id`  int(11) NOT NULL DEFAULT 0 ,
`role_name`  char(32) NOT NULL DEFAULT '' ,
`is_alive`  tinyint(4) NOT NULL DEFAULT 0 ,
`gender_id`  tinyint(4) NOT NULL DEFAULT 2 COMMENT '2:male 3:female' ,
`metier_id`  tinyint(4) NOT NULL DEFAULT 7 COMMENT '0:any metier 1:warrio 2:swordsman 3:hunter 4:sorceress 5:taoist  6:magicer  7:new guy' ,
`nationality_id`  tinyint(4) NOT NULL DEFAULT 1 ,
`face_type`  tinyint(4) NOT NULL DEFAULT 1 ,
`line_id`  tinyint(4) NOT NULL DEFAULT 1 ,
`map_id`  tinyint(4) NOT NULL DEFAULT 1 ,
`pos_x`  smallint(6) NULL DEFAULT 1 ,
`pos_y`  smallint(6) NULL DEFAULT 1 ,
`role_level`  tinyint(4) NOT NULL DEFAULT 1 ,
`role_expr`  bigint(21) NOT NULL DEFAULT 1 ,
`role_money`  int(11) NOT NULL DEFAULT 0 ,
`bounded_money`  int(11) NULL DEFAULT 0 ,
`country_id`  tinyint(4) NOT NULL DEFAULT 0 ,
`login_time`  int(11) NULL DEFAULT 0 ,
`online_duration`  int(11) NOT NULL DEFAULT 0 ,
`update_time`  timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP ,
`create_time`  datetime NULL DEFAULT NULL ,
`role_password`  char(32)  NULL DEFAULT NULL ,
`save_status`  tinyint(4) NULL DEFAULT 0 ,
`save_bitflag`  int(11) NULL DEFAULT 0 ,
`check_sum`  char(32)  NULL DEFAULT NULL ,
`role_status_param`  int(11) NULL DEFAULT 0 ,
`role_familyid`  int(11) NULL DEFAULT 0 ,
`role_corpsid`  int(11) NULL DEFAULT 0 ,
`charged_silver_yb`  int(11) NOT NULL DEFAULT 0 ,
`consumed_silver_yb`  int(11) NOT NULL DEFAULT 0 ,
`silver_yb`  int(11) NOT NULL DEFAULT 0 ,
`storage_money`  int(11) NULL DEFAULT 0 ,
`storage_bounded_money`  int(11) NULL DEFAULT 0 ,
`role_deletedtime`  int(11) NULL DEFAULT NULL ,
`role_protecttime`  int(11) NULL DEFAULT 0 ,
`role_vip_flag`  int(11) NULL DEFAULT 0 ,
`role_vip_time`  int(11) NULL DEFAULT 0 ,
PRIMARY KEY (`seq_id`),
KEY `IDX_ROLE_BY_ACCOUT_ID` (`account_id`, `role_id`) 
)
ENGINE=InnoDB
DEFAULT CHARACTER SET=utf8 COLLATE=utf8_general_ci
;


--
-- Table structure for `UMS_UNION`
--

DROP TABLE IF EXISTS `UMS_UNION`;
CREATE TABLE `UMS_UNION` (
  `union_id` int(11) NOT NULL AUTO_INCREMENT,
  `union_head` int(11) NOT NULL DEFAULT '0',
  `union_name` char(40) NOT NULL DEFAULT '0',
  `union_notice` char(200) NOT NULL DEFAULT '0',
  `corps0` int(11) NOT NULL DEFAULT '0',
  `corps1` int(11) NOT NULL DEFAULT '0',
  `corps2` int(11) NOT NULL DEFAULT '0',
  `corps3` int(11) NOT NULL DEFAULT '0',
  `corps4` int(11) NOT NULL DEFAULT '0',
  `corps5` int(11) NOT NULL DEFAULT '0',
  `corps6` int(11) NOT NULL DEFAULT '0',
  `corps7` int(11) NOT NULL DEFAULT '0',
  `corps8` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`union_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;




-- ----------------------------
-- Table structure for `UMS_WORLD`
-- ----------------------------

DROP TABLE IF EXISTS `UMS_WORLD`;
CREATE TABLE `UMS_WORLD` (
  `world_id` int(11) NOT NULL DEFAULT '0',
  `country_id` int(11) NOT NULL DEFAULT '0',
  `king_power` int(11) NOT NULL DEFAULT '0',
  `people_num` int(11) NOT NULL DEFAULT '0',
  `wizard0` int(11) NOT NULL DEFAULT '0',
  `wizard1` int(11) NOT NULL DEFAULT '0',
  `wizard2` int(11) NOT NULL DEFAULT '0',
  `wizard3` int(11) NOT NULL DEFAULT '0',
  `wizard4` int(11) NOT NULL DEFAULT '0',
  `wizard5` int(11) NOT NULL DEFAULT '0',
  `convalue0` int(11) NOT NULL DEFAULT '0',
  `convalue1` int(11) NOT NULL DEFAULT '0',
  `convalue2` int(11) NOT NULL DEFAULT '0',
  `convalue3` int(11) NOT NULL DEFAULT '0',
  `convalue4` int(11) NOT NULL DEFAULT '0',
  `convalue5` int(11) NOT NULL DEFAULT '0',
  `conlevel0` int(11) NOT NULL DEFAULT '0',
  `conlevel1` int(11) NOT NULL DEFAULT '0',
  `conlevel2` int(11) NOT NULL DEFAULT '0',
  `conlevel3` int(11) NOT NULL DEFAULT '0',
  `conlevel4` int(11) NOT NULL DEFAULT '0',
  `conlevel5` int(11) NOT NULL DEFAULT '0',
  `official` blob,
  `conwizard0` int(11) NOT NULL DEFAULT '0',
  `conwizard1` int(11) NOT NULL DEFAULT '0',
  `conwizard2` int(11) NOT NULL DEFAULT '0',
  `conwizard3` int(11) NOT NULL DEFAULT '0',
  `conwizard4` int(11) NOT NULL DEFAULT '0',
  `conwizard5` int(11) NOT NULL DEFAULT '0',
  `official0` blob DEFAULT NULL,
  `official1` blob DEFAULT NULL,
  `official2` blob DEFAULT NULL,
  `official3` blob DEFAULT NULL,
  `official4` blob DEFAULT NULL,
  `official5` blob DEFAULT NULL,
  `official6` blob DEFAULT NULL,
  `version` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`world_id`, `country_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `UMS_WORLDSTATUS`
-- ----------------------------

DROP TABLE IF EXISTS `UMS_WORLDSTATUS`;
CREATE TABLE `UMS_WORLDSTATUS` (
  `world_id` int(11) NOT NULL DEFAULT '0',
  `friend0` int(11) NOT NULL DEFAULT '0',
  `friend1` int(11) NOT NULL DEFAULT '0',
  `friend2` int(11) NOT NULL DEFAULT '0',
  `friend3` int(11) NOT NULL DEFAULT '0',
  `status0` int(11) NOT NULL DEFAULT '0',
  `status1` int(11) NOT NULL DEFAULT '0',
  `status2` int(11) NOT NULL DEFAULT '0',
  `status3` int(11) NOT NULL DEFAULT '0',
  `number` int(11) NOT NULL DEFAULT '0',
  `honer_inc` int(11) NOT NULL DEFAULT '0',
  `level_score` int(11) NOT NULL DEFAULT '0',
  `honer_score` int(11) NOT NULL DEFAULT '0',
  `weak_level` int(11) NOT NULL DEFAULT '0',
  `accuse_state` int(11) NOT NULL DEFAULT '0',
  `accuse_yes` int(11) NOT NULL DEFAULT '0',
  `accuse_no` int(11) NOT NULL DEFAULT '0',
  `accuse_abort` int(11) NOT NULL DEFAULT '0',
  `gate_times` int(11) NOT NULL DEFAULT '0',
  `flag_times` int(11) NOT NULL DEFAULT '0',
  `gt_target_world` int(11) NOT NULL DEFAULT '0',
  `gt_target_time` int(11) NOT NULL DEFAULT '0',
  `lw_target_world` int(11) NOT NULL DEFAULT '0',
  `lw_target_time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`world_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `UMS_TASK_GLOBAL_DATA`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_TASK_GLOBAL_DATA`;
CREATE TABLE `UMS_TASK_GLOBAL_DATA` (
  `publish_info` blob
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for `UMS_JUDGE_FAVOR`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_JUDGE_FAVOR`;
CREATE TABLE `UMS_JUDGE_FAVOR` (
  `role_src` int(11) NOT NULL DEFAULT '0',
  `role_des` int(11) NOT NULL DEFAULT '0',
  `value` int(11) DEFAULT NULL,
  `timestamp` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_src`,`role_des`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for `UMS_JUDGE_BAD`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_JUDGE_BAD`;
CREATE TABLE `UMS_JUDGE_BAD` (
  `role_src` int(11) NOT NULL DEFAULT '0',
  `role_des` int(11) NOT NULL DEFAULT '0',
  `value` int(11) DEFAULT NULL,
  `timestamp` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_src`,`role_des`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


-- ----------------------------
-- Table structure for `UMS_ACHIEVE`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_ACHIEVE`;
CREATE TABLE `UMS_ACHIEVE` (
  `role_id` int(11) NOT NULL DEFAULT '0',
  `achieve_info` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for UMS_ACHIEVE_REMOVED
-- ----------------------------
DROP TABLE IF EXISTS `UMS_ACHIEVE_REMOVED`;
CREATE TABLE `UMS_ACHIEVE_REMOVED` (
  `role_id` int(11) NOT NULL DEFAULT '0',
  `achieve_info` blob,
  PRIMARY KEY (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE  IF EXISTS `GMS_IBSTORE_PROMOTION`;
CREATE TABLE GMS_IBSTORE_PROMOTION
(
	goods_id int PRIMARY KEY,
	locked_count int NOT NULL DEFAULT 0,
	elapsed_time int NOT NULL DEFAULT 0,
	update_time timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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


DROP TABLE IF EXISTS `UMS_WORLDCHAR`;
CREATE TABLE `UMS_WORLDCHAR` (
  `world_id` int(11) NOT NULL,
  `charid` int(11) NOT NULL DEFAULT '0',
  `honor` int(11) NOT NULL DEFAULT '0',
  `killnum` int(11) NOT NULL DEFAULT '0',
  `rank` int(11) NOT NULL DEFAULT '0',
  `metier` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY(`world_id`, `metier`, `rank`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `UMS_WORLDGLOBAL`;
CREATE TABLE `UMS_WORLDGLOBAL` (
  `war_serial` int(11) NOT NULL DEFAULT '0',
  `war_status` int(11) NOT NULL,
  `war_titletime` int(11) NOT NULL,
  `war_winworld` int(11) NOT NULL,
  `tax_setrate` int(11) NOT NULL DEFAULT '-1',
  `tax_taxrate` int(11) NOT NULL DEFAULT '5',
  `tax_taxmoney` int(11) NOT NULL,
  `tax_corpsmoney` int(11) NOT NULL,
  `tax_settimes` int(11) NOT NULL,
  `war_nosubmit` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
  `bourse_trade_detail` bigint NOT NULL DEFAULT '0',
  `account_id` int(11) NOT NULL,
  `price` int(11) DEFAULT '0',
  `yb_count` int(11) DEFAULT '0',
  `trade_yb` int(11) DEFAULT '0',
  `trade_kind` int(11) DEFAULT '0',
  `trade_type` int(11) DEFAULT '0',
  `bourse_date` int(11) DEFAULT '0',
  `bourse_tax` int(11) DEFAULT '0',
  `role_id`    int(11) DEFAULT '0',
  `account_game_id` int(11) DEFAULT '0',
  PRIMARY KEY (`bourse_doc_no`),
  KEY `IDX_BOURSE_TRADE_BY_ACCT` (`account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `UMS_BOURSE_LOG`;
/*Table structure for table `UMS_BOURSE_LOG` */

CREATE TABLE `UMS_BOURSE_LOG` (
  `bourse_detail` int(11) NOT NULL AUTO_INCREMENT,
  `bourse_buy_detail` bigint(20) NOT NULL,
  `bourse_sell_detail` bigint(20) NOT NULL,
  `sell_price` int(11) NOT NULL,
  `sell_account` int(11) NOT NULL,
  `sell_role` int(11) NOT NULL,
  `buy_price` int(11) NOT NULL,
  `buy_account` int(11) NOT NULL,
  `buy_role` int(11) NOT NULL,
  `yb_count` int(11) DEFAULT '0',
  `bourse_date` int(11) DEFAULT '0',
  `rating_id` int(11) NOT NULL DEFAULT '0',
  `world_id` int(11) DEFAULT '0',
  `type` int(11) NOT NULL DEFAULT '1',
  `buy_account_game_id` int(11) DEFAULT '0',
  `sell_account_game_id` int(11) DEFAULT '0',
  PRIMARY KEY (`bourse_detail`,`rating_id`),
  KEY `IDX_BOURSE_LOG_BY_BUY_ACCOUNT` (`buy_account`),
  KEY `IDX_BOURSE_LOG_BY_SELL_ACCOUNT` (`sell_account`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8;


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
  `bourse_tax`      int(11) DEFAULT '0',
  `role_id`    int(11) DEFAULT '0',
  `account_game_id` int(11) DEFAULT '0',
  PRIMARY KEY (`bourse_trade_detail`),
  KEY `IDX_BOURSE_TRADE_HISTORY_BY_ACCT` ( `account_id` )
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `UMS_BOURSE_LOG_LOCKED`;
/*Table structure for table `UMS_BOURSE_LOG_LOCKED` */
CREATE TABLE `UMS_BOURSE_LOG_LOCKED` (
  `bourse_detail` int(11) NOT NULL,
  `sell_account` int(11) NOT NULL DEFAULT '0',
  `sell_role` int(11) NOT NULL DEFAULT '0',
  `buy_account` int(11) NOT NULL DEFAULT '0',
  `buy_role` int(11) NOT NULL DEFAULT '0',
  `yb_count` int(11) DEFAULT '0',
  `rating_id` int(11) NOT NULL DEFAULT '0',
  `world_id` int(11) DEFAULT '0',
  `bourse_date` int(11) DEFAULT '0',
  `failure_times` int(11) DEFAULT '0',
  `type` int(11) NOT NULL DEFAULT '1',
  `buy_account_game_id` int(11) DEFAULT '0',
  `sell_account_game_id` int(11) DEFAULT '0',
  PRIMARY KEY (`bourse_detail`,`rating_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


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



/*元宝摆摊记录表*/
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

-- ----------------------------
-- Table structure for `UMS_ACTIVITY_YQS_CAMPINFO`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_ACTIVITY_YQS_CAMPINFO`;
CREATE TABLE `UMS_ACTIVITY_YQS_CAMPINFO` (
  `ID` int(11) NOT NULL COMMENT '活动索引',
  `CampID` int(11) NOT NULL COMMENT '阵营ID',
  `CampRank` int(11) DEFAULT NULL,
  `CurrHandinDawNum` int(11) DEFAULT '0' COMMENT '当前上缴仙露个数',
  `CurrGrowing` int(11) DEFAULT '0' COMMENT '当前成长度',
  `CurrLevel` int(11) DEFAULT '0' COMMENT '当前等级',
  `SettlePhase` int(11) DEFAULT '-1' COMMENT '所处的结算阶段',
  PRIMARY KEY (`ID`,`CampID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `UMS_ACTIVITY_YQS_INDEX`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_ACTIVITY_YQS_INDEX`;
CREATE TABLE `UMS_ACTIVITY_YQS_INDEX` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `ActivityID` int(11) NOT NULL,
  `StartTime` datetime NOT NULL,
  `EndTime` datetime DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `UMS_ACTIVITY_YQS_STOREBOXINFO`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_ACTIVITY_YQS_STOREBOXINFO`;
CREATE TABLE `UMS_ACTIVITY_YQS_STOREBOXINFO` (
  `PlayerId` int(11) NOT NULL COMMENT '用户ID',
  `ActivityIndex` int(11) NOT NULL,
  `SettlePhase` int(11) NOT NULL,
  `StoreBoxId` int(11) NOT NULL,
  PRIMARY KEY (`PlayerId`,`ActivityIndex`,`SettlePhase`,`StoreBoxId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for `UMS_ACTIVITY_YQS_USERINFO`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_ACTIVITY_YQS_USERINFO`;
CREATE TABLE `UMS_ACTIVITY_YQS_USERINFO` (
  `PlayerId` int(11) NOT NULL COMMENT '玩家ID',
  `AcitivityIndex` int(11) NOT NULL COMMENT '摇钱树活动索引（指定某次摇钱树活动与UMS_ACITIVITY_YQS中的ID字段对应）',
  `Contribute` int(11) DEFAULT NULL COMMENT '玩家对本次摇钱树活动的贡献值',
  `HandInNum` int(11) DEFAULT NULL COMMENT '用户上缴仙露数',
  PRIMARY KEY (`PlayerId`,`AcitivityIndex`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- View structure for `v_activity_yqs`
-- ----------------------------
DROP VIEW IF EXISTS `v_activity_yqs`;
CREATE VIEW `v_activity_yqs` AS select `UMS_ACTIVITY_YQS_INDEX`.`ID` AS `ID`,`UMS_ACTIVITY_YQS_INDEX`.`ActivityID` AS `ActivityID`,`UMS_ACTIVITY_YQS_INDEX`.`StartTime` AS `StartTime`,`UMS_ACTIVITY_YQS_INDEX`.`EndTime` AS `EndTime`,`UMS_ACTIVITY_YQS_CAMPINFO`.`CampID` AS `CampID`,`UMS_ACTIVITY_YQS_CAMPINFO`.`CampRank` AS `CampRank`,`UMS_ACTIVITY_YQS_CAMPINFO`.`CurrHandinDawNum` AS `CurrHandinDawNum`,`UMS_ACTIVITY_YQS_CAMPINFO`.`CurrGrowing` AS `CurrGrowing`,`UMS_ACTIVITY_YQS_CAMPINFO`.`CurrLevel` AS `CurrLevel`,`UMS_ACTIVITY_YQS_CAMPINFO`.`SettlePhase` AS `SettlePhase` from (`UMS_ACTIVITY_YQS_INDEX` join `UMS_ACTIVITY_YQS_CAMPINFO`) where (`UMS_ACTIVITY_YQS_INDEX`.`ID` = `UMS_ACTIVITY_YQS_CAMPINFO`.`ID`);

-- ----------------------------
-- View structure for `v_activity_yqs_playerrankinfo`
-- ----------------------------
DROP VIEW IF EXISTS `v_activity_yqs_playerrankinfo`;
CREATE VIEW `v_activity_yqs_playerrankinfo` AS select `UMS_ACTIVITY_YQS_INDEX`.`ID` AS `ID`,`UMS_ACTIVITY_YQS_USERINFO`.`PlayerId` AS `role_id`,`UMS_ROLE`.`role_name` AS `role_name`,`UMS_ROLE`.`metier_id` AS `metier_id`,`UMS_ROLE`.`role_level` AS `role_level`,`UMS_ACTIVITY_YQS_USERINFO`.`Contribute` AS `Contribute` from ((`UMS_ACTIVITY_YQS_INDEX` join `UMS_ACTIVITY_YQS_USERINFO`) join `UMS_ROLE`) where ((`UMS_ACTIVITY_YQS_INDEX`.`ID` = `UMS_ACTIVITY_YQS_USERINFO`.`AcitivityIndex`) and (`UMS_ACTIVITY_YQS_USERINFO`.`PlayerId` = `UMS_ROLE`.`role_id`));


-- ----------------------------
-- Table structure for `UMS_ROLE_SWORN`
-- ----------------------------
DROP TABLE IF EXISTS `UMS_ROLE_SWORN`;
CREATE TABLE `UMS_ROLE_SWORN` (
  `sworn_id` int(11) NOT NULL,
  `sworn_name` varchar(31) DEFAULT NULL,
  `camp_id` int(11) DEFAULT NULL,
  `sworn_member` blob,
  PRIMARY KEY (`sworn_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
