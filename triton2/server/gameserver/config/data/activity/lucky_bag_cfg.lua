
-- 程序内部使用的奖励类型
enum_award_type=
{
	AWARD_ITEM = 1, -- 奖励道具
	AWARD_MONEY = 2, -- 奖励金钱
	AWARD_BAG = 3, -- 奖励包（已经在LUA中做了奖励）
	AWARD_OTHER = 4,  -- 其他奖励（已经在LUA中做了奖励）
	AWARD_TELEPORT = 5, -- 传送
};

-- 礼品包封装
bags = {
[638900] = {
	-- m-表示门派自适应 ID一般是天王殿的
	{ item_code="i-64133", item_id=64133, num=1, star=0 },
	{ item_code="i-60626", item_id=60626, num=1, star=0 },
	{ item_code="i-62022", item_id=62022, num=1, star=0 },
	{ item_code="i-58171", item_id=58171, num=1, star=0 },
	{ item_code="i-58172", item_id=58172, num=1, star=0 },
},

[638890] = {
	{ item_code="i-60296", item_id=60296, num=1, star=0 },
	{ item_code="i-62022", item_id=62022, num=1, star=0 },
},

[638910] = {
	{ item_code="i-64132", item_id=64132, num=1, star=0 },
	{ item_code="i-62022", item_id=62022, num=1, star=0 },
},

[701680] = {
	{ item_code="j-64573", item_id=64573, num=1, star=0 },
	{ item_code="i-60349", item_id=60349, num=9, star=0 },
	{ item_code="i-60192", item_id=60192, num=20, star=0 },
	{ item_code="i-60127", item_id=60127, num=20, star=0 },
	{ item_code="i-70167", item_id=70167, num=3, star=0 },
},

};

-- 乾坤袋的配置文件
main_cfg={};

