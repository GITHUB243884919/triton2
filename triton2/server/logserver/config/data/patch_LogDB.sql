-- 2010-03-24
-- 新增物品操作类型，金钱获得类型和经验获得类型

INSERT INTO SYS_CODE_VALUE (type_id, value_id, value_name, value_code, code_group, value_desc)
VALUES (14,  49,  '出售金镶玉获得',  'EM_GET_BY_SELL_JINXIANGYU',  1 , 0);

INSERT INTO SYS_CODE_VALUE (type_id, value_id, value_name, value_code, code_group, value_desc)
VALUES (14,  14,  '开礼包获得金钱',  'EM_GET_BY_GIFT',  1 , 0);

INSERT INTO SYS_CODE_VALUE (type_id, value_id, value_name, value_code, code_group, value_desc)
VALUES (9,  26,  '领取占领地图奖励', 'EM_GET_BY_AWARD',  4 , 0);

INSERT INTO SYS_CODE_VALUE (type_id, value_id, value_name, value_code, code_group, value_desc)
VALUES (9,  71,  '系统收回限时物品', 'EM_LOST_BY_TIMEOUT',  5 , 0);

INSERT INTO SYS_CODE_VALUE (type_id, value_id, value_name, value_code, code_group, value_desc)
VALUES (11,  13, '送人鲜花获得经验', 'EM_EXP_BY_FLOWER',  3 , 0);

-- 2010-04-14
UPDATE SYS_CODE_VALUE SET code_group ='0' WHERE (type_id='14') AND (value_id='67');
