-- MySQL dump 10.13  Distrib 5.1.34, for pc-linux-gnu (i686)
--
-- Host: localhost    Database: LogDB
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
-- Table structure for table `SYS_CODE_TYPE`
--

DROP TABLE IF EXISTS `SYS_CODE_TYPE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `SYS_CODE_TYPE` (
  `type_id` int(11) NOT NULL,
  `type_code` char(32) NOT NULL,
  `type_name` char(32) NOT NULL,
  `type_desc` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`type_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `SYS_CODE_VALUE`
--

DROP TABLE IF EXISTS `SYS_CODE_VALUE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `SYS_CODE_VALUE` (
  `type_id` int(11) NOT NULL,
  `value_id` int(11) NOT NULL,
  `value_name` char(32) NOT NULL,
  `value_code` varchar(32) NOT NULL,
  `code_group` int(11) DEFAULT NULL,
  `value_desc` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`type_id`,`value_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Dumping routines for database 'LogDB'
--

--
-- Dumping data for table `SYS_CODE_TYPE`
--

LOCK TABLES `SYS_CODE_TYPE` WRITE;
/*!40000 ALTER TABLE `SYS_CODE_TYPE` DISABLE KEYS */;

INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (1, 'LOG_MSG_ID', '日志表类型', '');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (2, 'EMLeaveReason', '离开原因', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (3, 'EmKickOutReason', '踢人原因', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (4, 'EMSaveResult', '存盘方式', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (5, 'EMSaveReason', '存盘原因', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (6, 'EPackType', '包裹类型', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (7, 'EItemLife', '物品生命周期', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (8, 'EMoneyType', '金钱类型', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (9, 'EItemAction', '操作物品类型', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (10, 'EUpgradeType', '经验类型', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (11, 'EExpAction', '获得经验方法', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (12, 'EM_TABLE_ID', '监控表类别', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (13, 'EM_STAT_ENTITY', '实体类型', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (14, 'EMoneyAction', '角色金钱行为', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (15, 'ETaskAction', '角色任务行为', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (16, 'EFamilyAttr', '家族属性枚举', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (17, 'ECorpsAttr', '军团属性', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (18, 'EOrganAction', '组织行为枚举', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (19, 'EM_MAILACTION', '玩家邮件行为', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (20, 'EM_STAT_CATALOG', '监控值类别', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (21, 'ERoleItemLife', '玩家对物品的拥有状态', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (22, 'ITEMTYPE', '物品类型', NULL);
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (23, 'ITEM_EQUIPUPG.LEVEL', '升级物品的级别', 'CTplItemEquipUpg.mLimit');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (24, 'ITEM_EQUIPUPG.SUBTYPE', '升级物品的子类型', 'mType << 4 + mUpType');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (25, 'ITEM_UPGLEAST.SUBTYPE', '使用的类型', 'CTplUpgLeast.mType');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (26, 'ITEM_PHYSIC.SUBTYPE', '生产类型', 'CTplItemPhysic.mShowType');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (27, 'ITEM_BOOK.SUBTYPE', '技能书颜色', 'CTplItemSkillBook.mBookColor');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (28, 'ITEM_WEAPON.SUBTYPE', '武器装备类型', 'mShowColor << 4 + mWeaponType');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (29, 'ITEM_MAGICWEAPON.SUBTYPE', '法宝类型', 'CTemplateEquipment.mShowColor');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (30, 'ITEM_ARMOR.SUBTYPE', '防护装备类型', 'mShowColor << 4 + mShowType');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (31, 'ITEM_RUNE.SUBTYPE', '符文套装类型', 'CTplItemRune.mSuitID');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (32, 'ITEM_JEWEL.SUBTYPE', '珠宝的颜色', 'CTplItemJewel.mShowColor');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (33, 'ITEM_CARD.SUBTPYE', '卡片类型', 'CTplItemCard.mType');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (34, 'ITEM_KITBAG.SUBTYPE', '背包格数', 'CTplKitBag.mIndexNum');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (35, 'ITEM_FUNCITEM.SUBTYPE', '功能物品类型', 'CTplFuncItem.mFuncCode');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (36, 'ITEM_SKILLBOOK.SUBTYPE', '法宝技能书颜色', 'CTplItemSkillBook.mBookColor');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (37, 'ITEM_REDPAPER.SUBTYPE', '红包类型', 'CTplItemRedPaper.mRedPaperType');
INSERT INTO `SYS_CODE_TYPE` (`type_id`, `type_code`, `type_name`, `type_desc`) VALUES (40, 'ERoleAction', '角色行为', '未定义');


/*!40000 ALTER TABLE `SYS_CODE_TYPE` ENABLE KEYS */;
UNLOCK TABLES;


--
-- Dumping data for table `SYS_CODE_VALUE`
--

LOCK TABLES `SYS_CODE_VALUE` WRITE;
/*!40000 ALTER TABLE `SYS_CODE_VALUE` DISABLE KEYS */;
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 1, '物品升级日志', 'ID_S2L_ITEM_UPGRADE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 2, '物品操作日志', 'ID_S2L_ROLE_OPITEM', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 3, '角色任务日志', 'ID_S2L_ROLE_OPTASK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 4, '角色死亡日志', 'ID_S2L_ROLE_KILLED', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 5, '角色登录日志', 'ID_S2L_ROLE_LOGOUT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 6, '角色获得经验日志', 'ID_S2L_ROLE_OPEXP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 7, '角色升级日志', 'ID_S2L_ROLE_UPGRADE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 8, '角色镜像日志', 'ID_S2L_ROLE_GHOST', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 9, '角色金钱日志', 'ID_S2L_ROLE_OPMONEY', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 10, '怪物掉落日志', 'ID_S2L_ORGE_DROPED', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 11, '物品消失日志', 'ID_S2L_ITEM_DISPEAR', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 12, '家族操作日志', 'ID_S2L_FAMILY_ACTION', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 13, '军团操作日志', 'ID_S2L_CORPS_ACTION', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (1, 14, '角色邮件日志', 'ID_S2L_MAIL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (2, 1, '跨场景离开', 'EM_LEAVE_REDIRECT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (2, 2, '返回网关', 'EM_LEAVE_RETURN2GATE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (2, 3, '离开游戏', 'EM_LEAVE_LEAVEGAME', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (2, 4, '被踢离开', 'EM_LEAVE_KICKOUT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (3, 1, '反外挂踢人', 'EM_KICKOUT_ANTIBOT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (3, 2, '玩家数据异常踢人', 'EM_KICKOUT_LOCKDATA', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (3, 3, '服务器停机维护踢人', 'EM_KICKOUT_SERVERMAINTANCE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (3, 4, 'GM踢人', 'EM_KICKOUT_GM', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (3, 5, '玩家帐号被托管踢人', 'EM_KICKOUT_ACCOUNTTRUSTEE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (3, 6, '玩家行为异常踢人', 'EM_KICKOUT_UNKOWNACTION', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (3, 7, '玩家重复登录踢人', 'EM_KICKOUT_RELOGIN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (3, 8, '玩家网管数据不存在', 'EM_KICKOUT_NOTINGATE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (3, 9, '存盘超时踢人', 'EM_KICKOUT_SAVETIMEOUT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (3, 10, '帐号被冻结踢人', 'EM_KICKOUT_ACCOUNTFROZEN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (4, 1, '存储至数据库', 'EM_SAVE2DB', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (4, 2, '存储至文件', 'EM_SAVE2FILE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (4, 3, '未存储', 'EM_NOSAVE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (5, 1, '跨服引起的存盘', 'SAVE_REDIRECT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (5, 2, '返回网关引起的存盘', 'SAVE_RETURN2GATE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (5, 3, '离开游戏引起的存盘', 'SAVE_LEAVEGAME', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (5, 4, '被踢引起的存盘', 'SAVE_KICKOUT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (5, 5, '定时存盘', 'SAVE_TIMETRIGER', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (5, 6, '玩家直接关闭客户端', 'SAVE_FORCEEXIT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (5, 7, '写文件引起的存盘', 'SAVE_FILE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (6, 0, '玩家装备', 'EM_ROLE_EQUIPMENT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (6, 1, '随身背包', 'EM_ROLE_PACKAGE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (6, 2, '随身扩展包', 'EM_ROLE_PACKAGE_KITBAG', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (6, 3, '仓库', 'EM_ROLE_STORAGE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (6, 4, '仓库扩展包', 'EM_ROLE_STORAGE_KITBAG', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (7, 1, '生产领域', 'EM_PRODUCE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (7, 2, '流通行为', 'EM_CIRCULATION', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (7, 3, '回收行为', 'EM_RECYCLE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (8, 1, '绑定金', 'em_bind_money', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (8, 2, '非绑定金', 'em_unbind_money', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (8, 3, '金元宝', 'em_golden_yuanbao', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (8, 4, '银元宝', 'em_silver_yuanbao', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 1, '从商城获得物品', 'EM_GET_BY_IBSTORE', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 2, '收邮件获得物品', 'EM_GET_BY_RECV_MAIL', 8, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 3, '通过其他玩家摊位买到物品', 'EM_GET_BY_STALL_BUY', 8, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 4, '通过玩家间交换得到', 'EM_GET_BY_EXCHANGE', 8, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 5, '接受任务得到物品', 'EM_GET_BY_ACCEPT_TASK', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 6, '交付任务得到物品', 'EM_GET_BY_END_TASK', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 7, '从活动NPC处得到物品', 'EM_GET_BY_ERATING', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 8, '从地上捡起物品', 'EM_GET_BY_PICKITEM', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 9, '玩家生产得到物品', 'EM_GET_BY_PRODUCE', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 10, '物品转化后获得', 'EM_GET_BY_CHANGE', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 11, '进行任务中获得物品', 'EM_GET_BY_RUN_TASK', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 12, '通过从NPC处购买获得', 'EM_GET_BY_NPC_SHOP', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 13, '默认初始装备', 'EM_GET_BY_DEFAULT', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 14, '任务物品获得', 'EM_GET_BY_TASKITEM', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 15, '采集获得', 'EM_GET_BY_COLLECT', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 16, '合成获得', 'EM_GET_BY_COMPOSE', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 17, '副本奖励', 'EM_GET_BY_REPETION', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 18, 'GMINSERT', 'EM_GET_BY_GMINSERT', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 19, '操作失败后补偿给玩家', 'EM_GET_BY_ROLLBACK', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 20, '开宝鼎直接获得', 'EM_GET_BY_DIRECTINS', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 21, '解除绑定获得宝石', 'EM_GET_BY_UNBOUND', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 22, '分解后获得', 'EM_GET_BY_DECOMPOUND', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 23, '调查问卷奖励', 'EM_GET_BY_INVESTIGATION', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 24, '挂机购买', 'EM_GET_BY_GUAJI_SHOP', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 25, '从NPC处回购物品', 'EM_GET_BY_BUY_BACK', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 26, '领取副本奖励', 'EM_GET_BY_AWARD', 4, '0');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 27, '摘星获得', 'EM_GET_BY_GETSTAR', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 28, '荣誉兑换获得', 'EM_GET_BY_HORNOR', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 48, '从系统邮件获得', 'EM_GET_ITEM_BY_SYSMAIL', 4, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 50, '发送邮件失去物品', 'EM_LOST_BY_SEND_MAIL', 8, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 51, '通过摆摊卖掉物品', 'EM_LOST_BY_STALL_SELL', 8, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 52, '通过玩家间交易失去', 'EM_LOST_BY_EXCHANGE', 8, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 53, '接受任务失去物品', 'EM_LOST_BY_ACCEPT_TASK', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 54, '交付任务失去物品', 'EM_LOST_BY_END_TASK', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 55, '从活动NPC处失去物品', 'EM_LOST_BY_ERATING', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 56, '玩家丢弃销毁物品', 'EM_LOST_BY_THROW_ITEM', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 57, '玩家通过生产失去材料', 'EM_LOST_BY_PRODUCE', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 58, '使用后失去物品', 'EM_LOST_BY_USE', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 59, '开槽后失去物品', 'EM_LOST_BY_OPEN_SLOT', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 60, '合成后失去物品', 'EM_LOST_BY_COMPOSE', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 61, '吃掉物品化成元气', 'EM_LOST_BY_EAT', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 62, '使用宝石镶嵌物品', 'EM_LOST_BY_INSERT_SLOT', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 63, '精炼物品', 'EM_LOST_BY_UPGRADE', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 64, '转化后失去', 'EM_LOST_BY_CHANGE', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 65, '分解后失去', 'EM_LOST_BY_DECOMPOUND', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 66, '卖给NPC失去', 'EM_LOST_BY_SELL2NPC', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 67, '鉴定后失去', 'EM_LOST_BY_JUDGE', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 68, '滴血后失去', 'EM_LOST_BY_BOUND', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 69, '放弃任务后失去', 'EM_LOST_BY_ABORTTASK', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 70, '玩家直接销毁物品', 'EM_LOST_BY_DESTROY', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 71, '限时物品系统回收', 'EM_LOST_BY_TIMEOUT', 5, '0');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 72, '装备转换', 'EM_LOST_BY_CHANGE_EQUIP', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 73, '失去副本钥匙', 'EM_LOST_BY_REPETION', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 74, '摘星后失去', 'EM_LOST_BY_GETSTAR', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 75, '加星后失去', 'EM_LOST_BY_ADDSTAR', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 76, '印鉴后失去', 'EM_LOST_BY_ADDEXTRA', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 77, '学习方寸山技能', 'EM_LOST_BY_FANGCUN', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (9, 78, '解除拜师失去', 'EM_LOST_BY_MASTER', 5, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (10, 0, '人物等级升级经验', 'EM_UPGRADE_ROLE_EXP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (10, 1, '人物修为升级经验', 'EM_UPGRADE_TALENT_EXP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (10, 2, '人物生活技能升级熟练度', 'EM_UPGRADE_LIFE_EXP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (10, 3, '人物生产技能升级熟练度', 'EM_UPGRADE_PRODUCE_EXP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (10, 4, '人物法宝升级经验', 'EM_UPGRADE_MW_EXP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 1, '荣誉转换为经验', 'EM_EXP_BY_HORNOR', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 2, '卡片获得经验', 'EM_EXP_BY_CARD', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 3, '兑换获得经验', 'EM_EXP_BY_MULTICOM', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 4, 'GM命令获得', 'EM_EXP_BY_COMMAND', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 5, '内丹获得经验', 'EM_EXP_BY_TEAM_ALLOT', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 6, '家族内丹', 'EM_EXP_BY_FAMILY_ALLOT', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 7, '答题卷', 'EM_EXP_BY_QUESTION', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 8, '调查问卷', 'EM_EXP_BY_INVESTIGATION', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 9, '天赐官奖励', 'EM_EXP_BY_ERATING', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 10, '任务奖励', 'EM_EXP_BY_TASK', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 11, '杀死怪物获得', 'EM_EXP_BY_ENTITY_DIE', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 12, '持续加经验固定值BUFF', 'EM_EXP_BY_BUFF', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 13, '送人鲜花获得经验', 'EM_EXP_BY_FLOWER', 3, '0');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 14, '使用经验卷卡获得', 'EM_EXP_BY_EXPITEM', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 15, '使用黑狗丸', 'EM_EXP_BY_BLACKPILLS', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 16, '使用经验球', 'EM_EXP_BY_EXPILLS', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (11, 17, '上缴物品获取经验', 'EM_EXP_BY_HANDIN', 3, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (12, 1, '金钱产出监控表', 'LOG_MONEY_PRODUCE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (12, 2, '金钱回收监控表', 'LOG_MONEY_RECYLE ', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (12, 3, '经验产出监控表', 'LOG_EXP_PRODUCE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (12, 4, '物品产出监控表', 'LOG_ITEM_PRODUCE ', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (12, 5, '物品回收监控表', 'LOG_ITEM_RECYCLE ', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (12, 6, '人物行为监控表', 'LOG_ACTION_MONIT ', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (13, 1, '监控实体角色', 'EM_STAT_ROLE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (13, 2, '监控实体家族', 'EM_STAT_FAMILY', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (13, 3, '监控实体军团', 'EM_STAT_CORPS', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 1, '卖给NPC', 'EM_GET_BY_SELL2NPC', 1, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 2, '从地上捡起', 'EM_GET_BY_PICKMONEY', 1, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 3, '交易获得', 'EM_GET_BY_EXCHANGE_MONEY', 7, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 4, 'GM插入', 'EM_GET_BY_GMINSERT_MONEY', 1, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 5, '从仓库支取金钱', 'EM_GET_BY_STORE_MONEY', 7, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 6, '读取红包', 'EM_GET_BY_RED_BAG', 1, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 7, '收邮件得到', 'EM_GET_BY_RECV_MONEY', 7, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 8, '每日答题', 'EM_GET_BY_QUESTLIB', 1, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 9, 'Erating奖励', 'EM_GET_BY_ERATING_MONEY', 1, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 10, '从军团获取', 'EM_GET_BY_CORPS', 7, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 11, '从家族获取', 'EM_GET_BY_FAMILY', 7, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 12, '摆摊获得', 'EM_GET_BY_STALL', 7, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 13, '从任务奖励获得', 'EM_GET_BY_TASK', 1, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 14, '从礼包获得', 'EM_GET_BY_GIFT', 1, '0');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 15, '国战领奖获得', 'EM_GET_BY_WAR', 1, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 16, '从元宝交易所获得', 'EM_GET_BY_BOURSE', 7, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 17, '元宝交易所存钱失败', 'EM_GET_BY_BOURSE_ERR', 7, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 18, '传送失败系统回滚', 'EM_GET_BY_TELEPORT_FAIL', 1, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 48, '从系统邮件获得', 'EM_GET_MONEY_BY_SYSMAIL', 1, '0');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 49, '兑换金珊瑚获得', 'EM_GET_BY_SELL_JINSHANHU', 1, '0');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 50, '邮件', 'EM_LOST_BY_SEND_MONEY', 7, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 51, '从NPC处购买', 'EM_LOST_BY_BUY4NPC', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 52, '修理装备', 'EM_LOST_BY_REPAIR', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 53, '生产装备', 'EM_LOST_BY_PRODUCE_EQUIP', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 54, '生产药品', 'EM_LOST_BY_PRODUCE_PHYS', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 55, '升级装备', 'EM_LOST_BY_UPGRADE_EQUIP', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 56, '镶嵌符文', 'EM_LOST_BY_INSERT_RUNE', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 57, '镶嵌宝石', 'EM_LOST_BY_INSERT_JEWEL', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 58, '装备开槽', 'EM_LOST_BY_OPEN_SLOT2', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 59, '学习天赋', 'EM_LOST_BY_STUDY_TALENT', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 60, '交换失去', 'EM_LOST_BY_EXCHANGE_MONEY', 7, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 61, '金钱复活', 'EM_LOST_BY_MONEY_RELIFE', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 62, 'NPC传送', 'EM_LOST_BY_NPC_TELEPORT', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 63, '回购物品', 'EM_LOST_BY_BUY_BACK', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 64, '绑定物品', 'EM_LOST_BY_BIND_ITEM', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 65, '解除绑定', 'EM_LOST_BY_UNBIND_ITEM', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 66, '学习技能', 'EM_LOST_BY_STUDY_SKILL', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 67, '向仓库存钱', 'EM_LOST_BY_STORE_MONEY', 7, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 68, '恢复绑定', 'EM_LOST_BY_RESUME_BIND', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 69, '摘除宝石', 'EM_LOST_BY_REMOVE_JEWEL', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 70, '鉴定装备', 'EM_LOST_BY_JUDGE_EQUIP', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 71, '转换抗性', 'EM_LOST_BY_CHANGE_RESIST', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 72, '合成宝石', 'EM_LOST_BY_COMPOSE_JEWEL', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 73, '兑换', 'EM_LOST_BY_MULTI_COMPOSE', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 75, '玩家升级生活技能消费', 'EM_LOST_BY_UP_LIFESKILL', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 76, '学习配方', 'EM_LOST_BY_STUDY_COMPOSE', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 77, '鉴定法宝', 'EM_LOST_BY_JUDGE_FABAO', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 78, '挂机买药', 'EM_LOST_BY_GUAJI_BUY', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 79, '发送邮件邮资', 'EM_LOST_BY_POSTAL_FEE', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 80, '创建军团', 'EM_LOST_BY_BUILD_CORPS', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 81, '向军团捐献', 'EM_LOST_BY_CONTR_CORPS', 7, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 82, '将建家族', 'EM_LOST_BY_BUILD_FAMILY', 2, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 83, '向家族捐献', 'EM_LOST_BY_CONTR_FAMILY', 7, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 84, '从摆摊处失去', 'EM_LOST_BY_STALL', 7, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 85, '装备转换', 'EM_LOST_BY_EQIP_CHANGE', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 86, '装备印鉴', 'EM_LOST_BY_ADD_EXTRA', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 87, '国战报名', 'EM_LOST_BY_WAR', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 88, '向元宝交易所存钱', 'EM_LOST_BY_BOURSE', 7, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 89, '元宝交易所扣税', 'EM_LOST_BY_TAX', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 90, '摆摊交易手续费', 'EM_LOST_BY_STALL_FEE', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (14, 91, '拜师失去', 'EM_LOST_BY_MASTERMONEY', 2, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (15, 1, '接受任务', 'EM_TASK_ACCEPT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (15, 2, '交付任务', 'EM_TASK_END', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (15, 3, '放弃任务', 'EM_TASK_CANCEL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (16, 1, '成员数', 'EM_FAMILY_MEMBER', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (16, 2, '金库', 'EM_FAMILY_MONEY', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (16, 3, '挑战赛', 'EM_FAMILY_CHANLENGE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (16, 4, '争霸赛', 'EM_FAMILY_BATTLE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (16, 5, '成立状态', 'EM_FAMILY_CREATION', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (16, 6, '贡献度', 'EM_FAMILY_CONTRIBUTION', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (16, 7, '等级', 'EM_FAMILY_LEVEL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (16, 8, '职位', 'EM_FAMILY_RANK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (17, 1, '成员数', 'EM_CORPS_MEMBER', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (17, 2, '金库', 'EM_CORPS_MONEY', NULL, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (17, 3, '挑战赛', 'EM_CORPS_CHANLENGE', NULL, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (17, 4, '争霸赛', 'EM_CORPS_BATTLE', NULL, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (17, 5, '成立状态', 'EM_CORPS_CREATION', NULL, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (17, 6, '贡献度', 'EM_CORPS_CONTRIBUTION', NULL, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (17, 7, '等级', 'EM_CORPS_LEVEL', NULL, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (17, 8, '职位', 'EM_CORPS_RANK', NULL, '');
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 1, '成员加入', 'EM_ORGAN_MEMBERJOIN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 2, '成员离开', 'EM_ORGAN_MEMBERLEAVE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 3, '家族(军团)成立', 'EM_ORGAN_CREATE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 4, '家族(军团)解散', 'EM_ORGAN_DISBAND', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 5, '获取家族(军团)金钱', 'EM_ORGAN_GETMONEY', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 6, '贡献家族(军团)金钱', 'EM_ORGAN_CONTRIBUTEMONEY', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 7, '购买家族(军团)物品(红包)', 'EM_ORGAN_BUYITEM', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 8, '家族(军团)升级', 'EM_ORGAN_LEVELUP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 9, '家族(军团)职位设定', 'EM_ORGAN_SETPOST', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 10, '开房间(报名)', 'EM_ORGAN_OPEN_ROOM', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 11, '关房间(取消报名)', 'EM_ORGAN_CLOSE_ROOM', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 12, '发起挑战', 'EM_ORGAN_SEND_CHALLEGE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 13, '接受挑战', 'EM_ORGAN_REC_CHALLEGE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 14, '竞价', 'EM_ORGAN_BID_', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 15, '报名', 'EM_ORGAN_SUBMIT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 16, '开战', 'EM_ORGAN_JOIN_BATTLE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 17, '获取家族(军团)npc福利', 'EM_ORGAN_GETNPCWELFARE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 18, '家族(军团)任务', 'EM_ORGAN_TASK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (18, 19, '家族(军团)兑换', 'EM_ORGAN_EXCHANGE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (19, 1, '发送邮件', 'EM_MAIL_SENDMAIL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (19, 2, '收邮件', 'EM_MAIL_RECEIVEMAIL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (19, 3, '删除邮件', 'EM_MAIL_DELMAIL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (20, 1, '个人金钱', 'EM_SERVER_MONEY', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (20, 2, '个人经验', 'EM_SERVER_EXP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (20, 3, '个人物品', 'EM_SERVER_ITEM', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (20, 4, '个人行为', 'EM_SERVER_EVENT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (20, 5, '家族属性', 'EM_SERVER_FAMILY', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (20, 6, '军团属性', 'EM_SERVER_CORPS', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (21, 1, '获得', 'EM_OBTAIN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (21, 2, '失去', 'EM_LOSE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 1, '回城卷轴', 'ITEM_RETURN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 2, '传送物品', 'ITEM_TELEPORT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 3, '装备升级物品', 'ITEM_EQUIPUPG', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 4, '复活卷轴', 'ITEM_RELIFE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 5, '宠物蛋', 'ITEM_PETEGG', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 6, '宠物食品', 'ITEM_PETFOOD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 7, '装备升级幸运符', 'ITEM_UPGLUCK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 8, '装备升级保底符', 'ITEM_UPGLEAST', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 9, '刷任务用道具', 'ITEM_REFRESHTASK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 10, '任务道具', 'ITEM_TASK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 11, '生产原料', 'ITEM_PRODUCE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 12, '喇叭喊话道具', 'ITEM_SHOUT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 13, '药品', 'ITEM_PHYSIC', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 14, '调用技能的物品', 'ITEM_SKILL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 15, '技能书', 'ITEM_BOOK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 16, '增加经验道具', 'ITEM_EXP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 17, '武器装备', 'ITEM_WEAPON', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 18, '防护装备', 'ITEM_ARMOR', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 19, '时装装备', 'ITEM_FASHION', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 20, '符文道具', 'ITEM_RUNE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 21, '珠宝道具', 'ITEM_JEWEL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 22, '元气虫道具', 'ITEM_YUANBUG', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 23, '元气蛋道具', 'ITEM_YUANEGG', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 24, '特殊任务物品', 'ITEM_SPECIAL_TASK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 25, '自动喊话道具', 'ITEM_AUTOSHOUT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 26, '坐骑道具', 'ITEM_HORSE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 27, '绑定石', 'ITEM_BINDSTONE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 28, '仙轴', 'ITEM_SCROLL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 29, '卡片', 'ITEM_CARD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 30, '指定传送卷', 'ITEM_RETURNPAPER', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 31, '指定传送符', 'ITEM_TELESYMBOL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 32, '内丹', 'ITEM_REDSTONE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 33, '指定炉石', 'ITEM_STOVE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 34, '题卷', 'ITEM_QUESTIONPAPER', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 35, '藏宝图', 'ITEM_MINEMAP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 36, '惩妖令牌', 'ITEM_TOKEN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 37, '背包', 'ITEM_KITBAG', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 38, '宝石摘除符', 'ITEM_REMOVE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 39, '抗性转化石', 'ITEM_CHANGE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 40, '物品适配器', 'ITEM_ADAPTER', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 41, '召唤类物品', 'ITEM_CALL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 42, '礼包类物品', 'ITEM_GIFTBAG', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 43, '点金石', 'ITEM_GOLDSTONE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 44, '采集的植物', 'ITEM_PLANT', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 45, '采集产生的矿物', 'ITEM_MINERAL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 46, '法宝', 'ITEM_MAGICWEAPON', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 47, '附魔物品', 'ITEM_MAGICSTONE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 48, '生活技能配方卷轴', 'ITEM_LIFESKILLSCROLL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 49, '功能性物品', 'ITEM_FUNCITEM', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 50, '法宝技能书', 'ITEM_SKILLBOOK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 51, '家族将军令', 'ITEM_CORPSTOKEN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (22, 52, '红包', 'ITEM_REDPAPER', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (23, 0, '低', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (23, 1, '中', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (23, 2, '高', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (24, 0, '精炼装备等级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (24, 1, '精炼法宝等级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (24, 16, '精炼装备抗性', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (24, 17, '精炼装备攻击', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (24, 32, '精炼法宝抗性', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (24, 33, '精炼法宝攻击', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (25, 0, '装备', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (25, 1, '宝石', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 0, '无类别', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 100, '红1级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 101, '红2级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 102, '红3级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 103, '红4级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 104, '蓝1级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 105, '蓝2级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 106, '蓝3级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 107, '蓝4级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 108, '紫1级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 109, '紫2级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 110, '紫3级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 111, '紫4级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 112, '黄1级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 113, '黄2级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 114, '黄3级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 115, '黄4级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 116, '白1级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 117, '白2级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 118, '白3级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 119, '白4级', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 120, '经验药', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (26, 121, '生活技能药', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (27, 0, '无限制', 'COLOR_NONE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (27, 1, '白色', 'COLOR_WHITE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (27, 2, '蓝色', 'COLOR_BLUE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (27, 3, '紫色', 'COLOR_PURPLE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (27, 4, '金色', 'COLOR_GOLD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 0, '白色枪', 'WHITE_WEAPON_SPEAR', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 1, '白色双刀', 'WHITE_WEAPON_BLADE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 2, '白色弓', 'WHITE_WEAPON_BOW', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 3, '白色法盾', 'WHITE_WEAPON_SHIELD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 4, '白色法杖', 'WHITE_WEAPON_STAFF', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 5, '白色钺', 'WHITE_WEAPON_TOMAHAWK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 6, '白色剑', 'WHITE_WEAPON_SWORD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 7, '白色护腕', 'WHITE_WEAPON_WRIST', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 16, '蓝色枪', 'BLUE_WEAPON_SPEAR', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 17, '蓝色双刀', 'BLUE_WEAPON_BLADE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 18, '蓝色弓', 'BLUE_WEAPON_BOW', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 19, '蓝色法盾', 'BLUE_WEAPON_SHIELD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 20, '蓝色法杖', 'BLUE_WEAPON_STAFF', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 21, '蓝色钺', 'BLUE_WEAPON_TOMAHAWK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 22, '蓝色剑', 'BLUE_WEAPON_SWORD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 23, '蓝色护腕', 'BLUE_WEAPON_WRIST', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 32, '紫色枪', 'PURPLE_WEAPON_SPEAR', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 33, '紫色双刀', 'PURPLE_WEAPON_BLADE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 34, '紫色弓', 'PURPLE_WEAPON_BOW', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 35, '紫色法盾', 'PURPLE_WEAPON_SHIELD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 36, '紫色法杖', 'PURPLE_WEAPON_STAFF', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 37, '紫色钺', 'PURPLE_WEAPON_TOMAHAWK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 38, '紫色剑', 'PURPLE_WEAPON_SWORD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 39, '紫色护腕', 'PURPLE_WEAPON_WRIST', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 48, '金色枪', 'GOLD_WEAPON_SPEAR', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 49, '金色双刀', 'GOLD_WEAPON_BLADE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 50, '金色弓', 'GOLD_WEAPON_BOW', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 51, '金色法盾', 'GOLD_WEAPON_SHIELD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 52, '金色法杖', 'GOLD_WEAPON_STAFF', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 53, '金色钺', 'GOLD_WEAPON_TOMAHAWK', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 54, '金色剑', 'GOLD_WEAPON_SWORD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (28, 55, '金色护腕', 'GOLD_WEAPON_WRIST', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (29, 0, '白色装备', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (29, 1, '蓝色装备', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (29, 2, '紫色装备', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (29, 3, '金色装备', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 16, '白色重甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 17, '白色轻甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 18, '白色布甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 32, '蓝色重甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 33, '蓝色轻甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 34, '蓝色布甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 48, '紫色重甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 49, '紫色轻甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 50, '紫色布甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 64, '金色重甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 65, '金色轻甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (30, 66, '金色布甲', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (31, 0, '逍遥', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (31, 1, '阿彌陀佛', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (31, 2, '空即為色', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (31, 3, '混兮其濁乎', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (31, 4, '般若波羅蜜', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (31, 5, '心淨而神通萬應', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (31, 6, '塵穢消除九孔受靈', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (31, 7, '玄牝之門可謂天地根', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (31, 8, '臨兵鬥者皆陣列在前', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (31, 9, '菩提本無樹明鏡亦非臺', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (32, 0, '白色宝石', 'JEWEL_WHITE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (32, 1, '蓝色宝石', 'JEWEL_BLUE ', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (32, 2, '紫色宝石', 'JEWEL_PURPLE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (32, 3, '金色宝石', 'JEWEL_GOLD ', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (32, 4, '彩色宝石', 'JEWEL_COLOR', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (33, 0, '妖魔志', 'CARD_YMZ', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (33, 1, '神兵榜', 'CARD_SBB', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (33, 2, '名臣谱', 'CARD_MCP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (33, 3, '封神记', 'CARD_FSJ', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (33, 4, '风物录', 'CARD_FWL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (34, 8, '8格扩展包', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (34, 16, '16格扩展包', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (34, 24, '24格扩展包', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (34, 32, '32格扩展包', 'UNKNOWN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (35, 1, '恢复法宝HP', 'FUNCCODE_RECVMWHP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (35, 2, '恢复法宝MP', 'FUNCCODE_RECVMWMP', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (35, 3, '鉴定法宝属性', 'FUNCCODE_JUDGEMWPRO', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (35, 4, '家族经验令牌', 'FUNCCODE_FAMILYEXPCARD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (35, 5, '军团扩军令', 'FUNCCODE_CORPSTOKEN', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (35, 6, '法宝小洗石', 'FUNCCODE_MW_WASH_SMALL', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (35, 7, '法宝大洗石', 'FUNCCODE_MW_WASH_BIG', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (35, 8, '获得才华值', 'FUNCCODE_WIZARDVALUE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (35, 9, '获得称号卷轴', 'FUNCCODE_GETTITLE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (36, 0, '无限制', 'COLOR_NONE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (36, 1, '白色', 'COLOR_WHITE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (36, 2, '蓝色', 'COLOR_BLUE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (36, 3, '紫色', 'COLOR_PURPLE', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (36, 4, '金色', 'COLOR_GOLD', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (37, 1, '家族红包', 'REDPAPER_FAMILY', NULL, NULL);
INSERT INTO `SYS_CODE_VALUE` (`type_id`, `value_id`, `value_name`, `value_code`, `code_group`, `value_desc`) VALUES (37, 2, '军团红包', 'REDPAPER_CORPS', NULL, NULL);

/*!40000 ALTER TABLE `SYS_CODE_VALUE` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2009-12-07 13:26:18

