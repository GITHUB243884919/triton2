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