-- 乾坤袋ID 63850 的配置
-- 注意：概率加起来一定要等于1W，否则剩下的概率如果被随到，玩家将什么都得不到。。。
main_cfg[63850]={
	-- 格式 奖项(和活动配置一致)，数量，奖项概率（万分比），开中后广播的消息ID(对应errcode里的错误码）（0是不提示）
	{ item_code="i-54723", item_id=54723, num=1, prob=50, notify_type=-69987, is_bag=0 },
	{ item_code="i-54726", item_id=54726, num=1, prob=120, notify_type=-69986, is_bag=0 },
	{ item_code="i-40131", item_id=40131, num=1, prob=100, notify_type=-69985, is_bag=0 },
	{ item_code="i-36009", item_id=36009, num=1, prob=600, notify_type=0, is_bag=0 },
	{ item_code="i-36781", item_id=36781, num=1, prob=600, notify_type=0, is_bag=0 },
	{ item_code="i-59232", item_id=59232, num=1, prob=700, notify_type=0, is_bag=0 },
	{ item_code="i-32663", item_id=32663, num=1, prob=700, notify_type=0,  is_bag=0},
	{ item_code="i-45534", item_id=45534, num=3, prob=1700, notify_type=0, is_bag=0 },
	{ item_code="i-45512", item_id=45512, num=3, prob=1000, notify_type=0, is_bag=0 },
	{ item_code="i-33970", item_id=33970, num=1, prob=700, notify_type=0,  is_bag=0},
	{ item_code="i-33978", item_id=33978, num=1, prob=700, notify_type=0, is_bag=0 },
	{ item_code="i-48374", item_id=48374, num=1, prob=1000, notify_type=0,  is_bag=0},
	{ item_code="i-45534", item_id=45534, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-45512", item_id=45512, num=1, prob=600, notify_type=0, is_bag=0 },
	{ item_code="i-40132", item_id=40132, num=1, prob=630, notify_type=-69984,  is_bag=0 },
};

-- 千里追风包
main_cfg[63890]={
	-- 格式 奖项(和活动配置一致)，数量，奖项概率（万分比），开中后广播的消息ID(对应errcode里的错误码）（0是不提示）
	-- b表示二次封装礼包 ID从物品ID*10开始排 b目前只能奖励1个
	{ item_code="b-638900", item_id=638900, num=1, prob=10000, notify_type=0, is_bag=1 },
};

-- 齐天包
main_cfg[63891]={
	{ item_code="b-638910", item_id=638910, num=1, prob=10000, notify_type=0, is_bag=1 },
};

-- 大圣包
main_cfg[63889]={
	{ item_code="b-638890", item_id=638890, num=1, prob=10000, notify_type=0, is_bag=1 },
};

-- 精英坐骑包
main_cfg[64133]={
	{ item_code="m-62191", item_id=62191, num=1, prob=10000, notify_type=0, is_bag=0 },
};

-- 10级武器包
main_cfg[64132]={
	{ item_code="m-63877", item_id=63877, num=1, prob=10000, notify_type=0, is_bag=0, star=5 },
};

-- 珠宝盒
main_cfg[68427]={
	-- 格式 奖项(和活动配置一致)，数量，奖项概率（万分比），开中后广播的消息ID(对应errcode里的错误码）（0是不提示）
	{ item_code="i-58086", item_id=58086, num=1, prob=50, notify_type=-69800, is_bag=0 },
	{ item_code="i-58078", item_id=58078, num=1, prob=50, notify_type=-69800, is_bag=0 },
	{ item_code="i-58084", item_id=58084, num=1, prob=50, notify_type=-69800, is_bag=0 },
	{ item_code="i-58076", item_id=58076, num=1, prob=50, notify_type=-69800, is_bag=0 },
	{ item_code="i-58090", item_id=58090, num=1, prob=100, notify_type=-69800, is_bag=0 },
	{ item_code="i-58088", item_id=58088, num=1, prob=100, notify_type=-69800, is_bag=0 },
	{ item_code="i-58072", item_id=58072, num=1, prob=20, notify_type=-69800, is_bag=0 },
	{ item_code="i-58080", item_id=58080, num=1, prob=20, notify_type=-69800, is_bag=0 },
	{ item_code="i-58074", item_id=58074, num=1, prob=20, notify_type=-69800, is_bag=0 },
	{ item_code="i-58082", item_id=58082, num=1, prob=20, notify_type=-69800, is_bag=0 },
	{ item_code="i-58070", item_id=58070, num=1, prob=20, notify_type=-69800, is_bag=0 },
	{ item_code="i-32666", item_id=32666, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32674", item_id=32674, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32682", item_id=32682, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32690", item_id=32690, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32866", item_id=32866, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32722", item_id=32722, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32714", item_id=32714, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32778", item_id=32778, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32786", item_id=32786, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32802", item_id=32802, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32858", item_id=32858, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32665", item_id=32665, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-32673", item_id=32673, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-32681", item_id=32681, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-32689", item_id=32689, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-32865", item_id=32865, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-32721", item_id=32721, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-32713", item_id=32713, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-32777", item_id=32777, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-32785", item_id=32785, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-32801", item_id=32801, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-32857", item_id=32857, num=1, prob=950, notify_type=0, is_bag=0 },

};

-- 如意袋
main_cfg[68426]={
	{ item_code="i-54724", item_id=54724, num=1, prob=50, notify_type=-69799, is_bag=0 },
	{ item_code="i-54727", item_id=54727, num=1, prob=120, notify_type=-69798, is_bag=0 },
	{ item_code="i-45586", item_id=45586, num=1, prob=10, notify_type=-69797, is_bag=0 },
	{ item_code="i-57325", item_id=57325, num=1, prob=20, notify_type=-69796, is_bag=0 },
	{ item_code="i-45584", item_id=45584, num=1, prob=20, notify_type=-69795, is_bag=0 },
	{ item_code="i-57326", item_id=57326, num=1, prob=20, notify_type=-69794, is_bag=0 },
	{ item_code="i-48370", item_id=48370, num=1, prob=900, notify_type=0, is_bag=0 },
	{ item_code="i-46174", item_id=46174, num=1, prob=900, notify_type=0, is_bag=0 },
	{ item_code="i-46170", item_id=46170, num=1, prob=900, notify_type=0, is_bag=0 },
	{ item_code="i-48366", item_id=48366, num=1, prob=900, notify_type=0, is_bag=0 },
	{ item_code="i-46173", item_id=46173, num=1, prob=900, notify_type=0, is_bag=0 },
	{ item_code="i-46169", item_id=46169, num=1, prob=900, notify_type=0, is_bag=0 },
	{ item_code="i-58228", item_id=58228, num=1, prob=900, notify_type=0, is_bag=0 },
	{ item_code="i-33963", item_id=33963, num=1, prob=500, notify_type=0, is_bag=0 },
	{ item_code="i-33967", item_id=33967, num=1, prob=500, notify_type=0, is_bag=0 },
	{ item_code="i-45538", item_id=45538, num=3, prob=900, notify_type=0, is_bag=0 },
	{ item_code="i-45525", item_id=45525, num=3, prob=700, notify_type=0, is_bag=0 },
	{ item_code="i-62175", item_id=62175, num=1, prob=860, notify_type=-69793, is_bag=0 },
};

-- 神兵鉴
main_cfg[69473]={
	{ item_code="m-63877", item_id=63877, num=1, prob=10000, notify_type=0, is_bag=0, star=5 },
};

-- 门派特权封印
main_cfg[69451]={
	{ item_code="m-69485", item_id=69485, num=1, prob=10000, notify_type=0, is_bag=0, star=0 },
};
--百宝袋
main_cfg[70167]={
	{ item_code="i-54725", item_id=54725, num=1, prob=50, notify_type=-69792, is_bag=0 },
	{ item_code="i-54728", item_id=54728, num=1, prob=120, notify_type=-69791, is_bag=0 },
	{ item_code="i-60502", item_id=60502, num=1, prob=20, notify_type=-69790, is_bag=0 },
	{ item_code="i-57327", item_id=57327, num=1, prob=30, notify_type=-69789, is_bag=0 },
	{ item_code="i-57328", item_id=57328, num=1, prob=30, notify_type=-69788, is_bag=0 },
	{ item_code="i-61322", item_id=61322, num=1, prob=10, notify_type=-69787, is_bag=0 },
	{ item_code="i-61325", item_id=61325, num=1, prob=30, notify_type=-69786, is_bag=0 },
	{ item_code="i-61321", item_id=61321, num=1, prob=10, notify_type=-69785, is_bag=0 },
	{ item_code="i-61324", item_id=61324, num=1, prob=50, notify_type=-69784, is_bag=0 },
	{ item_code="i-61327", item_id=61327, num=1, prob=600, notify_type=0, is_bag=0 },
	{ item_code="i-48371", item_id=48371, num=1, prob=1000, notify_type=0, is_bag=0 },
	{ item_code="i-46189", item_id=46189, num=1, prob=1000, notify_type=0, is_bag=0 },
	{ item_code="i-46185", item_id=46185, num=1, prob=1000, notify_type=0, is_bag=0 },
	{ item_code="i-48367", item_id=48367, num=1, prob=1000, notify_type=0, is_bag=0 },
	{ item_code="i-46188", item_id=46188, num=1, prob=1000, notify_type=0, is_bag=0 },
	{ item_code="i-46184", item_id=46184, num=1, prob=1000, notify_type=0, is_bag=0 },
	{ item_code="i-58230", item_id=58230, num=1, prob=700, notify_type=0, is_bag=0 },
	{ item_code="i-33964", item_id=33964, num=1, prob=500, notify_type=0, is_bag=0 },
	{ item_code="i-33968", item_id=33968, num=1, prob=500, notify_type=0, is_bag=0 },
	{ item_code="i-45542", item_id=45542, num=5, prob=1000, notify_type=0, is_bag=0 },
	{ item_code="i-62176", item_id=62176, num=1, prob=350, notify_type=-69783, is_bag=0 },
};
-- 多宝令牌盒
main_cfg[70276]={
	{ item_code="i-66654", item_id=66654, num=1, prob=360,   notify_type=0, is_bag=0 },
	{ item_code="i-66656", item_id=66656, num=1, prob=500,   notify_type=0, is_bag=0 },
	{ item_code="i-66658", item_id=66658, num=1, prob=289  , notify_type=0, is_bag=0 },
	{ item_code="i-66660", item_id=66660, num=1, prob=360,   notify_type=0, is_bag=0 },
	{ item_code="i-66664", item_id=66664, num=1, prob=500,   notify_type=0, is_bag=0 },
	{ item_code="i-66666", item_id=66666, num=1, prob=289,   notify_type=0, is_bag=0 },
	{ item_code="i-66554", item_id=66554, num=1, prob=1, notify_type=-69797, is_bag=0 },
	{ item_code="i-66553", item_id=66553, num=1, prob=1, notify_type=-69797, is_bag=0 },
	{ item_code="i-69439", item_id=69439, num=1, prob=7500, notify_type=0, is_bag=0 },
	{ item_code="i-69263", item_id=69263, num=1, prob=100, notify_type=-69797, is_bag=0 },
	{ item_code="i-69264", item_id=69264, num=1, prob=100, notify_type=-69797, is_bag=0 },
};

-- 悟空欲嫁包
main_cfg[70168]={
	{ item_code="b-701680", item_id=701680, num=1, prob=10000, notify_type=0, is_bag=1 },
};

--  珠宝盒
main_cfg[70844]={
	{ item_code="i-32669", item_id=32669, num=1, prob=30, notify_type=-69782, is_bag=0 },
	{ item_code="i-32677", item_id=32677, num=1, prob=30, notify_type=-69782, is_bag=0 },
	{ item_code="i-32685", item_id=32685, num=1, prob=30, notify_type=-69782, is_bag=0 },
	{ item_code="i-32693", item_id=32693, num=1, prob=30, notify_type=-69782, is_bag=0 },
	{ item_code="i-32869", item_id=32869, num=1, prob=30, notify_type=-69782, is_bag=0 },
	{ item_code="i-32725", item_id=32725, num=1, prob=30, notify_type=-69782, is_bag=0 },
	{ item_code="i-32717", item_id=32717, num=1, prob=20, notify_type=-69782, is_bag=0 },
	{ item_code="i-32781", item_id=32781, num=1, prob=20, notify_type=-69782, is_bag=0 },
	{ item_code="i-32789", item_id=32789, num=1, prob=20, notify_type=-69782, is_bag=0 },
	{ item_code="i-32805", item_id=32805, num=1, prob=20, notify_type=-69782, is_bag=0 },
	{ item_code="i-32861", item_id=32861, num=1, prob=20, notify_type=-69782, is_bag=0 },
	{ item_code="i-32668", item_id=32668, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32676", item_id=32676, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32684", item_id=32684, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32692", item_id=32692, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32868", item_id=32868, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32724", item_id=32724, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32716", item_id=32716, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32780", item_id=32780, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32788", item_id=32788, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32804", item_id=32804, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-32860", item_id=32860, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-58086", item_id=58086, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-58078", item_id=58078, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-58084", item_id=58084, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-58076", item_id=58076, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-58090", item_id=58090, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-58088", item_id=58088, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-58072", item_id=58072, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-58080", item_id=58080, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-58074", item_id=58074, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-58082", item_id=58082, num=1, prob=800, notify_type=0, is_bag=0 },
	{ item_code="i-58070", item_id=58070, num=1, prob=1170, notify_type=0, is_bag=0 },
};
-- 纸鸳
main_cfg[71197]={
	{ item_code="i-69520", item_id=69520, num=1, prob=700, notify_type=-69601, is_bag=0 },
	{ item_code="i-71369", item_id=71369, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71363", item_id=71363, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71365", item_id=71365, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71351", item_id=71351, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71353", item_id=71353, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71355", item_id=71355, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71357", item_id=71357, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71361", item_id=71361, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71371", item_id=71371, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71367", item_id=71367, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71359", item_id=71359, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="i-71370", item_id=71370, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="i-71364", item_id=71364, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="i-71366", item_id=71366, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="i-71352", item_id=71352, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="i-71354", item_id=71354, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="i-71356", item_id=71356, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="i-71358", item_id=71358, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="i-71362", item_id=71362, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="i-71372", item_id=71372, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="i-71368", item_id=71368, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="i-71360", item_id=71360, num=1, prob=50, notify_type=-69601, is_bag=0 },
	{ item_code="f-71337", item_id=71337, num=1, prob=3900, notify_type=-69603, is_bag=0 },
	{ item_code="t-711970", item_id=711970, num=1, prob=1000, notify_type=-69602, is_bag=0 },
};
-- 纸鸳
main_cfg[75777]={
      { item_code="f-76044", item_id=76044, num=1, prob=10000, notify_type=0, is_bag=0 },		
};
-- 越南版乾坤袋
main_cfg[71393]={
	{ item_code="i-58177", item_id=58177, num=2, prob=3000, notify_type=0, is_bag=0 },
	{ item_code="i-58200", item_id=58200, num=20, prob=1000, notify_type=0, is_bag=0 },
	{ item_code="i-58231", item_id=58231, num=1, prob=135, notify_type=0, is_bag=0 },
	{ item_code="i-32349", item_id=32349, num=1, prob=50, notify_type=-69797, is_bag=0 },
	{ item_code="i-32350", item_id=32350, num=1, prob=50, notify_type=-69797, is_bag=0 },
	{ item_code="i-32351", item_id=32351, num=1, prob=20, notify_type=-69797, is_bag=0 },
	{ item_code="i-32352", item_id=32352, num=1, prob=20, notify_type=-69797, is_bag=0 },
	{ item_code="i-52271", item_id=52271, num=1, prob=5, notify_type=-69797, is_bag=0 },
	{ item_code="i-65199", item_id=65199, num=1, prob=10, notify_type=-69797, is_bag=0 },
	{ item_code="i-52303", item_id=52303, num=1, prob=10, notify_type=-69797, is_bag=0 },
	{ item_code="i-52302", item_id=52302, num=1, prob=40, notify_type=-69797, is_bag=0 },
	{ item_code="i-66553", item_id=66553, num=1, prob=1, notify_type=-69797, is_bag=0 },
	{ item_code="i-66554", item_id=66554, num=1, prob=1, notify_type=-69797, is_bag=0 },
	{ item_code="i-54728", item_id=54728, num=1, prob=3, notify_type=-69797, is_bag=0 },
	{ item_code="i-54725", item_id=54725, num=1, prob=3, notify_type=-69797, is_bag=0 },
	{ item_code="i-57329", item_id=57329, num=1, prob=1, notify_type=-69797, is_bag=0 },
	{ item_code="i-60508", item_id=60508, num=1, prob=1, notify_type=-69797, is_bag=0 },
	{ item_code="i-58173", item_id=58173, num=1, prob=1650, notify_type=0, is_bag=0 },
	{ item_code="i-41449", item_id=41449, num=8, prob=2000, notify_type=0, is_bag=0 },
	{ item_code="i-41452", item_id=41452, num=8, prob=2000, notify_type=0, is_bag=0 },
};
--彩蛋·乐
main_cfg[72419]={
	{ item_code="f-57304", item_id=57304, num=1, prob=400, notify_type=0, is_bag=0 },
	{ item_code="f-57295", item_id=57295, num=1, prob=700, notify_type=0, is_bag=0 },
	{ item_code="f-57280", item_id=57280, num=1, prob=700, notify_type=0, is_bag=0 },
	{ item_code="f-57241", item_id=57241, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="f-57285", item_id=57285, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="f-57286", item_id=57286, num=1, prob=350, notify_type=0, is_bag=0 },
	{ item_code="f-57264", item_id=57264, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-57239", item_id=57239, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="f-57307", item_id=57307, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="f-57261", item_id=57261, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="f-57269", item_id=57269, num=1, prob=400, notify_type=0, is_bag=0 },
	{ item_code="f-57268", item_id=57268, num=1, prob=400, notify_type=0, is_bag=0 },
	{ item_code="f-57301", item_id=57301, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-57282", item_id=57282, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="f-57252", item_id=57252, num=1, prob=700, notify_type=0, is_bag=0 },
	{ item_code="f-57265", item_id=57265, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-57238", item_id=57238, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-57270", item_id=57270, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-57294", item_id=57294, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="f-57302", item_id=57302, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="f-57240", item_id=57240, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-57277", item_id=57277, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-57298", item_id=57298, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="f-57288", item_id=57288, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-57296", item_id=57296, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-57266", item_id=57266, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="f-57254", item_id=57254, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-57287", item_id=57287, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="f-71194", item_id=71194, num=1, prob=1800, notify_type=0, is_bag=0 },
};
--初级口诀
main_cfg[72796]={
	{ item_code="i-68176", item_id=68176, num=1, prob=5, notify_type=-69781, is_bag=0 },
	{ item_code="i-68177", item_id=68177, num=1, prob=10, notify_type=-69781, is_bag=0 },
	{ item_code="i-68178", item_id=68178, num=1, prob=5, notify_type=-69781, is_bag=0 },
	{ item_code="i-68164", item_id=68164, num=1, prob=10, notify_type=-69781, is_bag=0 },
	{ item_code="i-68165", item_id=68165, num=1, prob=20, notify_type=-69781, is_bag=0 },
	{ item_code="i-68166", item_id=68166, num=1, prob=10, notify_type=-69781, is_bag=0 },
	{ item_code="i-68151", item_id=68151, num=1, prob=30, notify_type=-69781, is_bag=0 },
	{ item_code="i-68153", item_id=68153, num=1, prob=20, notify_type=-69781, is_bag=0 },
	{ item_code="i-68154", item_id=68154, num=1, prob=30, notify_type=-69781, is_bag=0 },
	{ item_code="i-68140", item_id=68140, num=1, prob=30, notify_type=-69781, is_bag=0 },
	{ item_code="i-68141", item_id=68141, num=1, prob=40, notify_type=-69781, is_bag=0 },
	{ item_code="i-68130", item_id=68130, num=1, prob=40, notify_type=-69781, is_bag=0 },
	{ item_code="i-68167", item_id=68167, num=1, prob=150, notify_type=0, is_bag=0 },
	{ item_code="i-68168", item_id=68168, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="i-68169", item_id=68169, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-68170", item_id=68170, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-68171", item_id=68171, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="i-68172", item_id=68172, num=1, prob=150, notify_type=0, is_bag=0 },
	{ item_code="i-68173", item_id=68173, num=1, prob=150, notify_type=0, is_bag=0 },
	{ item_code="i-68174", item_id=68174, num=1, prob=50, notify_type=0, is_bag=0 },
	{ item_code="i-68175", item_id=68175, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="i-68155", item_id=68155, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="i-68156", item_id=68156, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="i-68157", item_id=68157, num=1, prob=200, notify_type=0, is_bag=0 },
	{ item_code="i-68158", item_id=68158, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="i-68159", item_id=68159, num=1, prob=200, notify_type=0, is_bag=0 },
	{ item_code="i-68160", item_id=68160, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="i-68161", item_id=68161, num=1, prob=200, notify_type=0, is_bag=0 },
	{ item_code="i-68162", item_id=68162, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="i-68163", item_id=68163, num=1, prob=100, notify_type=0, is_bag=0 },
	{ item_code="i-68148", item_id=68148, num=1, prob=400, notify_type=0, is_bag=0 },
	{ item_code="i-68149", item_id=68149, num=1, prob=200, notify_type=0, is_bag=0 },
	{ item_code="i-68150", item_id=68150, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="i-68142", item_id=68142, num=1, prob=200, notify_type=0, is_bag=0 },
	{ item_code="i-68143", item_id=68143, num=1, prob=400, notify_type=0, is_bag=0 },
	{ item_code="i-68144", item_id=68144, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="i-68145", item_id=68145, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="i-68146", item_id=68146, num=1, prob=400, notify_type=0, is_bag=0 },
	{ item_code="i-68147", item_id=68147, num=1, prob=200, notify_type=0, is_bag=0 },
	{ item_code="i-68131", item_id=68131, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="i-68133", item_id=68133, num=1, prob=500, notify_type=0, is_bag=0 },
	{ item_code="i-68135", item_id=68135, num=1, prob=500, notify_type=0, is_bag=0 },
	{ item_code="i-68136", item_id=68136, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="i-68137", item_id=68137, num=1, prob=500, notify_type=0, is_bag=0 },
	{ item_code="i-68139", item_id=68139, num=1, prob=300, notify_type=0, is_bag=0 },
	{ item_code="i-68123", item_id=68123, num=1, prob=650, notify_type=0, is_bag=0 },
	{ item_code="i-68126", item_id=68126, num=1, prob=650, notify_type=0, is_bag=0 },
	{ item_code="i-68127", item_id=68127, num=1, prob=650, notify_type=0, is_bag=0 },
};
--高级口诀
main_cfg[72808]={
{ item_code="i-68235", item_id=68235, num=1, prob=10, notify_type=-69780, is_bag=0 },
{ item_code="i-68236", item_id=68236, num=1, prob=10, notify_type=-69780, is_bag=0 },
{ item_code="i-68237", item_id=68237, num=1, prob=10, notify_type=-69780, is_bag=0 },
{ item_code="i-68238", item_id=68238, num=1, prob=5, notify_type=-69780, is_bag=0 },
{ item_code="i-68239", item_id=68239, num=1, prob=5, notify_type=-69780, is_bag=0 },
{ item_code="i-68213", item_id=68213, num=1, prob=15, notify_type=-69780, is_bag=0 },
{ item_code="i-68214", item_id=68214, num=1, prob=15, notify_type=-69780, is_bag=0 },
{ item_code="i-68215", item_id=68215, num=1, prob=20, notify_type=-69780, is_bag=0 },
{ item_code="i-68216", item_id=68216, num=1, prob=25, notify_type=-69780, is_bag=0 },
{ item_code="i-68191", item_id=68191, num=1, prob=40, notify_type=-69780, is_bag=0 },
{ item_code="i-68192", item_id=68192, num=1, prob=40, notify_type=-69780, is_bag=0 },
{ item_code="i-68193", item_id=68193, num=1, prob=25, notify_type=-69780, is_bag=0 },
{ item_code="i-68194", item_id=68194, num=1, prob=30, notify_type=-69780, is_bag=0 },
{ item_code="i-68217", item_id=68217, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68219", item_id=68219, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68220", item_id=68220, num=1, prob=50, notify_type=0, is_bag=0 },
{ item_code="i-68222", item_id=68222, num=1, prob=50, notify_type=0, is_bag=0 },
{ item_code="i-68223", item_id=68223, num=1, prob=50, notify_type=0, is_bag=0 },
{ item_code="i-68224", item_id=68224, num=1, prob=50, notify_type=0, is_bag=0 },
{ item_code="i-68225", item_id=68225, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68226", item_id=68226, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68228", item_id=68228, num=1, prob=50, notify_type=0, is_bag=0 },
{ item_code="i-68229", item_id=68229, num=1, prob=50, notify_type=0, is_bag=0 },
{ item_code="i-68230", item_id=68230, num=1, prob=50, notify_type=0, is_bag=0 },
{ item_code="i-68231", item_id=68231, num=1, prob=50, notify_type=0, is_bag=0 },
{ item_code="i-68232", item_id=68232, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68233", item_id=68233, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68234", item_id=68234, num=1, prob=50, notify_type=0, is_bag=0 },
{ item_code="i-68195", item_id=68195, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68196", item_id=68196, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68198", item_id=68198, num=1, prob=250, notify_type=0, is_bag=0 },
{ item_code="i-68199", item_id=68199, num=1, prob=250, notify_type=0, is_bag=0 },
{ item_code="i-68201", item_id=68201, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68203", item_id=68203, num=1, prob=250, notify_type=0, is_bag=0 },
{ item_code="i-68204", item_id=68204, num=1, prob=250, notify_type=0, is_bag=0 },
{ item_code="i-68206", item_id=68206, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68208", item_id=68208, num=1, prob=250, notify_type=0, is_bag=0 },
{ item_code="i-68210", item_id=68210, num=1, prob=250, notify_type=0, is_bag=0 },
{ item_code="i-68211", item_id=68211, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68212", item_id=68212, num=1, prob=150, notify_type=0, is_bag=0 },
{ item_code="i-68183", item_id=68183, num=1, prob=400, notify_type=0, is_bag=0 },
{ item_code="i-68184", item_id=68184, num=1, prob=400, notify_type=0, is_bag=0 },
{ item_code="i-68185", item_id=68185, num=1, prob=600, notify_type=0, is_bag=0 },
{ item_code="i-68186", item_id=68186, num=1, prob=600, notify_type=0, is_bag=0 },
{ item_code="i-68179", item_id=68179, num=1, prob=400, notify_type=0, is_bag=0 },
{ item_code="i-68180", item_id=68180, num=1, prob=400, notify_type=0, is_bag=0 },
{ item_code="i-68181", item_id=68181, num=1, prob=600, notify_type=0, is_bag=0 },
{ item_code="i-68182", item_id=68182, num=1, prob=600, notify_type=0, is_bag=0 },
{ item_code="i-68187", item_id=68187, num=1, prob=400, notify_type=0, is_bag=0 },
{ item_code="i-68188", item_id=68188, num=1, prob=600, notify_type=0, is_bag=0 },
{ item_code="i-68189", item_id=68189, num=1, prob=600, notify_type=0, is_bag=0 },
{ item_code="i-68190", item_id=68190, num=1, prob=400, notify_type=0, is_bag=0 },

};
-- 千机袋
main_cfg[72809]={
{ item_code="i-66553", item_id=66553, num=1, prob=50, notify_type=-69779, is_bag=0 },
{ item_code="i-66554", item_id=66554, num=1, prob=120, notify_type=-69778, is_bag=0 },
{ item_code="i-60506", item_id=60506, num=1, prob=5, notify_type=-69777, is_bag=0 },
{ item_code="i-45591", item_id=45591, num=1, prob=5, notify_type=-69776, is_bag=0 },
{ item_code="i-65258", item_id=65258, num=1, prob=50, notify_type=-69775, is_bag=0 },
{ item_code="i-69263", item_id=69263, num=1, prob=200, notify_type=-69774, is_bag=0 },
{ item_code="i-69264", item_id=69264, num=1, prob=200, notify_type=-69773, is_bag=0 },
{ item_code="i-66666", item_id=66666, num=2, prob=800, notify_type=0, is_bag=0 },
{ item_code="i-66658", item_id=66658, num=2, prob=800, notify_type=0, is_bag=0 },
{ item_code="i-66660", item_id=66660, num=3, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-66654", item_id=66654, num=3, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-66664", item_id=66664, num=3, prob=1300, notify_type=0, is_bag=0 },
{ item_code="i-66656", item_id=66656, num=3, prob=1300, notify_type=0, is_bag=0 },
{ item_code="i-60349", item_id=60349, num=9, prob=700, notify_type=0, is_bag=0 },
{ item_code="i-60349", item_id=60349, num=5, prob=1500, notify_type=0, is_bag=0 },
{ item_code="i-66887", item_id=66887, num=3, prob=970, notify_type=0, is_bag=0 },
};
--越南版二代乾坤袋
main_cfg[72965]={
{ item_code="i-67033", item_id=67033, num=1, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-69519", item_id=69519, num=5, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-57064", item_id=57064, num=2, prob=620, notify_type=0, is_bag=0 },
{ item_code="i-57081", item_id=57081, num=2, prob=620, notify_type=0, is_bag=0 },
{ item_code="i-57099", item_id=57099, num=2, prob=620, notify_type=0, is_bag=0 },
{ item_code="i-57109", item_id=57109, num=2, prob=620, notify_type=0, is_bag=0 },
{ item_code="i-57124", item_id=57124, num=2, prob=620, notify_type=0, is_bag=0 },
{ item_code="i-57112", item_id=57112, num=2, prob=620, notify_type=0, is_bag=0 },
{ item_code="i-66553", item_id=66553, num=1, prob=3, notify_type=-69797, is_bag=0 },
{ item_code="i-66554", item_id=66554, num=1, prob=3, notify_type=-69797, is_bag=0 },
{ item_code="i-54728", item_id=54728, num=1, prob=20, notify_type=-69797, is_bag=0 },
{ item_code="i-54725", item_id=54725, num=1, prob=20, notify_type=-69797, is_bag=0 },
{ item_code="i-57329", item_id=57329, num=1, prob=4, notify_type=-69797, is_bag=0 },
{ item_code="i-60500", item_id=60500, num=1, prob=100, notify_type=-69797, is_bag=0 },
{ item_code="i-40132", item_id=40132, num=3, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-69263", item_id=69263, num=1, prob=65, notify_type=-69797, is_bag=0 },
{ item_code="i-69264", item_id=69264, num=1, prob=65, notify_type=-69797, is_bag=0 },
};
--老服强化一
main_cfg[72966]={
{ item_code="i-67033", item_id=67033, num=1, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-25400", item_id=25400, num=1, prob=300, notify_type=-69797, is_bag=0 },
{ item_code="i-65258", item_id=65258, num=1, prob=100, notify_type=-69797, is_bag=0 },
{ item_code="i-57064", item_id=57064, num=2, prob=635, notify_type=0, is_bag=0 },
{ item_code="i-57081", item_id=57081, num=2, prob=635, notify_type=0, is_bag=0 },
{ item_code="i-57099", item_id=57099, num=2, prob=635, notify_type=0, is_bag=0 },
{ item_code="i-57109", item_id=57109, num=2, prob=635, notify_type=0, is_bag=0 },
{ item_code="i-57124", item_id=57124, num=2, prob=635, notify_type=0, is_bag=0 },
{ item_code="i-57112", item_id=57112, num=2, prob=635, notify_type=0, is_bag=0 },
{ item_code="i-60500", item_id=60500, num=1, prob=80, notify_type=0, is_bag=0 },
{ item_code="i-45586", item_id=45586, num=1, prob=80, notify_type=0, is_bag=0 },
{ item_code="i-66553", item_id=66553, num=1, prob=3, notify_type=-69797, is_bag=0 },
{ item_code="i-45588", item_id=45588, num=1, prob=25, notify_type=-69797, is_bag=0 },
{ item_code="i-60502", item_id=60502, num=1, prob=25, notify_type=-69797, is_bag=0 },
{ item_code="i-60338", item_id=60338, num=1, prob=70, notify_type=-69797, is_bag=0 },
{ item_code="i-45590", item_id=45590, num=1, prob=3, notify_type=-69797, is_bag=0 },
{ item_code="i-60506", item_id=60506, num=1, prob=3, notify_type=-69797, is_bag=0 },
{ item_code="i-58173", item_id=58173, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-40132", item_id=40132, num=3, prob=2501, notify_type=0, is_bag=0 },
};
--老服强化二
main_cfg[72967]={
{ item_code="i-67033", item_id=67033, num=1, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-25400", item_id=25400, num=1, prob=200, notify_type=0, is_bag=0 },
{ item_code="i-64763", item_id=64763, num=1, prob=160, notify_type=0, is_bag=0 },
{ item_code="i-57064", item_id=57064, num=2, prob=640, notify_type=0, is_bag=0 },
{ item_code="i-57081", item_id=57081, num=2, prob=640, notify_type=0, is_bag=0 },
{ item_code="i-57099", item_id=57099, num=2, prob=640, notify_type=0, is_bag=0 },
{ item_code="i-57109", item_id=57109, num=2, prob=640, notify_type=0, is_bag=0 },
{ item_code="i-57124", item_id=57124, num=2, prob=640, notify_type=0, is_bag=0 },
{ item_code="i-57112", item_id=57112, num=2, prob=640, notify_type=0, is_bag=0 },
{ item_code="i-69497", item_id=69497, num=1, prob=75, notify_type=-69797, is_bag=0 },
{ item_code="i-69493", item_id=69493, num=1, prob=75, notify_type=-69797, is_bag=0 },
{ item_code="i-66554", item_id=66554, num=1, prob=3, notify_type=-69797, is_bag=0 },
{ item_code="i-45589", item_id=45589, num=1, prob=30, notify_type=-69797, is_bag=0 },
{ item_code="i-60335", item_id=60335, num=1, prob=80, notify_type=-69797, is_bag=0 },
{ item_code="i-45591", item_id=45591, num=1, prob=6, notify_type=-69797, is_bag=0 },
{ item_code="i-58173", item_id=58173, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-40132", item_id=40132, num=3, prob=2531, notify_type=0, is_bag=0 },
};
--万禄礼包
main_cfg[75132]={
{ item_code="i-59202", item_id=59202, num=1, prob=698, notify_type=-69797, is_bag=0 },
{ item_code="i-59201", item_id=59201, num=1, prob=2000, notify_type=-69797, is_bag=0 },
{ item_code="i-52303", item_id=52303, num=1, prob=200, notify_type=-69797, is_bag=0 },
{ item_code="i-52271", item_id=52271, num=1, prob=200, notify_type=-69797, is_bag=0 },
{ item_code="i-70539", item_id=70539, num=1, prob=500, notify_type=-69797, is_bag=0 },
{ item_code="i-70542", item_id=70542, num=1, prob=400, notify_type=-69797, is_bag=0 },
{ item_code="i-57331", item_id=57331, num=1, prob=2, notify_type=-69797, is_bag=0 },
{ item_code="i-46067", item_id=46067, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-46078", item_id=46078, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-46077", item_id=46077, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-69519", item_id=69519, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-40132", item_id=40132, num=1, prob=2000, notify_type=0, is_bag=0 },
};
--碎片礼包
main_cfg[75133]={
{ item_code="i-70227", item_id=70227, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-70520", item_id=70520, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-66804", item_id=66804, num=1, prob=500, notify_type=-69797, is_bag=0 },
{ item_code="i-66805", item_id=66805, num=1, prob=500, notify_type=-69797, is_bag=0 },
{ item_code="i-70539", item_id=70539, num=1, prob=300, notify_type=-69797, is_bag=0 },
{ item_code="i-70542", item_id=70542, num=1, prob=200, notify_type=-69797, is_bag=0 },
{ item_code="i-68653", item_id=68653, num=10, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-58177", item_id=58177, num=1, prob=500, notify_type=0, is_bag=0 },
{ item_code="i-70802", item_id=70802, num=5, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-46828", item_id=46828, num=1, prob=500, notify_type=0, is_bag=0 },
{ item_code="i-46826", item_id=46826, num=1, prob=500, notify_type=0, is_bag=0 },
{ item_code="i-46831", item_id=46831, num=1, prob=500, notify_type=0, is_bag=0 },
{ item_code="i-46834", item_id=46834, num=1, prob=500, notify_type=0, is_bag=0 },
};
--斩妖令牌乾坤袋
main_cfg[75671]={
{ item_code="i-71351", item_id=71351, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-71353", item_id=71353, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-71355", item_id=71355, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-71357", item_id=71357, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-71359", item_id=71359, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-71361", item_id=71361, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-71363", item_id=71363, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-71365", item_id=71365, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-71367", item_id=71367, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-71369", item_id=71369, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-71371", item_id=71371, num=1, prob=300, notify_type=0, is_bag=0 },
{ item_code="i-69273", item_id=69273, num=1, prob=5, notify_type=-69797, is_bag=0 },
{ item_code="i-69275", item_id=69275, num=1, prob=5, notify_type=-69797, is_bag=0 },
{ item_code="i-75665", item_id=75665, num=1, prob=600, notify_type=0, is_bag=0 },
{ item_code="i-75666", item_id=75666, num=1, prob=600, notify_type=0, is_bag=0 },
{ item_code="i-75667", item_id=75667, num=1, prob=400, notify_type=0, is_bag=0 },
{ item_code="i-75668", item_id=75668, num=1, prob=600, notify_type=0, is_bag=0 },
{ item_code="i-75669", item_id=75669, num=1, prob=600, notify_type=0, is_bag=0 },
{ item_code="i-75670", item_id=75670, num=1, prob=400, notify_type=0, is_bag=0 },
{ item_code="i-34194", item_id=34194, num=1, prob=3490, notify_type=0, is_bag=0 },
};
--越南版装备礼包
main_cfg[75774]={
{ item_code="i-73065", item_id=73065, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-73064", item_id=73064, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-73052", item_id=73052, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-73054", item_id=73054, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-40132", item_id=40132, num=1, prob=2470, notify_type=0, is_bag=0 },
{ item_code="i-68653", item_id=68653, num=5, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-66805", item_id=66805, num=1, prob=360, notify_type=-69797, is_bag=0 },
{ item_code="i-66804", item_id=66804, num=1, prob=390, notify_type=-69797, is_bag=0 },
{ item_code="i-70525", item_id=70525, num=1, prob=440, notify_type=-69797, is_bag=0 },
{ item_code="i-70522", item_id=70522, num=1, prob=340, notify_type=-69797, is_bag=0 },
};
--佛缘武器包
main_cfg[76120]={
	{ item_code="m-76108", item_id=76108, num=1, prob=10000, notify_type=0, is_bag=0, star=6 },
};
--佛缘衣服包
main_cfg[76121]={
	{ item_code="m-76114", item_id=76114, num=1, prob=10000, notify_type=0, is_bag=0, star=6 },
};
--佛缘护腕包
main_cfg[76122]={
	{ item_code="m-76117", item_id=76117, num=1, prob=10000, notify_type=0, is_bag=0, star=6 },
};
--佛缘护符包
main_cfg[76123]={
	{ item_code="m-76106", item_id=76106, num=1, prob=10000, notify_type=0, is_bag=0, star=6 },
};
--越南版法宝飞升礼包
main_cfg[76362]={
{ item_code="i-73598", item_id=73598, num=1, prob=750, notify_type=-69797, is_bag=0 },
{ item_code="i-73580", item_id=73580, num=1, prob=500, notify_type=-69797, is_bag=0 },
{ item_code="i-73562", item_id=73562, num=1, prob=1500, notify_type=-69797, is_bag=0 },
{ item_code="i-73604", item_id=73604, num=1, prob=100, notify_type=-69797, is_bag=0 },
{ item_code="i-73586", item_id=73586, num=1, prob=75, notify_type=-69797, is_bag=0 },
{ item_code="i-73568", item_id=73568, num=1, prob=150, notify_type=-69797, is_bag=0 },
{ item_code="i-59203", item_id=59203, num=1, prob=50, notify_type=-69797, is_bag=0 },
{ item_code="i-59204", item_id=59204, num=1, prob=20, notify_type=-69797, is_bag=0 },
{ item_code="i-40132", item_id=40132, num=1, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-68653", item_id=68653, num=1, prob=1500, notify_type=0, is_bag=0 },
{ item_code="i-46065", item_id=46065, num=1, prob=855, notify_type=0, is_bag=0 },
{ item_code="i-46055", item_id=46055, num=1, prob=2500, notify_type=0, is_bag=0 },
};

--越南极品礼包
main_cfg[77022]={
{ item_code="i-73881", item_id=73881, num=1, prob=1500, notify_type=0, is_bag=0 },
{ item_code="i-73882", item_id=73882, num=1, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-73880", item_id=73880, num=1, prob=385, notify_type=-69797, is_bag=0 },
{ item_code="i-75253", item_id=75253, num=1, prob=5, notify_type=-69797, is_bag=0 },
{ item_code="i-45590", item_id=45590, num=1, prob=5, notify_type=-69797, is_bag=0 },
{ item_code="i-45591", item_id=45591, num=1, prob=5, notify_type=-69797, is_bag=0 },
{ item_code="i-58177", item_id=58177, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-69519", item_id=69519, num=1, prob=1000, notify_type=0, is_bag=0 },
{ item_code="i-68653", item_id=68653, num=1, prob=1100, notify_type=0, is_bag=0 },
{ item_code="i-40132", item_id=40132, num=1, prob=2000, notify_type=0, is_bag=0 },
{ item_code="i-46834", item_id=46834, num=1, prob=1000, notify_type=0, is_bag=0 },
};
---------------------------------------------------------------------------------------------------------------------------------------
-- 门派自适应道具解析表
metier_item_cfg = {};
metier_item_cfg[62191]={
[1] = 62191,
[2] = 62192,
[3] = 62193,
[4] = 62194,
[5] = 62195,
[6] = 62196,
};
metier_item_cfg[76108]={
[1] = 76108,
[2] = 76109,
[3] = 76110,
[4] = 76111,
[5] = 76112,
[6] = 76113,
};
metier_item_cfg[76114]={
[1] = 76114,
[2] = 76115,
[3] = 76115,
[4] = 76116,
[5] = 76116,
[6] = 76114,
};
metier_item_cfg[76117]={
[1] = 76117,
[2] = 76118,
[3] = 76118,
[4] = 76119,
[5] = 76119,
[6] = 76117,
};
metier_item_cfg[76106]={
[1] = 76106,
[2] = 76106,
[3] = 76106,
[4] = 76107,
[5] = 76107,
[6] = 76107,
};
metier_item_cfg[63877]={
[1] = 63877,
[2] = 63878,
[3] = 63879,
[4] = 63880,
[5] = 63881,
[6] = 63882,
};

metier_item_cfg[69485]={
[1] = 69485,
[2] = 69484,
[3] = 69482,
[4] = 69486,
[5] = 69483,
[6] = 69487,
};
metier_item_cfg[1944]={
[1] = 1944,
[2] = 13249,
[3] = 13627,
[4] = 14391,
[5] = 14005,
[6] = 14769,
};

-- 性别自适应道具解析表
jender_item_cfg = {};
jender_item_cfg[64573] = {
[2] = 64573,
[3] = 64574,
};


-- 传送点表
teleport_points={};
teleport_points[711970] = {
line_id=0,
map_id=67,
x=115,
y=26,
};
-- 绝世神器乾坤袋
main_cfg[77796]={
	{ item_code="m-1944", item_id=1944, num=1, prob=10000, notify_type=0, is_bag=0, star=4 },
};