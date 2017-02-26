import "property_pb.hxx";
import "coremessage_pb.hxx";

// 以最快速度序列/反序列
option optimize_for = SPEED;

// 把所有消息的ID集中再次，防止重复定义
// promessage消息段ID范围(0x0800 -- 0x0BFF)
enum PRO_MSG_ID
{	
	ID_S2C_NOTIFY_YOURPRODATA					= 0x0800;		// 服务器->客户端 你的数据信息
	ID_S2C_NOTIFY_CREATEPLAYER					= 0x0801;		// 服务器->客户端 创建玩家
	ID_S2C_NOTIFY_CREATENPC						= 0x0802;		// 服务器->客户端 创建NPC
	ID_C2S_REQUEST_USESKILL						= 0x0803;		// 客户端->服务器 使用技能
	ID_C2S_REQUEST_NORMALATTACK					= 0x0804;		// 客户端->服务器 普通攻击
	ID_C2S_REQUEST_PICKITEM						= 0x0805;		// 客户端->服务器 拾取物品
	ID_C2S_REQUEST_CLICKNPC						= 0x0806;		// 客户端->服务器 点击功能NPC
	ID_S2C_RESPONSE_CLICKNPC					= 0x0807;		// 服务器->客户端 点击功能NPC
	ID_S2C_NOTIFY_CLOSEDIALOG					= 0x0808;		// 服务器->客户端 关闭对话通知
	ID_S2C_NOTIFY_CHANGEAPPERTAININGS			= 0x0809;		// 服务器->客户端 通知修改物品归属
	ID_S2C_NOTIFY_UPDATEMONEY					= 0x0811;		// 服务器->客户端 获得金钱通知
	ID_C2S_REQUEST_PURCHASEITEM					= 0x0812;		// 客户端->服务器 购买道具请求
	ID_C2S_REQUEST_SELLITEM						= 0x0813;		// 客户端->服务器 出售道具请求
	ID_S2C_NOTIFY_REMOVEITEM					= 0x0814;		// 服务器->客户端 删除道具通知
	ID_S2C_NOTIFY_REMOVETASKITEM				= 0x0815;		// 服务器->客户端 删除任务道具通知
	ID_C2S_REQUEST_THROWITEM					= 0x0816;		// 客户端->服务器 丢弃道具请求
	ID_S2C_NOTIFY_THROWITEM						= 0x0817;		// 服务器->客户端 丢弃道具通知
	ID_C2S_REQUEST_THROWMONEY					= 0x0818;		// 客户端->服务器 丢弃金钱请求
	ID_C2S_REQUEST_SETSHORTCUT					= 0x0819;		// 客户端->服务器 设置快捷键请求
	ID_S2C_NOTIFY_ENTITYRELIFE					= 0x081A;		// 服务器->客户端 实体复活通知
	ID_S2C_NOTIFY_ENTITYDISAPPEAR				= 0x081B;		// 服务器->客户端 实体消失通知
	ID_C2S_REQUEST_PLAYERRELIVE					= 0x081C;		// 客户端->服务器 玩家复活请求
	ID_S2C_NOTIFY_PLAYERRELIVE					= 0x081D;		// 服务器->客户端 玩家复活通知
	ID_S2C_NOTIFY_REMOVEBUFF					= 0x081E;		// 服务器->客户端 移除BUFF通知
	ID_S2C_NOTIFY_CHANGESTATE					= 0x081F;		// 服务器->客户端 状态改变通知
	ID_S2C_NOTIFY_USESKILL						= 0x0820;		// 服务器->客户端 使用技能通知
	ID_S2C_NOTIFY_FUNCRESULT					= 0x0821;		// 服务器->客户端 功能结果 = PROMSG + 0; 技能 = PROMSG + 0;道具和Buff造成的结果都是功能组成的
	ID_S2C_NOTIFY_CHANGEMETIER					= 0x0822;		// 服务器->客户端 改变职业的通知
	ID_S2C_NOTIFY_LOCKITEM						= 0x0823;		// 服务器->客户端 锁定物品的通知
	ID_S2C_NOTIFY_LEVELUP						= 0x0824;		// 服务器->客户端 升级的通知
	ID_S2C_NOTIFY_OBTAINEXP						= 0x0825;		// 服务器->客户端 获得经验的通知
	ID_C2S_REQUEST_MOVEITEM						= 0x0826;		// 客户端->服务器 .物品的请求
	ID_S2C_NOTIFY_MOVEITEM						= 0x0827;		// 服务器->客户端 移动物品的通知
	ID_S2C_NOTIFY_INSERTITEM					= 0x0828;		// 服务器->客户端 获得物品的通知
	ID_S2C_NOTIFY_PRODEBUG						= 0x0829;		// 服务器->客户端 调试命令
	ID_S2C_NOTIFY_PROERROR						= 0x082A;		// 服务器->客户端 调试错误信息
	ID_C2S_REQUEST_EQUIPITEM					= 0x082B;		// 客户端->服务器 装备物品的请求
	ID_C2S_REQUEST_UNEQUIPITEM					= 0x082C;		// 客户端->服务器 卸下装备的请求
	ID_S2C_NOTIFY_EQUIPITEM						= 0x082D;		// 服务器->客户端 装备物品的通知
	ID_S2C_NOTIFY_UNEQUIPITEM					= 0x082E;		// 服务器->客户端 卸下装备的通知
	ID_S2C_NOTIFY_PLAYERCHANGEEQUIQ				= 0x082F;		// 服务器->客户端 玩家换装的通知
	ID_S2C_NOTIFY_ENTITYREFRESH					= 0x0830;		// 服务器->客户端 实体更新的通知
	ID_C2S_REQUEST_CLICKSKILLPOINT				= 0x0831;		// 客户端->服务器 加技能点的请求
	ID_S2C_RESPONSE_CLICKSKILLPOINT				= 0x0832;		// 服务器->客户端 加技能点的回应
	ID_C2S_REQUEST_CLICKPROPERTYPOINT			= 0x0833;		// 客户端->服务器 加属性点的请求
	ID_S2C_RESPONSE_CLICKPROPERTYPOINT			= 0x0834;		// 服务器->客户端 加属性点的回应
	ID_C2S_REQUEST_RESETSPP						= 0x0835;		// 客户端->服务器 重置1点属性点
	ID_C2S_REQUEST_RESETAPP						= 0x0836;		// 客户端->服务器 重置所有属性点
	ID_S2C_RESPONSE_RESETSPP					= 0x0837;		// 服务器->客户端 重置1点属性点
	ID_S2C_RESPONSE_RESETAPP					= 0x0838;		// 服务器->客户端 重置所有属性点
	ID_C2S_REQUEST_RESETSSP						= 0x0839;		// 客户端->服务器 重置1点技能点
	ID_C2S_REQUEST_RESETASP						= 0x083A;		// 客户端->服务器 重置所有技能点
	ID_S2C_RESPONSE_RESETSSP					= 0x083B;		// 服务器->客户端 重置1点技能点
	ID_S2C_RESPONSE_RESETASP					= 0x083C;		// 服务器->客户端 重置所有技能点
	ID_S2C_NOTIFY_ENTITYDIE						= 0x083D;		// 服务器->客户端 实体死亡的通知
	ID_C2S_REQUEST_GETROLEPERSONALINFOBYID		= 0x083E;		// 客戶端->服務器 通过ID得到玩家信息
	ID_C2S_REQUEST_GETROLEPERSONALINFOBYNAME	= 0x083F;		// 客戶端->服務器 通过名字得到玩家信息
	ID_S2C_RESPONSE_SENDROLEPERSONALINFO		= 0x0840;		// 服务器->客户端 发送玩家信息
	ID_C2S_REQUEST_SETROLEPERSONALINFO			= 0x0841;		// 客户端->服务器 修改玩家信息
	ID_S2C_RESPONSE_ROLEPERSONALINFOBACK		= 0x0842;		// 服务器->客户端 玩家修改资料回馈
	ID_C2S_REQUEST_USEITEM						= 0x0843;		// 客户端->服务器 使用物品
	ID_S2C_RESPONSE_USEITEM						= 0x0844;		// 服务器->客户端 使用物品
	ID_S2C_NOTIFY_PLAYERUSEITEM					= 0x0845;		// 服务器->客户端 玩家使用道具
	ID_S2C_RESPONSE_TELEPORT					= 0x0846;		// 服务器->客户端 传送
	ID_C2S_REQUEST_PRODUCEEQUIP					= 0x0847;		// 客户端->服务器 生产装备
	ID_C2S_REQUEST_UPGADDITEM					= 0x0848;		// 客户端->服务器 放置升级物品
	ID_S2C_RESPONSE_UPGADDITEM					= 0x0849;		// 服务器->客户端 放置升级物品
	ID_C2S_REQUEST_UPGDELITEM					= 0x084A;		// 客户端->服务器 拿回升级物品
	ID_S2C_RESPONSE_UPGDELITEM					= 0x084B;		// 服务器->客户端 拿回升级物品
	ID_C2S_REQUEST_UPGEQUIP						= 0x084C;		// 客户端->服务器 升级装备
	ID_S2C_NOTIFY_UPGEQUIP						= 0x084D;		// 服务器->客户端 升级装备结果
	ID_C2S_REQUEST_PRODUCEPHY					= 0x084E;		// 客户端->服务器 生产药品
	ID_S2C_NOTIFY_PROFIN						= 0x084F;		// 服务器->客户端 生产结果
	ID_S2C_NOTIFY_PROBREAK						= 0x0850;		// 服务器->客户端 生产中断
	ID_C2S_REQUEST_PUSHRUNE						= 0x0853;		// 客户端->服务器 放符文
	ID_S2C_RESPONSE_PUSHRUNE					= 0x0854;		// 服务器->客户端 放符文
	ID_C2S_REQUEST_POPRUNE						= 0x0855;		// 客户端->服务器 拿符文
	ID_S2C_RESPONSE_POPRUNE						= 0x0856;		// 服务器->客户端 拿符文
	ID_C2S_REQUEST_INSERTRUNE					= 0x0857;		// 客户端->服务器 镶符文
	ID_S2C_RESPONSE_INSERTRUNE					= 0x0858;		// 服务器->客户端 镶符文
	ID_C2S_REQUEST_PUSHJEWEL					= 0x0859;		// 客户端->服务器 放宝石
	ID_S2C_RESPONSE_PUSHJEWEL					= 0x085A;		// 服务器->客户端 放宝石
	ID_C2S_REQUEST_POPJEWEL						= 0x085B;		// 客户端->服务器 拿宝石
	ID_S2C_RESPONSE_POPJEWEL					= 0x085C;		// 服务器->客户端 拿宝石
	ID_C2S_REQUEST_INSERTJEWEL					= 0x085D;		// 客户端->服务器 镶宝石
	ID_S2C_RESPONSE_INSERTJEWEL					= 0x085E;		// 服务器->客户端 镶宝石
	ID_C2S_REQUEST_PUSHSLOT						= 0x085F;		// 客户端->服务器 放开槽
	ID_S2C_RESPONSE_PUSHSLOT					= 0x0860;		// 服务器->客户端 放开槽
	ID_C2S_REQUEST_POPSLOT						= 0x0861;		// 客户端->服务器 拿开槽
	ID_S2C_RESPONSE_POPSLOT						= 0x0862;		// 服务器->客户端 拿开槽
	ID_C2S_REQUEST_OPENSLOT						= 0x0863;		// 客户端->服务器 开槽
	ID_S2C_RESPONSE_OPENSLOT					= 0x0864;		// 服务器->客户端 开槽
	ID_C2S_REQUEST_PUSHEAT						= 0x0865;		// 客户端->服务器 放吃装备
	ID_S2C_RESPONSE_PUSHEAT						= 0x0866;		// 服务器->客户端 放吃装备
	ID_C2S_REQUEST_POPEAT						= 0x0867;		// 客户端->服务器 拿吃装备
	ID_S2C_RESPONSE_POPEAT						= 0x0868;		// 服务器->客户端 拿吃装备
	ID_C2S_REQUEST_EATEQUIP						= 0x0869;		// 客户端->服务器 吃装备
	ID_S2C_RESPONSE_EATEQUIP					= 0x086A;		// 服务器->客户端 吃装备
	ID_C2S_REQUEST_PUSHCOMPOSE					= 0x086B;		// 客户端->服务器 放合成
	ID_S2C_RESPONSE_PUSHCOMPOSE					= 0x086C;		// 服务器->客户端 放合成
	ID_C2S_REQUEST_POPCOMPOSE					= 0x086D;		// 客户端->服务器 拿合成
	ID_S2C_RESPONSE_POPCOMPOSE					= 0x086E;		// 服务器->客户端 拿合成
	ID_C2S_REQUEST_COMPOSE						= 0x086F;		// 客户端->服务器 合成
	ID_S2C_RESPONSE_COMPOSE						= 0x0870;		// 服务器->客户端 合成
	ID_C2S_REQUEST_PLAYERTELE					= 0x0871;		// 客户端->服务器 玩家传送
	ID_S2C_RESPONSE_PLAYERTELE					= 0x0872;		// 服务器->客户端 玩家传送
	ID_S2C_RANDMOVEDIRECTION					= 0x0873;		// 服务器->客户端 玩家随即移动
	ID_C2S_DEBUGCOMMAND							= 0x0875;		// 客户端->服务器 调试命令
	ID_C2S_NOTIFY_DIRECTION						= 0x0876;		// 客户端->服务器 同步玩家方向
	ID_S2C_NOTIFY_DIRECTION						= 0x0877;		// 服务器->客户端 同步玩家方向
	ID_C2S_REQUEST_CHANGEPETMODE				= 0x0878;		// 客户端->服务器 改变宠物模式
	ID_S2C_RESPONSE_CHANGEPETMODE				= 0x0879;		// 服务器->客户端 改变宠物模式
	ID_S2C_NOTIFY_PETLEVELUP					= 0x087A;		// 服务器->客户端 召唤兽临时等级提升
	ID_S2C_NOTIFY_CHANGEHARDINESS               = 0x087B;       // 服务器->客户端 玩家装备耐久度改变
	ID_S2C_NOTIFY_SYNCPOS						= 0x087C;		// 服务器->客户端 同步位置数据
	ID_C2S_REQUEST_MENDEQUIPMENT                = 0x087D;       // 客户端->服务器 修理装备请求
	ID_S2C_RESPONSE_MENDEQUIPMENT               = 0x087E;       // 服务器->客户端 修理装备完成
	ID_S2C_NOTIFY_SPEEDCHANGED					= 0x087F;		// 服务器->客户端 玩家移动速度改变
	ID_S2C_NOTIFY_PROEXP						= 0x0880;		// 服务器->客户端 玩家生产经验等级改变
	ID_C2S_REQUEST_SALEITEM						= 0x0881;		// 客户端->服务器 NPC交易回购商品信息(售出的)
	ID_S2C_RESPONSE_SALEITEM					= 0x0882;		// 服务器->客户端 NPC交易回购商品信息(售出的)
	ID_C2S_REQUEST_BUYBACK						= 0x0883;		// 客户端->服务器 购回售出给NPC的商品
	ID_C2S_REQUEST_MOVESTORAGEITEM              = 0x0884;       // 客户端->服务器 在包裹和仓库之间移动物品     
	ID_C2S_REQUEST_CLOSEWINDOW					= 0x0885;		// 客户端->服务器 关闭指定的窗口
	ID_S2C_RESPONSE_CLOSEWINDOW					= 0x0886;		// 服务器->客户端 关闭指定的窗口
	ID_S2C_RESPONSE_BINDITEM                    = 0x0887;       // 服务器->客户端 发送道具绑定的消息
	ID_C2S_REQUEST_BINDITEMREQ                  = 0x0888;       // 客户端->服务器 请求绑定的通知
	ID_S2C_NOTIFY_ERRORACTONBINDITEM            = 0x0889;       // 服务器->客户端 对绑定装备的错误操作
	ID_C2S_REQUEST_ADDITEMREQUEST               = 0x088A;       // 客户端->服务器 添加绑定的物品
	ID_S2C_RESPONSE_ADDITEMREPONSE              = 0x088B;       // 服务器->客户端 添加绑定物品的确认
	ID_C2S_REQUEST_TAKEITEMREQUEST              = 0x088C;       // 客户端->服务器 取下绑定物品的请求
	ID_S2C_RESPONSE_TAKEITEMRESPONSE            = 0x088D;       // 服务器->客户端 取下物品的回应 
	ID_C2S_REQUEST_ADDITEMFORFREE               = 0x088E;       // 客户端->服务器 对添加解除物品的请求
	ID_S2C_RESPONSE_ADDITEMFORFREE              = 0x088F;       // 服务器->客户端 对添加解除物品请求的回应
	ID_C2S_REQUEST_TAKEITEMFORFREE              = 0x0890;       // 客户端->服务器 卸下待绑定物品的请求
	ID_S2C_RESPONSE_TAKEITEMFORFREE             = 0x0891;       // 服务器->客户端 卸下带绑定物品的回应
	ID_C2S_REQUEST_FREEITEM                     = 0x0892;       // 客户端->服务器 解除绑定物品的请求
	ID_S2C_RESPONSE_FREEITEM                    = 0x0893;	    // 服务器->客户端 解除绑定物品请求的回应	
	ID_S2C_NOTIFY_RIDEHORSE						= 0x0894;		// 服务器->客户端 骑马通知
	ID_C2S_REQUEST_USETASKITEM					= 0x0895;       // 客户端->服务器 使用任务道具请求
	ID_S2C_NOTICE_USETASKITEM					= 0x0896;       // 服务器->客户端 使用任务道具通知
	ID_C2S_REQUEST_STUDYSKILL					= 0x0897;		// 客户端->服务器 学习技能
	ID_S2C_RESPONSE_STUDYSKILL					= 0x0898;		// 服务器->客户端 学习技能
	ID_C2S_REQUEST_ADDPROPERTYPOINT				= 0x0899;		// 客户端->服务器 加属性点
	ID_S2C_RESPONSE_ADDPROPERTYPOINT			= 0x089A;		// 服务器->客户端 加属性点
	ID_C2S_REQUEST_CHANGEPKMODE					= 0x089D;		// 客户端->服务器 切换PK模式
	ID_S2C_RESPONSE_CHANGEPKMODE				= 0x089E;		// 服务器->客户端 切换PK模式
	ID_C2S_REQUEST_SETTOWN						= 0x08A1;		// 客户端->服务器 设定回城
	ID_S2C_RESPONSE_SETTOWN						= 0x08A2;		// 服务器->客户端 设定回城
	ID_C2S_REQUEST_LOOKPROPERTY					= 0x08A3;		// 客户端->服务器 观察玩家装备的请求
	ID_S2C_RESPONSE_LOOKPROPERTY				= 0x08A4;		// 服务器->客户端 观察玩家装备的回复
	ID_S2S_REQUEST_LOOKPROPERTY					= 0x0AA5;		// 服务器->路由 观察玩家装备的请求 0x08A5 + 0x0200
	ID_S2S_RESPONSE_LOOKPROPERTY				= 0x0AA6;		// 服务器->路由 观察玩家装备的回复 0x08A6 + 0x0200
	ID_S2C_FUNCRESULT_CALLBACK					= 0x0AA7;		// 服务器->客户端  技能使用结果
	ID_C2S_REQUEST_THROWSTORITEM                = 0x08A8;		// 客户端->服务器  扔掉仓库物品的请求
	ID_C2S_RESPONSE_THROWSTORITEM               = 0x08A9;		// 服务器->客户端  扔掉仓库物品的回应
	ID_C2S_NOTIFY_ITEMHASBINDED                 = 0x08AA;		// 服务器->客户端  物品已经被绑定的通知	
	ID_S2C_RESPONSE_BUYBACK						= 0x08AB;		// 服务器->客户端 回购返回消息
	ID_S2C_RESPONSE_SELLITEM					= 0x08AC;		// 服务器->客户端 道具售出回复
	ID_S2C_NOTIFY_STORAGEITEM					= 0x08AD;		// 服务器->客户端 给客户端发送仓库物品消息
	ID_C2S_REQUEST_TASKCOLLECT					= 0x08AE;		// 客户端->服务器 请求采集任务物品
	ID_C2S_REQUEST_CHANGEHELPSTATUS				= 0x08AF;		// 客户端->服务器 请求改变帮助状态	
	ID_C2S_REQUEST_CLICKBINDSERVICE				= 0x08B0;		// 客户端->服务器 打开npc绑定服务界面的请求
	ID_S2C_RESPONSE_CLICKBINDSERVICE		    = 0x08B1;		// 服务器->客户端 打开npc绑定服务界面的回应
	ID_C2S_REQUEST_ADDEQTFORRESUMEBIND			= 0x08B2;		// 客户端->服务器 放置取消解除手动绑定的物品的请求
	ID_S2C_RESPONSE_ADDEQTFORRESUMEBIND  		= 0x08B3;		// 服务器->客户端 放置取消解除手动绑定的物品的回应
	ID_C2S_REQUEST_TAKEEQTFORRUMEBIND			= 0x08B4;		// 客户端->服务器 拿下需要取消解除手动绑定的物品的请求
	ID_S2C_RESPONSE_TAKEEQTFORRUMEBIND			= 0x08B5;		// 服务器->客户端 拿下需要取消解除手动绑定的物品的回应
	ID_C2S_REQUEST_RESUMEBIND					= 0x08B6;		// 客户端->服务器 回复物品的手动绑定的请求
	ID_S2C_RESPONSE_RESUMEBIND					= 0x08B7;		// 服务器->客户端 回复物品的手动绑定的回应
	ID_C2S_REQUEST_OBTAINDIPLOIDTIME			= 0x08B8;		// 客户端->服务器 领取双倍时间请求
	ID_S2C_NOTIFY_CHANGEHONOR					= 0x08B9;		// 服务器->客户端 玩家功勋改变
	ID_C2S_REQUEST_HONORTOEXP					= 0x08BA;		// 客户端->服务器 玩家功勋换经验
	ID_S2C_NOTIFY_SERVERREFRESH					= 0x08BB;		// 服务器->客户端 服务器刷新数据
	ID_C2S_REQUEST_QUERYREMAINTIME				= 0x08BC;		// 客户端->服务器 查询剩余的领双时间
	ID_S2C_RESPONSE_REMAINTIMENOTICE			= 0x08BD;		// 服务器->客户端 通知剩余领双时间
	ID_S2C_RESPONSE_BINDMONEYNOTICE				= 0x08CE;		// 服务器->客户端 通知绑定金钱改变
	ID_C2S_REQUEST_USECONTINUEDSKILL			= 0x08C0;		// 客户端->服务器 使用持续技能的请求
	ID_C2S_REQUEST_CLICKREDSTONDE				= 0x08CF;		// 客户端->服务器 使用内丹的请求
	ID_S2C_RESPONSE_CLICKREDSTONDE				= 0x08D0;		// 服务器->客户端 使用内丹的回应
	ID_S2C_NOTIFY_STOPRENDSTONETIMEER			= 0x08D1;		// 服务器->客户端 停止使用内丹
	ID_C2S_NOTIFY_REDSTONETIMEREND				= 0x08D2;		// 客户端->服务器 内丹读秒结束的消息
	ID_S2C_NOTIFY_EXPMAGICEND					= 0x08D3;		// 服务器->客户端 噬炼结束
	ID_C2S_REQUEST_FREEZEDIPLOID				= 0x08D4;		// 客户端->服务器 冻结(解冻)双倍时间
	ID_S2C_RESPONSE_FREEZEDIPLOID				= 0x08D5;		// 服务器->客户端 冻结(解冻)双倍时间
	ID_S2C_RESPONSE_OBTAINDIPLOID				= 0x08D6;		// 服务器->客户端 回应领双请求
	ID_S2C_NOTIFY_CHANGEREDSTONESTATUS			= 0x08D7;		// 服务器->客户端 改变玩家的噬炼状态
	ID_C2S_REQUEST_CLICKWELFARE					= 0x08D8;		// 客户端->服务器 点击npc每日问答服务的请求
	ID_S2C_RESPONSE_CLICKWELFARE				= 0x08D9;		// 服务器->客户端 点击npc每日问答服务的回应		
	ID_C2S_CHANGE_CAMP_REQUEST					= 0x08DA;		// client -> server change player camp request	
	ID_S2C_CHANGE_CAMP_RESPONSE					= 0x08DB;		// server -> client player change camp response
	ID_S2C_NOTIFY_INSERTEQUIP					= 0x08DC;		// 服务器->客户端 插入新装备
	ID_C2S_REQUEST_OPEN_SERVICE					= 0x08E0;		// 客户端->服务器 打开服务请求
	ID_S2C_RESPONSE_OPEN_SERVICE				= 0x08E1;		// 服务器->客户端 打开服务回复
	ID_S2C_NOTIFY_REFRESHPROPERTY				= 0x08E2;		// 服务器->客户端 刷新属性通知
	ID_S2C_NOTIFY_CHANGEPKTYPE					= 0x08E3;		// 服务器->客户端 改变PK类型通知
	ID_S2C_NOTIFY_REDSTONESTATRED				= 0x08E4;		// 服务器->客户端 内丹已经被成功开启
	ID_C2S_REQUEST_ACTIVEREMOVEBUFF				= 0x08E5;		// 客户端->服务器 手动删除BUFF的请求
	ID_S2C_NOTIFY_BEGINREDSTONEMAGIC			= 0x08E6;		// 服务器->客户端 内丹进入噬炼状态
	ID_S2C_NOTIFY_CHANGEENTITYPKTYPE			= 0x08E7;		// 服务器->客户端 改变实体PK类型
	ID_S2C_NOTIFY_BRUSHOGRE						= 0x08E8;		// 刷怪通知(藏宝图)	
	
	ID_C2S_REQUEST_SETPASSWORD					= 0x08E9;		// 设置密码的请求
	ID_S2C_RESPONSE_SETPASSWORD					= 0x08EA;		// 设置密码的回应
	ID_C2S_REQUEST_CHECKPASSWORD				= 0x08EB;		// 检查密码的请求
	ID_S2C_RESPONSE_CHECKPASSWORD				= 0x08EC;		// 检查密码的回应
	ID_C2S_REQUEST_MODIFYPASSWORD				= 0x08ED;		// 修改密码的请求
	ID_S2C_RESPONSE_MODIFYPASSWORD				= 0x08EE;		// 修改密码的回应消息
	ID_C2S_NOTIFY_SETPASSWORDOPTION				= 0x08EF;		// 设置密码选项	

	ID_C2S_REQUEST_CLICKSYMBOL					= 0x08F0;		// 打开传送符的传送表请求
	ID_S2C_RESPONSE_CLICKSYMBOL					= 0x08F1;		// response

	ID_C2S_REQUEST_ADDSLOT						= 0x08F6;		// 客户端->服务器 增加凹槽请求
	ID_S2C_RESPONSE_ADDSLOT						= 0x08F7;		// 服务器->客户端 增加凹槽回应
	ID_C2S_REQUEST_REMOVEJEWEL					= 0x08F8;		// 客户端->服务器 摘除宝石请求
	ID_S2C_RESPONSE_REMOVEJEWEL					= 0x08F9;		// 服务器->客户端 摘除宝石回应
	ID_C2S_REQUEST_JUDGE						= 0x08FA;		// 客户端->服务器 鉴定装备请求
	ID_S2C_RESPONSE_JUDGE						= 0x08FB;		// 服务器->客户端 鉴定装备回应
	ID_C2S_REQUEST_PUSHADDSLOT					= 0x08FC;		// 客户端->服务器 增加凹槽放入请求
	ID_S2C_RESPONSE_PUSHADDSLOT					= 0x08FD;		// 服务器->客户端 增加凹槽放入回应
	ID_C2S_REQUEST_POPADDSLOT					= 0x08FE;		// 客户端->服务器 增加凹槽拿出请求
	ID_S2C_RESPONSE_POPADDSLOT					= 0x08FF;		// 服务器->客户端 增加凹槽拿出回应
	ID_C2S_REQUEST_PUSHREMOVEJEWEL				= 0x0900;		// 客户端->服务器 摘除宝石放入请求
	ID_S2C_RESPONSE_PUSHREMOVEJEWEL				= 0x0901;		// 服务器->客户端 摘除宝石放入回应
	ID_C2S_REQUEST_POPREMOVEJEWEL				= 0x0902;		// 客户端->服务器 摘除宝石拿出请求
	ID_S2C_RESPONSE_POPREMOVEJEWEL				= 0x0903;		// 服务器->客户端 摘除宝石拿出回应
	ID_C2S_REQUEST_PUSHJUDGE					= 0x0904;		// 客户端->服务器 鉴定装备放入请求
	ID_S2C_RESPONSE_PUSHJUDGE					= 0x0905;		// 服务器->客户端 鉴定装备放入回应
	ID_C2S_REQUEST_POPJUDGE						= 0x0906;		// 客户端->服务器 鉴定装备拿出请求
	ID_S2C_RESPONSE_POPJUDGE					= 0x0907;		// 服务器->客户端 鉴定装备拿出回应
    
    ID_C2S_REQUEST_STORMONEY					= 0x0908;		// 客户端->服务器 存储金钱的请求消息
    ID_S2C_RESPONSE_STORMONEY					= 0x0909;		// 服务器->客户端 存储金钱的消息回应
    
    ID_C2S_REQUEST_TAKESTORMONEY				= 0x090A;		// 客户端->服务器 从仓库取出金钱的消息请求
    ID_S2C_RESPONSE_TAKESTORMONEY				= 0x090B;		// 服务器->客户端 从客户端取出金钱的消息回应            
    
	ID_C2S_REQUEST_ADDKITBAG				= 0x090C;		// 客户端->服务器 放置背包到仓库中的请求消息
	ID_S2C_RESPONSE_ADDKITBAG				= 0x090D;		// 服务器->客户端 放置背包到仓库中的回应消息
	
	ID_C2S_REQUEST_OPENKITBAG				= 0x090E;		// 客户端->服务器 打开仓库背包的请求消息	
	ID_S2C_RESPONSE_OPENKITBAG				= 0x090F;		// 服务器->客户端 打开仓库背包的回应消息
	
	ID_C2S_REQUEST_MOVEKITBAG					= 0x0910;		// 客户端->服务器 移动背包的消息请求
	ID_S2C_RESPONSE_MOVEKITBAG					= 0x0911;		// 服务器->客户端 移动背包的消息回应

	ID_C2S_REQUEST_PUSHCHANGE					= 0x0912;		// 客户端->服务器 转化抗性放入请求
	ID_S2C_RESPONSE_PUSHCHANGE					= 0x0913;		// 服务器->客户端 转化抗性放入回应
	ID_C2S_REQUEST_POPCHANGE					= 0x0914;		// 客户端->服务器 转化抗性拿出请求
	ID_S2C_RESPONSE_POPCHANGE					= 0x0915;		// 服务器->客户端 转化抗性拿出回应
	ID_C2S_REQUEST_CHANGE						= 0x0916;		// 客户端->服务器 转化抗性请求
	ID_S2C_RESPONSE_CHANGE						= 0x0917;		// 服务器->客户端 转化抗性回应
	ID_C2S_REQUEST_PUSHJEWCOM					= 0x0918;		// 客户端->服务器 宝石合成放入请求
	ID_S2C_RESPONSE_PUSHJEWCOM					= 0x0919;		// 服务器->客户端 宝石合成放入回应
	ID_C2S_REQUEST_POPJEWCOM					= 0x091A;		// 客户端->服务器 宝石合成拿出请求
	ID_S2C_RESPONSE_POPJEWCOM					= 0x091B;		// 服务器->客户端 宝石合成拿出回应
	ID_C2S_REQUEST_JEWCOM						= 0x091C;		// 客户端->服务器 宝石合成请求
	ID_S2C_RESPONSE_JEWCOM						= 0x091D;		// 服务器->客户端 宝石合成回应

    ID_C2S_REQUEST_LISTACTIVITYMENU             = 0x091E;		// 客户端->服务器 点击活动NPC获取活动菜单请求
	ID_S2C_RESPONSE_LISTACTIVITYMENU            = 0x091F;		// 服务器->客户端 点击活动NPC获取的菜单列表
    ID_C2S_REQUEST_LISTACTIVITYITEM             = 0x0920;		// 客户端->服务器 点击活动后的物品列表请求
    ID_S2C_RESPONSE_LISTACTIVITYITEM            = 0x0921;		// 服务器->客户端 点击活动后的物品列表应答
    ID_C2S_REQUEST_MINUSACTIVITYITEM            = 0x0922;		// 客户端->服务器 获取活动物品请求         


	ID_C2S_REQUEST_INTERRUPTSKILL				= 0x0923;		// 客户端->服务器 请求打断技能请求
	ID_S2C_NOTIFY_INTERRUPTSKILL				= 0x0924;		// 服务器->客户端 通知打断技能
	ID_S2C_NOTIFY_USECONTINUESKILL				= 0x0925;		// 服务器->客户端 通知使用持续性技能
	ID_C2S_REQUEST_MULTICOM						= 0x0926;		// 客户端->服务器 复杂合成请求
	ID_S2C_RESPONSE_MULTICOM					= 0x0927;		// 服务器->客户端 复杂合成回应

	ID_S2C_NOTIFY_MODIFYPKVALUE					= 0x0928;		// 服务器->客户端 通知客户端改变PK值(增量改变、直接设置)
	ID_S2C_NOTIFY_YELLOWSTATE					= 0x0929;		// 服务器->客户端 黄名状态通知

	ID_S2C_NOTIFY_EVENT							= 0x092A;		// 服务器->客户端 场景事件通知
	ID_S2C_NOTIFY_ACTIVECARD					= 0x092B;		// 服务器->客户端 激活卡片通知
	ID_C2S_REQUEST_PUSHCARD						= 0x092C;		// 客户端->服务器 放入卡片请求
	ID_S2C_RESPONSE_PUSHCARD					= 0x092D;		// 服务器->客户端 放入卡片回应

	ID_S2C_NOTIYFY_TALENTCHANGE					= 0x092E;		// 服务器->客户端 修为值造化点改变通知
	ID_C2S_REQUEST_CLICKTALENTPOINT				= 0x092F;		// 客户端->服务器 点击造化点请求
	ID_S2C_RESPONSE_CLICKTALENTPOINT			= 0x0930;		// 服务器->客户端 点击造化点回应
	ID_C2S_REQUEST_INCREASESTAGE				= 0x0931;		// 客户端->服务器 造化进阶请求
	ID_S2C_RESPONSE_INCREASESTAGE				= 0x0932;		// 服务器->客户端 造化进阶回应
	ID_S2C_NOTICE_UPDATETITLE					= 0x0933;		// 服务器->客户端 更新称号
	ID_C2S_REQUEST_SELECTTITLE					= 0x0934;		// 客户端->服务器 选择称号
	ID_S2C_NOTICE_UPDATECURRENTTITLE			= 0x0935;		// 服务器->客户端 更换当前称号

	ID_S2C_NOTICE_ACTIVITYDIPLOID				= 0x0936;		// 服务器->客户端 活动双倍通知
	
	ID_C2S_REQUEST_INITIALLIFESKILL				= 0x0937;		// 客户端->服务器 生活技能入门的请求
	ID_S2C_RESPONSE_INITIALLIFESKILL			= 0x0938;		// 服务器->客户端 生活技能入门的回应
	
	ID_C2S_REQUEST_LEVELUPLIFESKILL				= 0x0939;		// 客户端->服务器 生活技能进阶的请求
	ID_S2C_RESPONSE_LEVELUPLIFESKILL			= 0x093A;		// 服务器->客户端 生活技能进阶的回应
	
	ID_C2S_REQUEST_STUDYCOMPOSE					= 0x093B;		// 客户端->服务器 学习新的技能配方的请求
	ID_S2C_RESPONSE_STUDYCOMPOSE				= 0x093C;		// 服务器->客户端 学习新的技能配方的回应消息
	
	ID_C2S_REQUEST_SETMAGICSTONE				= 0x093D;		// 客户端->服务器 附魔的请求消息
	ID_S2C_RESPONSE_SETMAGICSTONE				= 0x093E;		// 服务器->客户端 附魔的消息回应
	
	ID_C2S_REQUEST_FORGETLIFESKILL				= 0x093F;		// 客户端->服务器 遗忘生活技能请求
	ID_S2C_RESPONSE_FORGETLIFESKILL				= 0x0940;		// 服务器->客户端 遗忘生活技能回应
	
	ID_C2S_REQUEST_DECOMPOUNDEQUIP				= 0x0941;		// 客户端->服务器 分解装备的请求
	ID_S2C_RESPONSE_DECOMPOUNDEQUIP				= 0x0942;		// 服务器->客户端 分解装备的回应消息	
	
	ID_C2S_REQUEST_ADDMATERIAL					= 0x0943;		// 客户端->服务器 放置材料的消息请求
	ID_S2C_RESPONSE_ADDMATERIAL					= 0x0944;		// 服务器->客户端 放置材料的消息回应	
	
	ID_C2S_REQUEST_TAKEMATERIAL					= 0x0945;		// 客户端->服务器 取下材料的消息请求
	ID_S2C_RESPONSE_TAKEMATERIAL				= 0x0946;		// 服务器->客户端 取下材料的消息回应
	
	ID_S2C_NOTIFY_LIFESKILLATTRIBUTE			= 0x0947;		// 服务器->客户端 生活技能属性改变的通知

	ID_C2S_REQUEST_ADDPOTENTIAL					= 0x0948;		// 客户端->服务器 法宝加点请求
	ID_S2C_RESPONSE_ADDPOTENTIAL				= 0x0949;		// 服务器->客户端 法宝加点回应

	ID_C2S_REQUEST_CHANGEEXPSCALE				= 0x094A;		// 客户端->服务器 分配经验百分比 请求
	ID_S2C_RESPONSE_CHANGEEXPSCALE				= 0x094B;		// 服务器->客户端 分配经验百分比 回应

	ID_S2C_NOTIFY_REFRESHVALUE					= 0x094C;		// 服务器->客户端 刷新法宝数值 通知

	ID_S2C_NOTIFY_ACTIVESKILLBOX				= 0x094D;		// 服务器->客户端 激活技能框 通知
	ID_S2C_NOTIFY_INSERTSKILL					= 0x094E;		// 服务器->客户端 插入新技能 通知
	ID_C2S_REQUEST_MWADDSKILLPOINTS				= 0x094F;		// 客户端->服务器 法宝技能加点 请求
	ID_S2C_RESPONSE_MWADDSKILLPOINTS			= 0x0950;		// 服务器->客户端 法宝技能加点 回应
	ID_C2S_REQUEST_MWJUDGESKILL					= 0x0951;		// 客户端->服务器 法宝技能鉴定 请求

	ID_C2S_REQUEST_MWLOCKBAG					= 0x0952;		// 客户端->服务器 放上法宝 请求
	ID_S2C_RESPONSE_MWLOCKBAG					= 0x0953;		// 服务器->客户端 放上法宝 回应
	ID_C2S_REQUEST_MWTAKEOFF					= 0x0954;		// 客户端->服务器 取下法宝 请求

	ID_C2S_REQUEST_JUDGEPROPERTY				= 0x0955;		// 客户端->服务器 属性鉴定 请求
	ID_S2C_RESPONSE_JUDGEPROPERTY				= 0x0956;		// 服务器->客户端 属性鉴定 回应

	ID_C2S_REQUEST_USESKILLBOOK					= 0x0957;		// 客户端->服务器 使用技能书合成技能 请求

	ID_C2S_REQUEST_MULTIBUFF					= 0x0958;		// 客户端->服务器 组合buff请求

	ID_S2C_CHANGE_CAMP							= 0x0959;
	ID_C2S_CHANGEPKDROP							= 0x095A;	
	
	ID_C2S_REQUEST_GETEUIPEXCHANGELIST			= 0x095B;		// 获取装备兑换列表的请求
	ID_S2C_RESPONSE_GETEUIPEXCHANGELIST			= 0x095C;		// 获取装备兑换列表的回应

	ID_C2S_REQUEST_AUTOHOOK						= 0x095D;		// 自动挂机
	ID_S2C_RESPONSE_AUTOHOOK					= 0x095E;

	ID_C2S_REQUEST_BUYPHYSIC					= 0x095F;		// 挂机买药
	ID_S2C_RESPONSE_BUYPHYSIC					= 0x0960;

	ID_C2S_REQUEST_PUSHMULTI					= 0x0961;		// 客户端->服务器 复杂合成放入请求
	ID_S2C_RESPONSE_PUSHMULTI					= 0x0962;		// 服务器->客户端 复杂合成放入回应
	ID_C2S_REQUEST_POPMULTI						= 0x0963;		// 客户端->服务器 复杂合成拿出请求
	ID_S2C_RESPONSE_POPMULTI					= 0x0964;		// 服务器->客户端 复杂合成拿出回应
	
	ID_C2S_REQUEST_ADDDECOMPOUNDEQUIP			= 0x0965;		// 客户端->服务器 放置待分解装备的消息请求
	ID_S2C_RESPONSE_ADDDECOMPOUNDEQUIP			= 0x0966;		// 服务器->客户端 放置待分解装备的消息回应
	
	ID_C2S_REQUEST_TAKERDECOMPOUNDEQUIP			= 0x0967;		// 客户端->服务器 拿下待分解装备的消息请求
	ID_S2C_RESPONSE_TAKERDECOMPOUNDEQUIP		= 0x0968;		// 服务器->客户端 拿下待分解装备的消息回应
	
	ID_S2C_NOTIFY_PLAYERFCMSTATUS				= 0x0969;		// 服务器 -> 客户端 通知客户端玩家的防沉迷状态

	ID_C2S_MAGICWEAPONCLEARPOINTREQUEST			= 0x096A;		// 客户端->服务器 法宝洗点请求
	ID_S2C_MAGICWEAPONCLEARPOINTRESPONSE		= 0x096B;		// 服务器->客户端 法宝洗点请求回应

	ID_C2S_MAGICWEAPONCLEARPOINT_ADDMWREQUEST	= 0x096C;		// 客户端->服务器 法宝洗点放法宝请求
	ID_S2C_MAGICWEAPONCLEARPOINT_ADDMWRESPONSE	= 0x096D;		// 服务器->客户端 法宝洗点放法宝回应

	ID_C2S_REQUEST_ADDWIZARDPOINT				= 0x096E;		// 客户端->服务器 增加才华等级请求
	ID_S2C_RESPONSE_ADDWIZARDPOINT				= 0x096F;		// 服务器->客户端 增加才华等级回应

	ID_C2S_REQUEST_HONORTOWIZARD				= 0x0970;		// 客户端->服务器 功勋换才华请求

	ID_C2S_REQUEST_GETTREND						= 0x0971;		// 客户端->服务器 算命请求
	ID_S2C_RESPONSE_GETTREND					= 0x0972;		// 服务器->客户端 算命回应

	ID_S2C_NOTIFY_RUNTIMEPARAM					= 0x0973;		// 服务器->客户端 运行时参数

	ID_C2S_GETHELPDATAREQUEST					= 0x0974;		// 客户端->服务器 客户端请求帮助数据
	ID_S2C_GETHELPDATARESPONSE					= 0x0975;		// 服务器->客户端 客户端请求帮助数据 回应	

	ID_S2C_NOTIFY_PRISONTIME					= 0x0976;		// 服务器->客户端 监狱囚禁时间倒计时通知
	ID_S2C_NOTIFY_PICKITEM						= 0x0977;
	
	ID_C2S_REQUEST_TAKEKITBAG					= 0x0978;		// 客户端->服务器   拿下背包的消息请求
	ID_S2C_RESPONSE_TAKEKITBAG					= 0x0979;		// 服务器->客户端	拿下背包的消息回应	
	

	ID_C2S_REQUEST_GETTELELIST					= 0x097A;		// 客户端->服务器   获取传送坐标列表请求
	ID_S2C_RESPONSE_GETTELELIST					= 0x097B;		// 服务器->客户端	获取传送坐标列表回应

	ID_C2S_REQUEST_ADDTELEPOINT					= 0x097C;		// 客户端->服务器   记录传送点请求
	ID_S2C_RESPONSE_ADDTELEPOINT				= 0x097D;		// 服务器->客户端	记录传送点回应

	ID_C2S_REQUEST_DELTELEPOINT					= 0x097E;		// 客户端->服务器   删除传送点请求
	ID_S2C_RESPONSE_DELTELEPOINT				= 0x097F;		// 服务器->客户端	删除传送点回应

	ID_C2S_REQUEST_USETELEITEM					= 0x0980;		// 客户端->服务器   使用传送道具传送请求

	ID_S2C_NOTIFY_VIPNOTICE						= 0x0981;		// 服务器->客户端	VIP 通知

	ID_C2S_REQUEST_QUERY_GOLDENYB					= 0x0982;		// 客户端->服务器 查询帐户元宝余额
	ID_S2C_RESPONSE_QUERY_GOLDENYB					= 0x0983;		// 服务器->客户端 通知客户端未领元宝余额数目
	ID_C2S_REQUEST_DRAW_GOLDENYB					= 0x0984;		// 客户端->服务器 领取元宝到本区请求
	ID_S2C_RESPONSE_DRAW_GLODENYB					= 0x0985;		// 服务器->客户端 领取元宝到本区回应
	
	ID_S2C_NOTIFY_USEITEM							= 0x0986;		// 服务器->客户端 广播玩家使用道具情况
	
	ID_C2S_REQUEST_SETPWDPROTECTTIME				= 0x0987;		// 客户端->服务器 设置密码安全保护时间的请求
	ID_S2C_RESPONSE_SETPWDPROTECTTIME				= 0x0988;		// 服务器->客户端 设置密码安全保护时间的消息回应	

	ID_S2C_NOTIFY_ACTIVITYCHANGED					= 0x0989;		// 服务器->客户端 活动状态变更

	ID_C2S_REQUEST_USEFLOWER						= 0x098A;		// 客户端->服务器 使用鲜花请求
	ID_S2C_RESPONSE_USEFLOWER						= 0x098B;		// 服务器->客户端 使用鲜花回应	
	ID_S2C_NOTIFY_OBTAINFLOWER						= 0x098C;		// 服务器->客户端 获赠鲜花通知

	ID_S2S_REQUEST_USEFLOWER						= 0x0B8D;		// 服务器->服务器 使用鲜花请求 +0x0200
	ID_S2S_RESPONSE_USEFLOWER						= 0x0B8E;		// 服务器->服务器 使用鲜花回应 +0x0200

	ID_S2C_RESPONSE_MINUSACTIVITYITEM				= 0x0B8F;		// 服务器->客户端 获取活动物品请求回应
	
	ID_C2S_REQUEST_COMMONBIND						= 0x0B90;		// 服务器->客户端 普通绑定的消息请求
	ID_S2C_RESPONSE_COMMONBIND						= 0x0B91;		// 服务器->客户端 普通绑定的消息回应
	
	ID_C2S_REQUEST_REMOTEMEND						= 0x0B92;		// 客服端 -> 服务器 远程修理
	ID_S2C_RESPONSE_REMOTEMEND						= 0x0B93;		// 服务器 -> 客户端 远程修理
	
	ID_C2S_REQUEST_STARTPROGRESS					= 0x0B94;		// 客服端 -> 服务器 读进度条的请求
	ID_S2C_RESPONSE_STARTPROGRESS					= 0x0B95;		// 服务器 -> 客户端 读进度条的回应

	ID_C2S_REQUEST_RESETTALENT						= 0x0B96;		// 客服端-> 服务器重置造化
	ID_S2C_RESPONSE_RESETTALENT						= 0x0B97;		// 服务器-> 客户端重置造化

	ID_C2S_REQUEST_GETACTVITYINFO					= 0x0B98;		// 客服端-> 服务器 获得活动信息
	ID_S2C_RESPONSE_GETACTVITYINFO					= 0x0B99;		// 服务器-> 客户端 活动信息 回应

	ID_C2S_REQUEST_CHANGEEQUIPPRO					= 0x0B9A;		// 客服端-> 服务器 改变装备属性
	ID_S2C_RESPONSE_CHANGEEQUIPPRO					= 0x0B9B;		// 服务器-> 客户端 改变装备属性
	
	ID_C2S_REQUEST_USEKITBAGNEEDLE					= 0x0B9C;		// 客服端->服务器 使用绣花针修理背包的消息请求
	ID_S2C_RESPONSE_USEKITBAGNEEDLE					= 0x0B9D;		// 服务器->客户端 使用绣花针修理背包的消息回应

	ID_C2S_REQUEST_OPENSTONE						= 0x0B9E;		// 客户端->服务器 开启灵石请求
	ID_S2C_RESPONSE_OPENSTONE						= 0x0B9F;		// 服务器->客户端 开启灵石回应
	
	ID_C2S_REQUEST_LOCKPASSWORD						= 0x0BA0;		// 客户端->服务器 安全保护密码重新锁定的请求
	ID_S2C_RESPONSE_LOCKPASSWORD					= 0x0BA1;		// 服务器->客户端 安全保护密码重新锁定的回应消息

	ID_C2S_REQUEST_LEARNOGRESKILL					= 0x0BA2;		// 客户端->服务器 学习妖魔志技能请求
	ID_S2C_RESPONSE_LEARNOGRESKILL					= 0x0BA3;		// 服务器->客户端 学习妖魔志技能回应

	ID_S2C_NOTIFY_PLAYERPICKITEM					= 0x0BA4;		// 服务器->客户端 玩家拾取物品通知

	ID_C2S_REQUEST_QUERYPEDAL						= 0x0BA5;		// 客户端->服务器 查询传送点请求
	ID_S2C_RESPONSE_QUERYPEDAL						= 0x0BA6;		// 服务器->客户端 查询传送点回应

	ID_C2S_REQUEST_AUTOBUYITEM						= 0x0BA7;		// 客户端->服务器 自动购物请求
	ID_S2C_RESPONSE_AUTOBUYITEM						= 0x0BA8;		// 服务器->客户端 自动购物回应

	ID_C2S_REQUEST_USESUMMONITEM					= 0x0BA9;		// 客户端->服务器 使用召集物品请求
	ID_S2C_RESPONSE_USESUMMONITEM					= 0x0BAA;		// 服务器->客户端 使用召集物品回应

	ID_C2S_REQUEST_SPLITEQUIP						= 0x0BAB;		// 客户端->服务器 拆分装备请求
	ID_S2C_RESPONSE_SPLITEQUIP						= 0x0BAC;		// 服务器->客户端 拆分装备回应

	ID_C2S_REQUEST_GETSTAR							= 0x0BAD;		// 客户端->服务器 摘星请求
	ID_S2C_RESPONSE_GETSTAR							= 0x0BAE;		// 服务器->客户端 摘星回应

	ID_C2S_REQUEST_ADDSTAR							= 0x0BB0;		// 客户端->服务器 加星请求
	ID_S2C_RESPONSE_ADDSTAR							= 0x0BB1;		// 服务器->客户端 加星回应

	ID_S2C_NOTIFY_EQUIPOVERDUE						= 0x0BB2;		// 服务器->客户端 装备过期通知
	ID_S2C_NOTIFY_STARTTIMER						= 0x0BB3;		// 服务器->客户端 开始过期计时通知

	ID_C2S_REQUEST_ADDEXTRA							= 0x0BB4;		// 客户端->服务器 加铭刻请求
	ID_S2C_RESPONSE_ADDEXTRA						= 0x0BB5;		// 服务器->客户端 加铭刻回应

	ID_S2C_NOTIFY_CHANGEOWNCAMP						= 0x0BB6;		// 服务器->客户端 改变所属阵营通知

	ID_C2S_REQUEST_WAROPERATE						= 0x0BB7;		// 客户端->服务器 国战操作请求
	ID_S2C_RESPONSE_WAROPERATE						= 0x0BB8;		// 服务器->客户端 国战操作回应
	
	ID_C2S_REQUEST_CANUSEAUTOCOLLECTITEM			= 0x0BB9;		// 客户端->服务器 是否可以使用采集道具的请求
	ID_S2C_RESPONSE_CANUSEAUTOCOLLECTITEM			= 0x0BBA;		// 服务器->客户端 是否可以使用采集道具的消息回应
	
	ID_C2S_REQUEST_COLLECTBYAUTOITEM				= 0x0BBB;		// 客户端->服务器 使用自动采集道具采集的消息请求
	ID_S2C_RESPONSE_COLLECTBYAUTOITEM				= 0x0BBC;		// 服务器->客户端 使用自动采集道具采集的消息回应

	ID_C2S_REQUEST_LEARNFANGCUNSKILL				= 0x0BBD;		// 客户端->服务器   学习方寸山技能请求
	ID_S2C_RESPONSE_LEARNFANGCUNSKILL				= 0x0BBE;		// 服务器->客户端	学习方寸山技能回应
	ID_S2C_NOTIFY_PLAYERTRANSFORM					= 0x0BC0;		// 服务器->客户端	玩家复制变身通知

	ID_C2S_REQUEST_HONORTOITEM						= 0x0BC1;		// 客户端->服务器 荣誉换物品请求
	ID_S2C_NOTIFY_USEBLACKPILLS						= 0x0BC2;		// 服务器->客户端 使用黑狗丸的通知
	
	ID_S2C_NOTIFY_EXPCONTAINER						= 0x0BC3;		// 服务器->客户端 修炼经验丹经验改变的通知
	ID_S2C_NOTICE_YUANBAOUPDATE						= 0x0BC4;		// 服务器->客户端 元宝变化通知
	ID_S2C_NOTICE_UPGANDJEW							= 0x0BC5;		// 服务器->客户端 装备升级和宝石合成通知
	ID_C2S_REQUEST_JEWELCARVE						= 0x0BC6;		// 客户端->服务器 宝石雕琢请求
	ID_S2C_RESPONSE_JEWELCARVE						= 0x0BC7;		// 服务器->客户端 宝石雕琢回应

	ID_C2S_REQUEST_QUERYBOSS						= 0x0BC8;		// 客户端->服务器 查询BOSS请求
	ID_S2C_RESPONSE_QUERYBOSS						= 0x0BC9;		// 服务器->客户端 查询BOSS回应

	//符文第二期
	ID_C2S_REQUEST_CHANGERUNE						= 0x0BCA;	    // 客户端->服务端  符文转换申请
	ID_S2C_RESPONSE_CHANGERUNE						= 0x0BCB;       // 服务端->客户端  符文转换回复
	ID_C2S_REQUEST_RUNECOM							= 0x0BCC;		// 客户端->服务端  符文升级申请
	ID_S2C_RESPONSE_RUNECOM							= 0x0BCD;		// 服务端->客户端  符文升级回复
	
	// 数字礼花
	ID_C2S_REQUEST_STARTDIGITALFIREWORKS			= 0x0BCE;		// 客户端->服务端  放数字礼花的消息请求
	ID_S2C_RESPONSE_STARTDIGITALFIREWORKS			= 0x0BCF;		// 服务端->客户端  放数字礼花的消息回应
	ID_S2C_NOTIFY_STARTDIGITALFIREWORKS				= 0x0BD0;		// 服务端->客户端  放数字礼花的消息通知

    // 变更用户时装显示状态
    ID_C2S_REQUEST_PLAYERCHANGEFASHIONSHOW          = 0x0BD1;        // 客户端->服务器 变更玩家时装显示状态请求
    ID_S2C_RESPONSE_PLAYERCHANGEFASHIONSHOW         = 0x0BD2;        // 服务器->客户端 变更玩家时装显示状态回应
    ID_S2C_NOTIFY_PLAYERCHANGEFASHIONSHOW           = 0x0BD3;        // 服务端->客户端 变更玩家时装显示状态通知

    // 变更用户装备是否可被察看
    ID_C2S_REQUEST_PLAYERCHANGEEQUIPSHOW            = 0x0BD4;        // 客户端->服务器 变更玩家装备察看状态请求
    ID_S2C_RESPONSE_PLAYERCHANGEEQUIPTSHOW          = 0x0BD5;        // 服务器->客户端 变更玩家装备察看状态回应

	// 分解法宝消息
	ID_C2S_REQUEST_DECOMPOSEMW						= 0x0BD6;		// 分解法宝请求
	ID_S2C_RESPONSE_DECOMPOSEMW						= 0x0BD7;		// 分解法宝回应

	// 法宝印记消息
	ID_C2S_REQUEST_MWQLPNT							= 0x0BD8;		
	ID_S2C_RESPONSE_MWQLPNT							= 0x0BD9;		

	// 法宝飞升消息
	ID_C2S_REQUEST_QUICKUP							= 0x0BDA;		
	ID_S2C_RESPONSE_QUICKUP							= 0x0BDB;		

	// 器灵合成消息
	ID_C2S_REQUEST_QLCOMPOSE						= 0x0BDC;		
	ID_S2C_RESPONSE_QLCOMPOSE						= 0x0BDD;		

	// 法宝获得经验通知
	ID_S2C_NOTIFY_MWOBTAINEXP						= 0x0BDE;		

	ID_C2S_QUERY_AROUND_PLAYER						= 0x0BDF;
	ID_S2C_QUERY_AROUND_PLAYER						= 0x0BE0;
	// 发送玩家的副本数据
	ID_S2C_PLAYER_REPETION_DATA						= 0x0BE1;
	// 同步玩家的声望
	ID_S2C_NOTIFY_SYNCFAME							= 0x0BE2;

	// 启灵请求
	ID_C2S_REQUEST_SPIRIT_OPEN						= 0x0BE3;
	// 启灵响应
	ID_S2C_RESPONSE_SPIRIT_OPEN						= 0x0BE4;
	// 铸灵请求
	ID_C2S_REQUEST_SPIRIT_CASTING					= 0x0BE5;
	// 铸灵响应
	ID_S2C_RESPONSE_SPIRIT_CASTING					= 0x0BE6;
	// 聚灵请求
	ID_C2S_REQUEST_SPIRIT_UPGRADE					= 0x0BE7;
	// 聚灵响应
	ID_S2C_RESPONSE_SPIRIT_UPGRADE					= 0x0BE8;
	// 退灵请求
	ID_C2S_REQUEST_SPIRIT_DOWNGRADE					= 0x0BE9;
	// 退灵响应
	ID_S2C_RESPONSE_SPIRIT_DOWNGRADE				= 0x0BEA;
	// 铸灵信息请求
	ID_C2S_REQUEST_SPIRIT_INFO						= 0x0BEB;
	// 铸灵信息通知
	ID_S2C_NOTIFY_SPIRIT_INFO						= 0x0BEC;

	// 服务器到客户端   通知背包锁定
	ID_S2C_NOTIFY_BAGGAGELOCKEN						= 0x0BED;

	// 客户端到服务器   背包锁定请求
	ID_C2S_NOTIFY_BAGGAGELOCKEN						= 0x0BEF;
};

// *************************************************************************** //
// CMessagePlayerChangeFashionShowRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePlayerChangeFashionShowRequest
{
    optional uint32 PlayerId           = 1; // 玩家编号
    optional uint32 FashionShowState   = 2; // 玩家时装状态， 0为显示时装，1为不显示时装
};

// *************************************************************************** //
// CMessagePlayerChangeFashionShowResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessagePlayerChangeFashionShowResponse
{
    optional uint32 Result              = 1; // 更新结果，0为成功，其它为失败
    optional uint32 PlayerId            = 2; // 玩家编号
    optional uint32 FashionShowState    = 3; // 玩家时装状态， 0为显示时装，1为不显示时装
};

// *************************************************************************** //
// CMessagePlayerChangeFashionNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessagePlayerChangeFashionNotice
{
    optional uint32 PlayerId           = 1; // 玩家编号
    optional uint32 FashionShowState   = 2; // 玩家时装状态，0为显示时装，1为不显示时装
};

// *************************************************************************** //
// CMessagePlayerChangeEquipShowRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePlayerChangeEquipShowRequest
{
    optional uint32 PlayerId            = 1; // 玩家编号
    optional uint32 EquipShowState      = 2; // 玩家装备显示状态，0为允许察看装备，1为不允许察看装备
};

// *************************************************************************** //
// CMessagePlayerChangeEquipShowResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessagePlayerChangeEquipShowResponse
{
    optional uint32 Result              = 1; // 更新结果，0为成功，其他为失败
    optional uint32 PlayerId            = 2; // 玩家编号
    optional uint32 EquipShowState      = 3; // 玩家当前装备显示状态， 0为不允许察看装备，1为允许察看装备
};




// *************************************************************************** //
// CMessageYourProDataNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageYourProDataNotice
{
	optional string CorpsName			 = 		1; 		// 军团名称	
	optional uint32 FamilyID			 = 		2; 		// 家族ID
	optional uint32 MapID				 = 		3; 
	optional uint32 LineID				 = 		4; 
	optional uint32 IsFamilyHeader		 = 		5; 		// 是否家族族长
	optional uint32 Sex					 = 		6; 
	optional uint32 Nationality			 = 		7; 		// 阵营ID
	optional uint32 Level				 = 		8; 
	optional uint32 AliveState			 = 		9; 
	optional uint32 MotionState			 = 		10; 
	optional uint32 MaxMP				 = 		11; 
	optional uint32 IfHasCorpsApply		 = 		12; 		// 是否有家族申请信息1表示是其他表示否 	
	optional string MapName				 = 		13; 
	optional uint32 CorpsID				 = 		14; 		// 军团ID
	optional uint32 MaxHP				 = 		15; 
	optional string CountryName			 = 		16; 
	optional uint32 HorseTempID			 = 		17; 		// 坐骑道具模板ID
	optional uint32 IsCorpsHeader		 = 		18; 		// 是否军团首领1表示是其他表示否
	optional PBProperty Property		 = 		19; 		// 属性集合
	optional uint32 EntityID			 = 		20; 
	optional uint64 Exp					 = 		21; 
	optional uint32 PkgCurrentNum		 = 		22; 		// 当前分包次数
	optional uint32 PkgTotal			 = 		23; 		// 总分包包数
	optional string FamilyName			 = 		24; 		// 家族名称
	optional uint32 CharID				 = 		25; 
	optional uint32 HorseState			 = 		26; 		// 坐骑状态(0:没坐骑 1:有坐骑)
	optional uint32 IfHasFamilyApply	 = 		27; 		// 是否有申请信息
	optional uint32 PosY				 = 		28; 
	optional uint32 PosX				 = 		29; 
	optional uint32 Direction			 = 		30; 
	optional uint32 Metier				 = 		31; 
	optional uint32	CampID				 =		32;			// PVP战场阵营信息
	optional uint32	FamilyHeaderID		 =		33;			// 自己家族首领的角色ID
	optional uint32	CorpsHeaderID		 =		34;			// 自己军团首领的角色ID
	optional uint32	MaxAP				 =		35;			// AP Max
	optional uint32	FamilyPost			 =		36;			// 家族职位
	optional uint32	CorpsPost			 =		37;			// 军团职位	
	optional uint32	FamilyMoney			 =		38;			// 家族金钱
	optional uint32	CorpsMoney			 =		39;			// 军团金钱
	optional uint32 ChestLevel			 = 		40; 		// 角色身穿衣服的等级
	optional uint32 WeaponLevel			 = 		41; 		// 角色手持武器的等级
	optional uint32	WeaponJewel			 =		42;			// 武器宝石数
	optional uint32	ChestJewel			 =		43;			// 防具宝石数
	optional uint32	UnionID				 =		44;
	optional string UnionName			 =		45;
	optional uint32	HeadCorpsID			 =		46;
	optional uint32 CountryID			 =		47;			// 国籍
	optional uint32 OfficialID			 =		48;			// 官职
	optional uint32 UserType			 =		49;			// 用户类型(GM)
	optional PBStateList StateList		 = 		50; 		// 状态列表
	optional PBSkillList EquipSkill		 =		51;			// 装备提供的技能
	optional int32 Money				 =		52;			// 金钱
	optional int32 BindMoney			 =		53;			// 绑定金
	optional uint32 KingID				 =		54;			// 国王ID
	optional uint32	PasswordStatus		 =		55;			// 安全密码状态
	optional uint32	ProtectTime			 =		56;			// 安全保护的到期时间	
	optional uint32	ProtectTimeLength	 =		57;			// 安全保护时间总长
	optional uint32	CharmValue			 =		58;			// 魅力值
	optional uint32	TodayLeftFlower		 =		59;			// 今天剩余使用鲜花次数
	optional string	SpouseName			 =		60;			// 配偶名称
	optional uint32	CurWarHonor			 =		61;			// 当前战功
	optional uint32	AllWarHonor			 =		62;			// 总计战功
	optional uint32	TodayWarHonor		 =		63;			// 今日战功
	optional uint32	OffLineTime			 =		64;			// 可以转换为经验的离线时间
	optional uint32	ExpHonor			 =		65;			// 已兑换经验荣誉
	optional uint32	ItemHonor			 =		66;			// 已兑换物品荣誉
    optional uint32 FashionShowState     =      67;         // 玩家时装显示状态
    optional uint32 EquipShowState       =      68;         // 玩家装备显示状态
	optional PBRepetDataList RepeDataToClientList	 =		69;         // 副本的数值，不同副本对应不一样。
	optional uint32	OffLineExp			=		70;			// 离线经验池
};

// *************************************************************************** //
// CMessageCreatePlayerProNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CPlayerProInfo
{
	optional uint32 FamilyID		 = 		1; 		// 家族ID
	optional uint32 CharID			 = 		2;        // 角色ID
	optional string Name			 = 		3; 
	optional uint32 CreatMethod		 = 		4; 		// 进入玩家视野的创建方式
	optional uint32 Direction		 = 		5; 
	optional PBTeamMsg Msg			 = 		6; 
	optional uint32 CurAP			 = 		7; 		// 角色当前活力( 百分比0-100 )
	optional int32	PKValue			 = 		8; 		// pk值
	optional uint32 MaxMP			 = 		9; 		// MP最大值
	optional uint32 HorseState		 = 		10; 		// 坐骑状态(0:没坐骑 1:有坐骑)
	optional uint32 Fashion			 = 		11; 		// 角色身穿时装的模版ID
	optional string FamilyName		 = 		12; 		// 家族名称	 
	optional uint32	HasRedStoneSta   = 		13; 		// 是否有内丹的噬炼状态 1表示有 0 表示没有
	optional uint32 CurMP			 = 		14; 		// 角色当前法力( 百分比0-100 )
	optional uint32 FaceType		 = 		15; 		// 玩家头像ID	
	optional uint32 EntityID		 = 		16; 		// 角色实体ID
	optional uint32 HorseTempID		 = 		17; 		// 坐骑道具模板ID
	optional uint32 Metier			 = 		18; 		// 角色职业
	optional string CorpsName		 = 		19; 		// 军团名称
	optional uint32	Sex				 = 		20; 		// 角色性别
	optional uint32 Nationality		 = 		21; 		// 国家ID
	optional uint32 MaxHP			 = 		22; 		// HP最大值
	optional uint32 TeamFlag		 = 		23; 		// 玩家队伍状态
	optional uint32 PosY			 = 		24; 
	optional uint32 MotionState		 = 		25; 
	optional PBStallInfo StallInfo   = 		26; 		// 摆摊数据
	optional uint32 Chest			 = 		27; 		// 角色身穿衣服的模版ID
	optional uint32 AliveState		 = 		28; 
	optional PBBuffList BuffList	 = 		29; 
	optional uint32 CurHP			 = 		30; 		// 角色当前生命( 百分比0-100 )
	optional uint32 PosX			 = 		31; 
	optional uint32 Weapon			 = 		32; 		// 角色手持武器的模版ID
	optional uint32 Speed			 = 		33; 
	optional uint32	CorpsID			 = 		34; 		// 军团ID
	optional PBStateList StateList	 = 		35; 		// 状态列表
	optional uint32 ChestLevel		 = 		36; 		// 角色身穿衣服的等级
	optional uint32 WeaponLevel		 = 		37; 		// 角色手持武器的等级
	optional uint32	YellowStatus	 = 		38; 		// 是否黄名
	optional uint32 Level			 = 		39; 		// 角色等级
	optional uint32	CurrentTitle	 =		40;			// 当前称号
	optional uint32	WeaponJewel		 =		41;			// 武器宝石数
	optional uint32	ChestJewel		 =		42;			// 防具宝石数
	optional uint32	MWID			 =		43;			// 法宝ID
	optional uint32 CampID			 =		44;			// 阵营ID
	optional uint32	MaxAP			 =		45;			// ap max			
	optional uint32	UserType		 =		46;			// 用户类型 见protype里的 enUserType枚举
	optional uint32 UnionID			 =		47;
	optional string UnionName		 =		48;
	optional uint32	CountryID		 =		49;			// 国籍
	optional uint32	OfficialID		 =		50;			// 官职
	optional uint32	VipFlag			 =		51;			// VIP 标志
	optional uint32	ProtectStatus	 =		52;			// 保护状态	
	optional string SpouseName		 =		53;			// 配偶名称
	optional uint32	HydrangeaStatus	 =		54;			// 绣球状态(Em_HydrangeaStatus)
	optional uint32	MarriageID		 =		55;			// 结婚装备的ID
    optional uint32 FashionShowState =      56;         // 玩家时装显示状态
    optional uint32 EquipShowState   =      57;         // 玩家装备显示状态
	optional uint32	MWQLID			 =		58;			// 法宝器灵ID
	optional string	SwornName		 =		59;			// 结拜名称
};

message CMessageCreatePlayerProNotice
{
	repeated CPlayerProInfo PlayerList	 = 		1; 	// 玩家
};

// *************************************************************************** //
// CMessageCreateNpcProNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CNpcProInfo
{
	optional uint32 Speed			 = 		1; 
	optional uint32 DieEntityID		 = 		2; 	// 怪物ID
	optional uint32 NpcID			 = 		3; 	// NpcID;
	optional uint32 NationNality	 = 		4; 	// 所属国家
	optional uint32 PedalType		 = 		5; 	// 陷阱类型(PEDAL_TELEPORT传送踏板 = 0, PEDAL_TRAP陷阱踏板 = 1, PEDAL_HIEROGRAM神符踏板 = 2)
	optional uint32 CurHP			 = 		6; 	// 角色当前生命( 百分比0-100 )
	optional uint32 SkillLevel		 = 		7; 	// 陷阱技能所带技能等级
	optional uint32 CreateType		 = 		8; 	// 建立的原因，0 代表正常建立，1 代表死亡掉落建立
	optional uint32 RedStoneStatus   = 		9; 	// 内丹状态 EM_RedStoneStatus
	optional uint32 CurMP			 = 		10; 	// 角色当前法力( 百分比0-100 )
	optional uint32 MaxHP			 = 		11; 	// HP最大值
	optional uint32 PetTmpLevel		 = 		12; 	// 召唤兽临时等级
	optional uint32 PosY			 = 		13; 
	optional uint32 SkillID			 = 		14; 	// 陷阱技能所带技能ID
	repeated uint32 Appertainings	 = 		15; 	// 归属(charid),列表为空表示所有人
	optional uint32 OwnerID			 = 		16; 
	optional uint32 Reserve			 = 		17; 
	optional uint32 PosX			 = 		18; 
	optional uint32 WaitTransTime	 = 		19; 	// 等待变形时间(来自npc存活刷新)
	optional uint32 NpcType			 = 		20; 	// NPC类型
	optional string OwnerName		 = 		21; 	// NPC所属的玩家名字 
	optional uint32 Direction		 = 		22; 
	optional uint32 Level			 = 		23; 	// 显示等级
	optional uint32 MaxMP			 = 		24; 	// MP最大值
	optional PBBuffList BuffList	 = 		25; 
	optional uint32 Money			 = 		26; 
	optional uint32 TaskTip			 = 		27; 	// 任务显示头顶
	optional uint32 CurAP			 = 		28; 	// 角色当前活力( 百分比0-100 )
	optional uint32 AliveTime		 = 		29; 	// 存活时间
	optional uint32 PKType			 = 		30; 	// PK类型
	optional uint32 AliveState		 = 		31; 
	optional uint32 PetAttackMode	 = 		32; 	// 召唤兽当前攻击模式
	optional uint32 EntityID		 = 		33; 	// 角色实体ID
	optional uint32 MotionState		 = 		34; 
	optional int32	TargetX			 =		35;		// 目标位置X
	optional int32	TargetY			 =		36;		// 目标位置Y
	optional string FamilyName		 =		37;		// 所属家族名称
	optional int32 CampID			 =		38;		// 阵营ID
	optional string	CorpsName		 =		39;		// 所属军团名称
	optional PBStateList StateList	 = 		40; 	// 状态列表
	optional string NewlyName		 =		41;		// 婚礼npc要显示的新人名称
	optional uint32	OwnCampID		 =		42;		// 所属阵营ID
};

message CMessageCreateNpcProNotice
{
	repeated CNpcProInfo NpcList	 = 		1; 	// npc 列表
	optional uint32 ListID			 = 		2; 
};

// *************************************************************************** //
// CMessageUseSkillRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUseSkillRequest
{
	optional uint32 PosX  = 		1; 
	repeated uint32 EntityIDList  = 2; 	// 群攻被作用实体列表
	optional uint32 Index  = 		3; 	// 技能索引 ( 在SkillList中的数组下标 )
	optional uint32 ListID  = 		4; 	// 客户端用于表示技能先后的序列号
	optional uint32 EntityID  = 	5;	// 目标实体
	optional uint32 PosY  = 		6; 
	optional uint32 SkillID  = 		7; 	// 技能ID
	optional uint32 IsMagicWeapon= 	8; 	// 是否法宝技能
	optional uint32 SrcEntityID	=	9; // 源实体(可以为召唤兽)
};

// *************************************************************************** //
// CMessageNormalAttackRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageNormalAttackRequest
{
	optional uint32 EntityID  = 		1; 
};

// *************************************************************************** //
// CMessagePickItemRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePickItemRequest
{
	optional uint32 EntityID  = 		1; 
};

// *************************************************************************** //
// 点击功能NPC请求
// *************************************************************************** //
message CMessageClickNpcRequest
{
	optional uint32 EntityID  = 		1; 
};

// *************************************************************************** //
// 点击功能NPC回应
// *************************************************************************** //
message CMessageClickNpcResponse
{
	optional uint32 EntityID  = 		1; 
};

// *************************************************************************** //
// 关闭对话通知
// *************************************************************************** //
message CMessageCloseDialogNotice
{
};

//// *************************************************************************** //
//// 点击NPC商店服务
//// *************************************************************************** //
//message CMessageClickShopServiceRequest
//{
//	optional uint32 EntityID = 1;
//};
//
//// *************************************************************************** //
//// 点击NPC商店服务
//// *************************************************************************** //
//message CMessageClickShopServiceResponse
//{
//	optional uint32 EntityID = 1;
//};

//// *************************************************************************** //
//// 点击NPC邮件服务
//// *************************************************************************** //
//message CMessageClickMailServiceRequest
//{
//	optional uint32 EntityID = 1;
//};
//
//// *************************************************************************** //
//// 点击NPC邮件服务
//// *************************************************************************** //
//message CMessageClickMailServiceResponse
//{
//	optional uint32 EntityID = 1;
//};

//// *************************************************************************** //
//// 点击NPC仓库服务
//// *************************************************************************** //
//message CMessageClickStorageServiceRequest
//{
//	optional uint32 EntityID = 1;
//};
//
//// *************************************************************************** //
//// 点击NPC仓库服务
//// *************************************************************************** //
//message CMessageClickStorageServiceResponse
//{
//	optional uint32 EntityID		= 1;
//	repeated PBItemObject PbItem	= 2; 
//};


//// *************************************************************************** //
//// 点击NPC升级服务
//// *************************************************************************** //
//message CMessageClickUpgServiceRequest
//{
//	optional uint32 EntityID = 1;
//};
//
//// *************************************************************************** //
//// 点击NPC升级服务
//// *************************************************************************** //
//message CMessageClickUpgServiceResponse
//{
//	optional uint32 EntityID = 1;
//};

//// *************************************************************************** //
//// 点击NPC传送服务
//// *************************************************************************** //
//message CMessageClickTeleServiceRequest
//{
//	optional uint32 EntityID = 1;
//};
//
//// *************************************************************************** //
//// 点击NPC传送服务
//// *************************************************************************** //
//message CMessageClickTeleServiceResponse
//{
//	optional uint32 EntityID = 1;
//};

// *************************************************************************** //
// 获得金钱通知
// *************************************************************************** //
message CMessageUpdateMoneyNotice
{
	optional uint32 Total  = 		1; 			// 总的钱数
	optional int32 Money  = 		2; 			// 改变的钱数
};

// *************************************************************************** //
// 获得绑定金钱通知
// *************************************************************************** //

message CMessageUpdateBindMoneyNotice
{
	optional uint32	Total  = 		1; 			// 总的绑定金钱数
	optional int32	Money  = 		2; 			// 改变的绑定金钱数
};

// *************************************************************************** //
// 购买道具请求
// *************************************************************************** //
message CMessagePurchaseItemRequest
{
	optional uint32 Number  = 		1; 			// 购买数量
	optional uint32 PageIndex  = 		2; 		// 道具出售表中的页索引
	optional uint32 EntityID  = 		3; 		// 服务NPC实体ID
	optional uint32 ItemIndex  = 		4; 		// 道具出售表中的道具索引
};

// *************************************************************************** //
// 出售道具请求
// *************************************************************************** //
message CMessageSellItemRequest
{
	optional uint32 EntityID  = 		1; 		// 服务NPC实体ID
	optional uint32 Number  = 		2; 			// 道具包裹中的道具数量
	optional uint32 ItemIndex  = 		3; 		// 道具包裹中的道具索引
};

//**************************************************************
// NPC 售出成功回复
//**************************************************************

message CMessageSellItemResponse
{
	optional PBItemObject Item  = 		1; 
};

// *************************************************************************** //
// 删除道具通知
// *************************************************************************** //
message CMessageRemoveItemNotify
{
	optional uint32 ItemIndex  = 		1; 		// 道具包裹中的道具索引
	optional uint32 Pos  = 		2; 
	optional uint32 Number  = 		3; 		// 道具包裹中的道具数量
};


// *************************************************************************** //
// 删除任务道具通知
// *************************************************************************** //
message CMessageRemoveTaskItemNotice
{
	optional uint32 Number  = 		1; 		// 任务包裹中的道具数量
	optional uint32 ItemIndex  = 		2; 		// 任务包裹中的道具索引
};


// *************************************************************************** //
// 丢弃道具请求
// *************************************************************************** //
message CMessageThrowItemRequest
{
	optional uint32	Number     = 		1; 	    // 道具包裹中的道具数量
	optional uint32	ItemIndex  = 		2; 		// 道具包裹中的道具索引
};

// *************************************************************************** //
// 丢弃道具通知
// *************************************************************************** //
message CMessageThrowItemNotice
{
	optional uint32	ItemIndex  = 		1; 		// 道具包裹中的道具索引
	optional uint32	Number  = 		2; 		// 道具包裹中的道具数量
};

// *************************************************************************** //
// 丢弃金币请求
// *************************************************************************** //
message CMessageThrowMoneyRequest
{
	optional uint32	Number  = 		1; 		// 丢弃的金币数量
};

// *************************************************************************** //
// 修改快捷栏请求
// 增加了表示栏位类型和栏位索引的字段
// *************************************************************************** //

message CMessageSetShortcutRequest
{
	optional uint32 Type  = 		1; 
	optional uint32 LabelType   = 		2; 
	optional uint32 LabelIndex  = 		3;  
	optional uint32 Index  = 		4; 
	optional uint32 Shortcut  = 		5; 
	optional uint32 Param1	= 6;
};

// *************************************************************************** //
// CMessageEntityRelifeNotice 游戏服务器  -> 游戏客户端
// *************************************************************************** //
message CMessageEntityRelifeNotice
{
	optional uint32 EntityID  = 		1; 
	optional uint32 MaxHP  = 		2; 
	optional uint32 CurMP  = 		3; 
	optional uint32 PosX  = 		4; 
	optional uint32 CurHP  = 		5; 
	optional uint32 MaxMP  = 		6; 
	optional uint32 PosY  = 		7; 
};

// *************************************************************************** //
// CMessageEntityDisappearNotice 游戏服务器  -> 游戏客户端
// *************************************************************************** //
message CMessageEntityDisappearNotice
{
	optional uint32 EntityID  = 		1; 
};

// *************************************************************************** //
// CMessagePlayerReliveRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //

message CMessagePlayerReliveRequest
{
	optional uint32 ReliveType  = 		1; 		// 复活类型
};

// *************************************************************************** //
// CMessagePlayerReliveRequest 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessagePlayerReliveNotice
{
	optional uint32 PosY  = 		1; 			// 复活的横坐标
	optional uint32 MaxMP  = 		2; 			// 复活后的MP最大值
	optional uint32 PosX  = 		3; 			// 复活的横坐标
	optional uint32 MaxHP  = 		4; 			// 复活后的HP最大值
	optional uint32 CurHP  = 		5; 			// 复活后的HP当前值
	optional uint32 CurMP  = 		6; 			// 复活后的MP当前值
	optional uint32 EntityID  = 		7; 		// 复活实体ID
};

// *************************************************************************** //
// CMessageRemoveBuffNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageRemoveBuffNotice
{
	optional uint32 EntityID  = 		1; 		// 实体ID
	optional uint32 BuffID  = 		2; 		// BUFFID
};

// *************************************************************************** //
// CMessageChangeStateNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //

message CMessageChangeStateNotice
{
	optional uint32 State  = 		1; 
	optional bool InState  = 		2; 
	optional uint32 Parame1  = 		3; 
	optional uint32 ListID  = 		4; 
	optional uint32 Parame2  = 		5; 
	optional uint32 EntityID  = 		6; 
};

// *************************************************************************** //
// 功能结果, 技能,道具和Buff造成的结果都是功能组成的
// *************************************************************************** //
message PBFuncResult
{
	optional uint32 PosX  = 		1; 
	optional uint32 Func  = 		2; 
	optional uint32 EntityID  = 		3; 	// 目标实体ID
	optional uint32 Direction  = 		4; 
	optional uint32 PosY  = 		5; 
	optional uint32 Value1  = 		6; 
	optional uint32 Value2  = 		7; 
	optional uint32 Value5  = 		8; 
	optional uint32 Value3  = 		9; 
	optional uint32 Value4  = 		10; 
	optional uint32 Result  = 		11; 	// 执行结果和功能，高16位代表结果，低16位代表功能
	optional uint32 ListID  = 		12; 
	optional uint32 SrcEntityID  = 		13; 
};

// *************************************************************************** //
// CMessageUseSkillNotice 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUseSkillNotice
{
	repeated PBFuncResult FuncResult  = 		1; 
	optional uint32 SrcY  = 		2; 
	optional uint32 SrcEntityID  = 		3; 		// 源实体ID
	optional uint32 ResultNumber  = 		4; 		// 结果数量
	optional uint32 DesX  = 		5; 
	optional uint32 SkillID  = 		6; 			// 技能ID
	optional uint32 SrcX  = 		7; 
	optional uint32 DesY  = 		8; 
	optional uint32 SkillLevel  = 		9; 		// 技能等级
	optional uint32 DesEntityID  = 		10; 		// 目标实体ID
	optional uint32 ListID  = 		11; 				// 客户端用于表示技能先后的序列号
};

// *************************************************************************** //
// CMessageFuncResultNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageFuncResultNotice
{
	optional uint32 ResultNumber  = 		1; 		// 结果数量
	repeated PBFuncResult FuncResult  = 		2; 
};

// *************************************************************************** //
// CMessageRemoveBuffNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageChangeMetierNotice
{
	optional uint32 MetierID  = 		1; 		// 职业ID
	optional uint32 EntityID  = 		2; 		// 实体ID
};

// *************************************************************************** //
// CMessageLockItemNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageLockItemNotice
{
	optional uint32 Index  = 		1; 			// 被锁定的索引
	optional uint32 Locked  = 		2; 		// 锁类型
};


// *************************************************************************** //
// CMessageObtainExpNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageObtainExpNotice
{
	optional uint32 ExpDelta  = 		1; 	// 升级获得的经验
	optional uint32 ListID  = 		2; 		// 技能序列号
	optional uint32 LeveUpFlag  = 		3; 	// 因为获得经验而升级标志( 1 升级, 0 不升级 )
	optional uint32 Exp  = 		4; 		// 升级当前的经验
};

// *************************************************************************** //
// CMessageMoveItemRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageMoveItemRequest
{
	optional uint32 Number  = 		1; 
	optional uint32 DesIndex  = 	2; 
	optional uint32 SrcIndex  = 	3; 
	optional uint32 SrcSlot  = 		4; 
	optional uint32 DesSlot  = 		5; 
	optional uint32	NpcID	 =		6;			// 如果移动涉及仓库的话，NpcID表示仓库服务npc的实体ID
};

// *************************************************************************** //
// CMessageMoveItemNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageMoveItemNotice
{
	optional uint32 DesIndex  = 		1; 
	optional uint32 SrcSlot   = 		2; 
	optional uint32 SrcIndex  = 		3; 
	optional uint32 Number    = 		4; 
	optional uint32 DesSlot   = 		5; 
	optional uint32	Errcode	  =			6;		// 错误码	
	optional uint32	SrcNum	  =			7;		// 源格子物品最终数量	
	optional uint32	DesNum	  =			8;		// 目标格子物品最终数量
};

// *************************************************************************** //
// CMessageInsertEquipNotify 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageInsertEquipNotify
{
	optional PBItemObject ItemObjet  = 		1; 
	optional uint32 Pos  = 		2; 
};

// *************************************************************************** //
// CMessageInsertItemNotify 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageInsertItemNotify
{
	repeated uint32 Index  = 		1; 
	repeated uint32 Number  = 		2; 
	optional uint32 ItemID  = 		3; 
	optional uint32 BindState  = 		4;  // 绑定状态
	optional uint32 Pos  = 		5; 			
	optional uint32 Yuan  = 		6; 	   // 如果是元气虫则为元气值	
};

// *************************************************************************** //
// CMessageProDebugNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageProDebugNotice
{
	optional uint32 Command  = 		1; 			// 调试命令
	repeated uint32 Parameter  = 		2; 	// 命令参数
	optional uint32 ParameterNumber  = 		3; 
};

// *************************************************************************** //
// CMessageProErrorNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageProErrorNotice
{
	optional int32 ErrorCode  = 		1; 		// 错误码
};

// *************************************************************************** //
// CMessageEquipItemRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageEquipItemRequest
{
	optional uint32 SrcIndex  = 		1; 			// 装备的索引
	optional uint32 DesIndex  = 		2; 			// 目的的索引
};

// *************************************************************************** //
// CMessageEquipItemRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUnEquipItemRequest
{
	optional uint32 Index  = 		1; 			// 装备的索引
	optional uint32 Part  = 		2; 			// 卸装的部位
};

// *************************************************************************** //
// CMessageEquipItemNotice 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageEquipItemNotice
{
	optional uint32 SrcIndex  = 		1; 			// 包裹的索引
	optional uint32 DesIndex  = 		2; 			// 目的的索引
	optional uint32	Errcode	  =			3;			// 错误码	 	
};

// *************************************************************************** //
// CMessageUnEquipItemNotice 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUnEquipItemNotice
{
	optional uint32 Part	= 		1; 			// 装备的部位
	optional uint32 Index	= 		2; 			// 装备的索引
	optional uint32	Errcode	=		3;			// 错误码
};

// *************************************************************************** //
// CMessagePlayerEquipNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessagePlayerChangeEquipNotice
{
	optional uint32 MaxHP  = 		1; 			// 最大血
	optional uint32 CurMP  = 		2; 			// 当前蓝
	optional uint32 EntityID  = 		3; 		// 装备的玩家实体ID
	optional uint32 Part  = 		4; 			// 装备的部位
	optional uint32 MaxMP  = 		5; 			// 最大蓝
	optional uint32 ItemID  = 		6; 			// 装备的道具ID
	optional uint32 CurHP  = 		7; 			// 当前血
	optional uint32	WeaponJewel		 =		8;			// 武器宝石数
	optional uint32	ChestJewel		 =		9;			// 防具宝石数
	optional uint32	WeaponLevel		 =		10;			// 武器等级
	optional uint32	ChestLevel		 =		11;			// 防具等级
	optional uint32	MWID			 =		12;			// 法宝ID
	optional uint32	FashionID		 =		13;			// 时装ID
	optional uint32	MarriageID		 =		14;			// 结婚装备ID
	optional uint32	MWQLID			 =		15;			// 法宝器灵ID
};

// *************************************************************************** //
// CMessageEntityRefreshNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageEntityRefreshNotice
{
	optional uint32 MaxMP  = 		1; 			// 玩家MP最大值
	optional uint32 CurMP  = 		2; 			// 玩家的当前蓝
	optional uint32 CurHP  = 		3; 			// 玩家的当前血
	optional uint32 MaxHP  = 		4; 			// 玩家HP最大值
	optional uint32 EntityID  = 		5; 		// 玩家实体ID
};

// *************************************************************************** //
// CMessageClickSkillPointRequest 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageClickSkillPointRequest
{
	optional uint32 Row  = 		1; 			// 技能树的纵坐标
	optional uint32 Col  = 		2; 			// 技能树的横坐标
	optional uint32 GenerID  = 		3; 		// 技能树的系别ID
};

// *************************************************************************** //
// CMessageClickSkillPointResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageClickSkillPointResponse
{
	optional uint32 Col  = 		1; 			// 技能树的横坐标
	optional uint32 Row  = 		2; 			// 技能树的纵坐标
	optional uint32 GenerID  = 		3; 		// 技能树的系别模版ID
};



// *************************************************************************** //
// CMessageResetAPPRequest 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageResetAPPRequest
{
};


// *************************************************************************** //
// CMessageResetAPPResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageResetAPPResponse
{
	optional uint32 Ret  = 		1; 	// 错误码
};

// *************************************************************************** //
// CMessageResetSSPRequest 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageResetSSPRequest
{
	optional uint32 Col  = 		1; 
	optional uint32 Row  = 		2; 
	optional uint32 GenerID  = 		3; 
};

// *************************************************************************** //
// CMessageResetASPRequest 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageResetASPRequest
{
};

// *************************************************************************** //
// CMessageResetSSPResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageResetSSPResponse
{
	optional uint32 Row  = 		1; 
	optional uint32 GenerID  = 		2; 
	optional uint32 Col  = 		3; 
};

// *************************************************************************** //
// CMessageResetASPResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageResetASPResponse
{
	optional uint32 Ret  = 		1; 	// 错误码
};

// *************************************************************************** //
// CMessageEntityDieNotice 游戏客户端 -> 游戏服务器 
// *************************************************************************** //
message CMessageEntityDieNotice
{
	optional uint32 EntityID  = 		1; 
};

// *************************************************************************** //
// GetRolePersonalInfoByID 游戏客户端 -> 游戏服务器 
// 根据玩家ID获取玩家资料
// *************************************************************************** //
message GetRolePersonalInfoByIDMsg
{
	optional uint32 EntityID  = 		1; 
	optional uint32 SessionID  = 		2; 
};

// *************************************************************************** //
// GetRolePersonalInfoByName 游戏客户端 -> 游戏服务器 
// 根据玩家角色名获取玩家资料
// *************************************************************************** //
message GetRolePersonalInfoByNameMsg
{
	optional string Name  = 		1; 
	optional uint32 SessionID  = 		2; 
};


//本类封装了角色的个人资料
message PBRolePersonalInfo
{
	optional string RealMetier  = 		1;  //真实职业
	optional string CharName  = 		2;  //角色名
	optional string Space  = 		3;  //个性空间
	optional uint32 Age  = 		4; 		//年龄 1-254
	optional string LoveNote  = 		5;  //我的爱情观
	optional uint32 Cons  = 		6;  //星座ID 1-12
	optional uint32 Head  = 		7;  //头像ID，0开始
	optional string Title  = 		8;  //称谓
	optional uint32 PlayerID  = 		9;  //玩家ID
	optional uint32 Sex  = 		10; 		//性别 
};

// *************************************************************************** //
// SendRolePersonalInfo 游戏服务器 -> 游戏客户端 
// 服务器传回玩家资料
// *************************************************************************** //
message SendRolePersonalInfoMsg
{
	optional PBRolePersonalInfo Info  = 		1; 
	optional uint32 SessionID  = 		2; 
};

// *************************************************************************** //
// GetRolePersonalInfo 游戏客户端 -> 游戏服务器 
// 修改玩家资料
// *************************************************************************** //
message SetRolePersonalInfoMsg
{
	optional PBRolePersonalInfo Info  = 		1; 
	optional uint32 SessionID  = 		2; 
};

// *************************************************************************** //
// RolePersonalInfoBackMsg 游戏服务器 -> 游戏客户端 
// 玩家资料相关操作的
// *************************************************************************** //
message RolePersonalInfoBackMsg
{
	enum CHANGE_INFO
	{
		SET_OK = 0; //修改玩家资料成功
		SET_FAIL = 1; //失败
	};
	optional uint32 Code = 1;
	optional uint32 SessionID = 2;
};


// *************************************************************************** //
// CMessageUseItemRequest 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageUseItemRequest
{
	optional uint32 Pos    = 		1; 			// 位置的种类
	optional uint32 Index  = 		2; 			// 包裹的索引
	optional uint32 Param  = 		3; 			// 参数1( 使用传送符物品时，传送方式选择 )
};

// *************************************************************************** //
// CMessageUseItemResponse 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUseItemResponse
{
	optional uint32 Index  = 		1; 		// 包裹的索引
	optional uint32 Param1  = 		2; 		// 如果是人民币药，表示剩余的HP; 如果是次数型传送符，表示已使用次数
	optional uint32 Pos  = 		3; 		// 位置的种类
	optional uint32 Result  = 		4; 		// 返回的结果
	optional uint32 Param2  = 		5; 		// 如果是人民币药，表示剩余的MP
	optional uint32 ItemID  = 		6; 		// 使用物品的ID	
	optional uint32	Param3	=		7;		// 剩余得AP
};

// *************************************************************************** //
// CMessagePlayerUseItemNotice 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePlayerUseItemNotice
{
	// 实体ID
	// 道具ID
	optional uint32 CurHP  = 		1; 			// 当前血
	optional uint32 CurMP  = 		2; 			// 当前蓝
	optional uint32 ItemID  = 		3; 
	optional uint32 EntityID  = 		4; 
};


// *************************************************************************** //
// CMessageTelePortResponse 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageTelePortResponse
{
	// 传送的位置
	optional uint32 PosX  = 		1; 
	optional uint32 Index  = 		2; 			// 包裹的索引
	optional uint32 PosY  = 		3; 
};

// *************************************************************************** //
// CMessageProduceEquipRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageProduceEquipRequest
{
	optional uint32 Type2  = 		1; 		// 生产类型细化，武器为武器类型，防具为防具位置，药品为药品类型
	optional uint32 Type1  = 		2; 		// 生产的类型,0武器 1防具 
	optional uint32 Type3  = 		3; 		// 生产类型细化，防具为防具效果	
	optional uint32 Level  = 		4; 		// 生产道具的等级
	optional uint32 Number  = 		5; 	// 需要生产的数量
	optional uint32 Rank  = 		6; 		// 所用材料的级别
	optional uint32 Bind =			7;	
};

// *************************************************************************** //
// CMessageUpgAddItemRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUpgAddItemRequest
{
	optional uint32 Index  = 		1; 		// 包里的位置
};

// *************************************************************************** //
// CMessageUpgAddItemResponse 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUpgAddItemResponse
{
	optional uint32 Index1  = 		1; 		// 包里的位置
	optional uint32 Index2  = 		2; 		// 升级面板里的位置
};

// *************************************************************************** //
// CMessageUpgDelItemRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUpgDelItemRequest
{
	optional uint32 Index  = 		1; 		// 升级面板里的位置
};

// *************************************************************************** //
// CMessageUpgDelItemResponse 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUpgDelItemResponse
{
	optional uint32 Index1  = 		1; 		// 包里的位置
	optional uint32 Index2  = 		2; 		// 升级面板里的位置
};

// *************************************************************************** //
// CMessageUpgEquipRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUpgEquipRequest
{
	optional uint32 NpcEntityID  = 		1;   // 玩家正在操作的绑定该服务的NPC实体ID
	optional uint32 Type		 = 		2;	 // 升级装备的类型	
	optional uint32 EquipIndex	 =		3;	 // 装备的位置
	optional uint32	ItemIndex	 =		4;	 // 精炼石的位置
	repeated uint32 LuckIndex	 =		5;	 // 符的位置
};

// *************************************************************************** //
// CMessageUpgEquipNotice 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUpgEquipNotice
{
	optional uint32 Type  = 		1; 			// 升级类型
	optional uint32 Level  = 		2; 			// 当前装备等级
	optional uint32 Result  = 		3; 			// 升级结果 0成功1保底2消失
	optional uint32 EqpIdx =		4;			// 装备在包裹里的索引
};

// *************************************************************************** //
// CMessageProducePhysRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageProducePhysRequest
{
	optional uint32 Type  = 		1; 		// 生产的类型 
	optional uint32 Level  = 		2; 		// 生产的等级
	optional uint32 Number  = 		3; 		// 需要生产的数量
};

// *************************************************************************** //
// CMessageProFinNotice 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageProFinNotice
{
	optional uint32 Result   = 		1; 
	optional uint32 LeftNum  = 		2; 	
};

// *************************************************************************** //
// CMessageProBreakNotice 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageProBreakNotice
{
	optional uint32 Result  = 		1; 
};


// 往面板上放装备或符文的请求/回应
// *************************************************************************** //
// CMessagePushRuneRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePushRuneRequest
{
	optional uint32 Index  = 		1; 					// 所放装备或符文在包中的位置
};

// *************************************************************************** //
// CMessagePushRuneResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessagePushRuneResponse
{
	optional uint32 Index1  = 		1; 					// 所放装备或符文在包中的位置
	optional uint32 Index2  = 		2; 					// 在面板里的位置
};


// 从面板上拿下装备/符文的请求/回应
// *************************************************************************** //
// CMessagePopRuneRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePopRuneRequest
{
	optional uint32 Index  = 		1; 					// 拿下的位置
};

// *************************************************************************** //
// CMessagePopRuneResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessagePopRuneResponse
{
	optional uint32 Index2  = 		1; 					// 拿下的位置
	optional uint32 Index1  = 		2; 					// 所放装备或符文在包里的位置
};


// 镶嵌符文的请求/回应
// *************************************************************************** //
// CMessageInsertRuneRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageInsertRuneRequest
{
	optional uint32	EquipIndex	=	1;			// 装备的位置
	optional uint32	RuneIndex	=	2;			// 符文的位置
};

// *************************************************************************** //
// CMessageInsertRuneNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageInsertRuneNotice
{
	optional uint32 Result  = 		1; 		// 返回结果，0成功
};


// 往镶嵌宝石面板上放装备/宝石的请求/回应
// *************************************************************************** //
// CMessageInsertPushJewelRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageInsertPushJewelRequest
{
	optional uint32 DesIndex  = 		1; 					// 放置的位置
	optional uint32 SrcIndex  = 		2; 					// 所放装备或宝石在包中的位置
};

// *************************************************************************** //
// CMessageInsertPushJewelResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageInsertPushJewelResponse
{
	optional uint32 Index1  = 		1; 					// 所放装备或宝石在包中的位置
	optional uint32 Index2  = 		2; 					// 面板的位置
};


// 从镶嵌宝石面板上拿下装备或宝石的请求/回应
// *************************************************************************** //
// CMessageInsertPopJewelRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageInsertPopJewelRequest
{
	optional uint32	Index  = 		1; 					// 拿下的位置
};

// *************************************************************************** //
// CMessageInsertPopJewelResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageInsertPopJewelResponse
{
	optional uint32	Index2  = 		1; 					// 拿下的位置
	optional uint32 Index1  = 		2; 					// 所放装备或珠宝在包里的位置
};


// 镶嵌宝石的请求/回应
// *************************************************************************** //
// CMessageInsertJewelRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageInsertJewelRequest
{
	optional uint32 EquipIndex	=	1;
	repeated uint32 JewelIndex	=	2;
};

// *************************************************************************** //
// CMessageInsertJewelNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageInsertJewelNotice
{
	optional uint32 Bind    = 		1; 		// 装备是否被绑定
	optional uint32	Result  = 		2; 		// 镶嵌结果，0成功
};


// 往元气虫吃装备界面上放装备的请求/回应
// *************************************************************************** //
// CMessageEatPushEquipRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageEatPushEquipRequest
{
	optional uint32	Index  = 		1; 					// 所放装备在包中的位置
};

// *************************************************************************** //
// CMessageEatPushEquipResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageEatPushEquipResponse
{
	optional uint32 Index2  = 		1; 					// 面板的位置
	optional uint32	Index1  = 		2; 					// 所放装备在包中的位置
};


// 从元气虫吃装备界面上拿下装备的请求/回应
// *************************************************************************** //
// CMessageEatPopEquipRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageEatPopEquipRequest
{
	optional uint32	Index  = 		1; 					// 拿下的位置
};

// *************************************************************************** //
// CMessageEatPopEquipResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //

message CMessageEatPopEquipResponse
{
	optional uint32	Index2  = 		1; 					// 拿下的位置
	optional uint32 Index1  = 		2; 					// 所放装备在包里的位置
};

// 吃装备的请求/回应
// *************************************************************************** //
// CMessageEatEquipRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageEatEquipRequest
{
	optional uint32 BugIndex	= 		1; 				// 元气虫在包里的位置
	repeated uint32 EquipIndex	=		2;				// 要吃的装备在包里的位置
};

// *************************************************************************** //
// CMessageEatEquipNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageEatEquipNotice
{
	optional uint32 Number  = 		1; 		// 吃掉后的点数
	optional uint32 Index  = 		2; 			// 元气虫在包里的位置
};

// 合成放材料/元气蛋的请求/回应
// *************************************************************************** //
// CMessageComposePushRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageComposePushRequest
{
	optional uint32	Index  = 		1; 					// 合成道具在包中的位置
};

// 游戏客户端 -> 游戏服务器  发送栏位锁定请求
message CMessageBaggageLockedRequest
{	
	optional uint32 BagType			=		1;			//  背包的类型
	optional uint32 KitBagIndex		=		2;			//  背包的工具栏索引
	optional uint32 Locked			=		3;			//  背包是否锁定

};

// 服务器->客户端		回应锁定结果     
message CMessageBaggageLockedResponse
{	
	optional uint32 BagType			=		1;			//  背包的类型
	optional uint32 KitBagIndex		=		2;			//  背包的工具栏索引
	optional uint32 Locked			=		3;			//  背包是否锁定

};

// *************************************************************************** //
// CMessageComposePushResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageComposePushResponse
{
	optional uint32 Index2  = 		1; 					// 面板的位置
	optional uint32	Index1  = 		2; 					// 合成道具在包中的位置
};

// 合成拿下材料/元气蛋的请求/回应
// *************************************************************************** //
// CMessageComposePopRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageComposePopRequest
{
};

// *************************************************************************** //
// CMessageComposePopResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageComposePopResponse
{
	optional uint32 Index  = 		1; 					// 合成道具在包里的位置
};

// 合成的请求/回应
// *************************************************************************** //
// CMessageComposeRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageComposeRequest
{
	optional uint32 Index  = 		1; 					// 材料的位置
};

// *************************************************************************** //
// CMessageComposeNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageComposeNotice
{
	optional uint32 Number2  = 		1; 					// 剩余的数量
	optional uint32 Index  = 		2; 						// 合成道具在包里的位置
	optional uint32 Number1  = 		3; 					// 生成的数量
	optional uint32 ItemID  = 		4; 					// 生成的物品
};


// 开槽放装备/元气蛋的请求/回应
// *************************************************************************** //
// CMessageOpenPushRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageOpenPushRequest
{
	optional uint32	Index  = 		1; 					// 开槽道具在包中的位置
};

// *************************************************************************** //
// CMessageOpenPushResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageOpenPushResponse
{
	optional uint32 Index2  = 		1; 					// 面板的位置
	optional uint32	Index1  = 		2; 					// 开槽道具在包中的位置
};

// 开槽拿下材料/元气蛋的请求/回应
// *************************************************************************** //
// CMessageOpenPopRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageOpenPopRequest
{
	optional uint32	Index  = 		1; 					// 拿下的位置
};

// *************************************************************************** //
// CMessageOpenPopResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageOpenPopResponse
{
	optional uint32	Index2  = 		1; 					// 拿下的位置
	optional uint32 Index1  = 		2; 					// 开槽道具在包里的位置
};

// 开槽的请求/回应
// *************************************************************************** //
// CMessageOpenSlotRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageOpenSlotRequest
{
	optional uint32	EquipIndex	=	1;				// 装备的位置
	optional uint32 EggIndex	=	2;				// 元气蛋的位置
};

// *************************************************************************** //
// CMessageOpenSlotNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageOpenSlotNotice
{
	optional uint32 Index1  = 		1; 					// 开槽装备在包里的位置
	optional uint32 Index2  = 		2; 					// 元气蛋在包里的位置
	optional uint32 Number  = 		3; 					// 剩余的元气蛋数量
};


// 传送的请求/回应
// *************************************************************************** //
// CMessagePlayerTeleRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessagePlayerTeleRequest
{
	optional uint32 NpcEntityID  = 		1;	// 玩家正在操作的绑定该服务的NPC实体ID
	optional uint32 Order		 = 		2;	// 选择的地点在列表中的位置
	optional uint32	Index		 = 		3;	// 传送需求物品在包裹中的物品
	optional uint32 PedalEntityID =		4;	// 传送踏板的实体ID
	optional uint32 AsynTeleTokenID =	5;	// 异步传送ID
	optional uint32 IsStatic	 =		6;	// 是否是在阻挡中传送
	optional uint32	MarriageItem =		7;	// 结婚道具传送(道具索引)
};

// *************************************************************************** //
// CMessagePlayerTeleResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessagePlayerTeleResponse
{
	optional int32	ErrCode	=		1;	// 错误码	
	optional uint32 Order  = 		2; 	// 选择的地点在列表中的位置
};

// *************************************************************************** //
// CMessagePlayerTeleNotify 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessagePlayerTeleNotify
{
	optional uint32 SrcCharID		= 1;	// 召集传送发起者ID
	optional string SrcCharName		= 2;	// 召集传送发起者名字
	optional uint32 TeleType		= 3;	// 召集类型（待定）
	optional uint32 Timeout			= 4;	// 召集令倒计时（过时无效)
	optional uint32 AsynTeleTokenID = 5;	// 异步传送ID
	optional uint32 LineID			= 6;	// 目的线
	optional uint32 MapID			= 7;	// 目的地图
	optional uint32 PosX			= 8;	// 目的x
	optional uint32 PosY			= 9;	// 目的y
};


message CMessageRandMoveDirectCallBack
{
	optional uint32 Direction  = 		1; 
	optional uint32 EntityID  = 		2; 
};

message CMessageProDebugCommand
{
	optional string DebugCommond  = 		1; 
};

message CMessageNotifyDirection
{
	optional uint32 Direction  = 		1; 
};

message CMessageNotifyDirectionCallBack
{
	optional uint32 EntityID  = 		1; 
	optional uint32 Direction  = 		2; 
};

message CMessageChangePetModeRequest
{
	optional uint32 ModeType  = 		1; 
};

message CMessageChangePetModeResponse
{
	optional uint32 Result  = 		1; 
};

message CMessagePetLevelUpNotice
{
	optional uint32 EntityID  = 		1; 
	optional uint32 Level  = 		2; 
};
// *************************************************************************** //
// CMessageNotifyEqptDurability 游戏服务器 -> 游戏客户端
// 更新装备耐久度的通知
// *************************************************************************** //
message CMessageNotifyEqptDurability
{
	optional uint32 IsInPackage     = 		1;       // 是否在包裹中（1表示在包裹中，0表示装备在身上）
	repeated uint32 EqptDurability  = 		2;       // 装备耐久度	
	repeated uint32 EqptIndex       = 		3;       // 装备索引
};

// 当距离不够时与客户端同步坐标
// *************************************************************************** //
// CMessageSyncPosNotify 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageSyncPosNotify
{
	optional uint32 PosX  = 		1; 				// 实体的X坐标
	optional uint32 EntityID  = 		2; 			// 需要同步的实体ID
	optional uint32 PosY  = 		3; 				// 实体的Y坐标
};
// *************************************************************************** //
// CMessageMendEquipment  游戏客户端->游戏服务器
// 修理装备的请求
// *************************************************************************** //
message CMessageMendEquipment 
{
	optional uint32 Index            = 		1;      // 装备索引 
	optional uint32 IsInPackage      = 		2;      // 是否在包裹中（1表示在包裹中，0表示装备在身上）
	optional uint32 IsAllEquipment   = 		3;      // 是否修理所有装备
	optional uint32	EntityID		 =		4;		// npc的实体ID
};
// *************************************************************************** //
// CMessageMendEquipment 游戏服务器->游戏客户端  
// 修理装备完成的通知
// *************************************************************************** //
message CMessaegMendEquipmentCallBack
{
	optional uint32  MendResult     = 		1;       // 是否修理成功	
};


// 有速度改变时通知其他玩家
// *************************************************************************** //
// CMessageSpeedChangedNotify 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageSpeedChangedNotify
{
	optional uint32 EntityID  = 		1; 			// 速度改变的实体ID
	optional uint32 Speed  = 		2; 				// 实体的新速度
};

// 通知玩家生产经验等级的改变
// *************************************************************************** //
// CMessageProExpNotify 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageProExpNotify
{
	optional uint32 ProExp  = 		1; 				// 新的生产经验
	optional uint32 ProLevel  = 		2; 			// 新的生产等级
};

// 请求获得玩家售出给NPC的道具信息
//****************************************************************************//
// CMessageObtainSaleItem	游戏客户端 -> 游戏服务器
//****************************************************************************//

message CMessageObtainSaleItem
{
	optional uint32 EntityID   = 		1; 			// 进行NPC交易的实体ID
};

// 请求获得玩家售出给NPC的道具信息
//****************************************************************************//
// CMessageObtainSaleItemCallBack	游戏服务器 -> 游戏客户端
//****************************************************************************//

message CMessageObtainSaleItemCallBack
{
	repeated PBItemObject ItemList  = 		1; 		// 道具列表
	optional uint32 Amount  = 		2; 				// 道具数量
};

// 购回售出给NPC的商品
//****************************************************************************//
// CMessageBuyBackItem		游戏客户端 -> 游戏服务器
//****************************************************************************//

message CMessageBuyBackItem
{
	optional uint32 SaleIndex  = 		1; 			// 购回界面的格子索引
}

message CMessageBuyBackItemCallBack
{
	optional uint32 SaleIndex	 = 		1; 
	optional uint32 Number		 = 		2; 		// 道具数量
	repeated PBItemObject ItemList  = 		3; 		// 道具列表
};

message CMessageMoveStorageItem 
{
	optional uint32 DesIndex  = 		1;            // 目标位置格子索引
	optional uint32 SrcKind   = 		2;            // 原位置： 0表示在包裹里面 1 表示在仓库里面 
	optional uint32 DesKind   = 		3;            // 目标位置：0表示在包裹里面 1 表示在仓库里面
	optional uint32 SrcIndex  = 		4;            // 原位置格子索引
	optional uint32 ItemNum   = 		5;            // 移动的物品数量
}

// 关闭指定的窗口
//****************************************************************************//
// CMessageCloseWindowReuest		游戏客户端 -> 游戏服务器
//****************************************************************************//

message CMessageCloseWindowRequest
{
	optional uint32 CloseType  = 		1; 				// 关闭窗口的类别
}

//****************************************************************************//
// CMessageCloseWindowResponse		游戏服务器 -> 游戏客户端
//****************************************************************************//

message CMessageCloseWindowResponse
{
	optional uint32 Result  = 		1; 				// 关闭窗口的类别
	optional uint32 CloseType  = 		2; 				// 关闭窗口的类别
}

//****************************************************************************//
// CMessageBindItemResponse		游戏服务器 -> 游戏客户端
// 绑定物品完毕
//****************************************************************************//
message CMessageErrorActOnBindItemNotify
{
	optional uint32 ItemPos    = 		1;     // 物品位置
	optional int32 ErrorCode  = 		2;     // 错误码
	optional uint32 ItemIndex  = 		3;     // 物品索引
}
//****************************************************************************//
// CMessageAddBindItem		游戏客户端->游戏服务器
// 添加绑定物品
//****************************************************************************//
message CMessageAddBindItemRequest
{
	optional uint32  ItemIndex  = 		1;                 // 位置索引  
};
//****************************************************************************//
// CMessageAddBindItemResponse		 游戏服务器 -> 游戏客户端
// 添加绑定物品的返回消息
//****************************************************************************//
message CMessageAddBindItemResponse
{
	optional uint32 InputPos   = 		1; 			     // 放置位置0表示武器位置1表示绑定石位置
	optional uint32 ItemIndex  = 		2;                // 位置索引
	optional uint32 Result     = 		3; 				 // 操作结果1表示成功0表示失败
};

//****************************************************************************//
// CMessageTakeBindItemRequest		 游戏服务器 -> 游戏客户端
// 取下放在绑定界面上的物品请求
//****************************************************************************//
message CMessageTakeBindItemRequest
{
	optional uint32 ItemIndex  = 		1;           // 位置索引0表示武器位置1表示绑定石位置	
};

//****************************************************************************//
// CMessageTakeBindItemResponse		 游戏服务器 -> 游戏客户端
// 取下放在绑定界面上的物品回应
//****************************************************************************//
message CMessageTakeBindItemResponse
{
	optional uint32 ItemIndex  = 		1;     // 位置索引	
	optional uint32 InputPos   = 		2; 			     // 放置位置0表示武器位置1表示绑定石位置	
};

//****************************************************************************//
// CMessageBindItemRequest	 游戏客户端	 ->  游戏服务器
// 绑定物品的请求
//****************************************************************************//
message CMessageBindItemRequest
{
	optional uint32 CLientCode  = 		1; 
	optional uint32	ItemIndex	=		2;
	optional uint32	BindIndex	=		3;
};
//****************************************************************************//
// CMessageBindItemResponse		游戏服务器 -> 游戏客户端
// 绑定物品完毕
//****************************************************************************//
message CMessageBindItemResponse
{
	optional uint32 BindResult  = 		1; 
	optional uint32 BindStatus  = 		2; 
	optional uint32 ItemIndex   = 		3; 
};

//****************************************************************************//
// CMessageFreeItem		游戏客户端->游戏服务器
// 添加解除绑定物品的请求
//****************************************************************************//
message CMessageAddItemForFreeRequest
{
	optional uint32 ItemIndex  = 		1;    // 位置索引
};

//****************************************************************************//
// CMessageFreeItem		游戏客户端->游戏服务器
// 添加解除绑定物品的请求
//****************************************************************************//
message CMessageAddItemForFreeResponse
{
	optional uint32 InputPos   = 		1;     // 放置位置0表示武器位置1表示绑定石位置	
	optional uint32 ItemIndex  = 		2;     // 位置索引	
};

//****************************************************************************//
// CMessageTakeItemForFreeRequest		游戏客户端->游戏服务器
// 卸下待解除绑定物品的请求
//****************************************************************************//
message CMessageTakeItemForFreeRequest
{
	optional uint32 ItemIndex  = 		1; 
};

//****************************************************************************//
// CMessageTakeItemForFreeResponse		游戏客户端->游戏服务器
// 卸下待解除绑定物品的请求
//****************************************************************************//
message CMessageTakeItemForFreeResponse
{
	optional uint32 ItemIndex  = 		1;     // 位置索引	
	optional uint32 InputPos   = 		2; 	 // 放置位置0表示武器位置1表示绑定石位置	
};

//*********************************************************************

//****************************************************************************//
// CMessageFreeItem		游戏客户端->游戏服务器
// 解除物品绑定的请求
//****************************************************************************//
message CMessageFreeItem 
{
	optional uint32 CLientCode     = 		1;
	optional uint32 ItemIndex	   =		2;	
};
//****************************************************************************//
// CMessageTakeBindItemResponse		 游戏服务器 -> 游戏客户端
// 取下放在绑定界面上的物品回应
//****************************************************************************//
message CMessageFreeItemResponse
{
	optional uint32 ItemIndex       = 		1;     // 位置索引	
	optional uint32 Result          = 		2;     // 操作结果1表示成功0表示失败
	optional uint32 InputPos        = 		3; 	  // 放置位置0表示武器位置1表示绑定石位置	
	optional uint32 BindFreeTime	   = 		4;     // 绑定解除时间
};

//****************************************************************************//
// CMessageRideHorseNotice			 游戏服务器 -> 游戏客户端
// 骑马或下马通知
//****************************************************************************//
message CMessageRideHorseNotice
{
	optional uint32 EntityID       = 		1;     // 实体ID	
	optional uint32 Speed          = 		2;     // 实体当前速度
	optional uint32 HorseResult	   = 		3;     // 坐骑结果(0为下马,1为上马)
	optional uint32 HorseTempID    = 		4; 	  // 坐骑道具模板ID
};

// *************************************************************************** //
// CMessageUseTaskItemRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUseTaskItemRequest
{
	optional uint32 Index  = 		1; 			// 包裹的索引
};


// *************************************************************************** //
// CMessageUseTaskItemNotice 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageUseTaskItemNotice
{
	optional uint32 Index  = 		1; 			// 包裹的索引
};

// *************************************************************************** //
// CMessageStudySkillRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageStudySkillRequest
{
	repeated CMessageClickSkillPointRequest StudySkillList  = 		1; 			// 学习技能列表
};

message CMessageSkillTreeResponse
{
	optional uint32								Index			 = 		1; 
	optional uint32								Result			 = 		2; 
	optional uint32								GenerID			 = 		3; 		// 技能树的系别模版ID
	optional uint32								Row				 = 		4; 			// 技能树的纵坐标
	optional uint32								Col				 = 		5; 			// 技能树的横坐标
	optional uint32								SkillID			 = 		6; 
};

// *************************************************************************** //
// CMessageStudySkillResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageStudySkillResponse
{
	//repeated CMessageClickSkillPointRequest StudySkillList		= 1;		// 学习技能列表
	//repeated uint32							ResultList			= 2;		// 学习技能结果列表
	repeated CMessageSkillTreeResponse		StudySkillList		 = 		1; 
	optional uint32							MaxHP				 = 		2; 		// 最大HP
	optional uint32							CurHP				 = 		3; 		// 当前HP
	optional uint32							RemainSkillPoint	 = 		4; 		// 剩余技能点
	optional uint32							MaxMP				 = 		5; 		// 最大MP
	optional uint32							CurMoney			 = 		6; 		// 当前金币数
	optional uint32							CurMP				 = 		7; 		// 当前MP
};

// *************************************************************************** //
// CMessageAddPropertyPointRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageAddPropertyPointRequest
{
	optional uint32 AddSTR  = 		1; 		// 力量加点
	optional uint32 AddSPR  = 		2; 		// 精神加点
	optional uint32 AddWis  = 		3; 		// 智力加点
	optional uint32 AddCON  = 		4; 		// 体质加点
	optional uint32 AddSTA  = 		5; 		// 耐力加点
};

// *************************************************************************** //
// CMessageAddPropertyPointResponse 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageAddPropertyPointResponse
{
	optional uint32 Result			 = 		1; 		// 加点结果
};


// *************************************************************************** //
// 设定回城		客户端->服务器
// *************************************************************************** //
message CMessageSetTownRequest
{
	optional uint32 NpcEntityID  = 		1;   // 玩家正在操作的绑定该服务的NPC实体ID
};

// *************************************************************************** //
// 设定回城		服务器->客户端
// *************************************************************************** //
message CMessageSetTownResponse
{

};

// *************************************************************************** //
// 切换PK模式		客户端->服务器
// *************************************************************************** //
message CMessageChangePKModeRequest
{
	optional uint32 PKMode  = 		1; 		// 改变后的PK模式
};

// *************************************************************************** //
// 切换PK模式		服务器->客户端
// *************************************************************************** //
message CMessageChangePKModeResponse
{
	optional uint32 PKMode  = 		1; 		// 当前的PK模式
	optional uint32 Result  = 		2; 		// 切换的结果( 根据错误码 )
};

// *************************************************************************** //
// 观察玩家装备的请求		客户端->服务器
// *************************************************************************** //
message CMessageLookPropertyRequest
{
	optional uint32 CharID		 = 		1; 		// 被观察玩家的CharID
	optional uint32 LookType	 =		2;		// 观察类型
};

// *************************************************************************** //
// 观察玩家装备的请求		服务器->客户端
// *************************************************************************** //
message CMessageLookPropertyResponse
{
	optional string			Family						 = 		1; 		// 家族
	optional uint32			Sex							 = 		2; 		// 性别
	optional uint32			PKValue						 = 		3; 		// PK值
	optional uint32			MetierID					 = 		4; 		// 职业
	optional PBItemBox		Equipment					 = 		5; 		// 玩家装备
	optional uint32			CharID						 = 		6; 		// 被观察者的CHARID
	optional string			Name						 = 		7; 		// 被观察者名字
	optional uint32			FaceType					 = 		8; 		// 头像ID
	optional string			IndividualityIdiograph		 = 		9; 		// 个性签名
	optional uint32			Level						 = 		10; 		// 等级
	optional string			Title						 = 		11; 		// 称号
	optional uint32			NationID					 = 		12; 		// 国家
	optional string			Corps						 = 		13; 		// 军团
	optional uint32			Honor						 = 		14; 		// 功勋
	optional uint32			KillNum						 = 		15; 		// 杀敌数
	optional string			Mate						 = 		16; 		// 配偶
	optional PBWizard		Wizard						 =		17;		// 才华
	optional uint32			LookType					 =		18;		// 观察类型
	optional uint32			VipFlag						 =		19;		// vip 标志
	optional uint32			CountryID					 =		20;		// 国籍
	optional uint32			Result						 =		21;		// 查询结果	
};

// *************************************************************************** //
// 观察玩家装备的请求		sceneserver->router->sceneserver
// *************************************************************************** //
message CMessageLookPropertyRouterRequest
{
	optional CMessageRouter		Router			 = 		1; 
	optional uint32				LookType		 =		2;
};

// *************************************************************************** //
// 观察玩家装备的请求		sceneserver->router->sceneserver
// *************************************************************************** //
message CMessageLookPropertyRouterResponse
{
	optional CMessageRouter						Router				 = 		1; 
	optional CMessageLookPropertyResponse		EquipmentInfo		 = 		2; 
};


// *************************************************************************** //
// CMessageFuncResultCallBack 游戏服务器 -> 游戏客户端
// *************************************************************************** //
message CMessageFuncResultCallBack
{
	optional uint32 EntityID  = 		1; 
	optional uint32 ResultNumber  = 		2; 		// 结果数量
	repeated PBFuncResult FuncResult  = 		3; 
	optional uint32	PosY  = 		4; 
	optional uint32	PosX  = 		5; 
};
// *************************************************************************** //
// CMessageThrowStorItemRequest   游戏服务器->游戏客户端
// *************************************************************************** //
message CMessageThrowStorItemRequest
{
	optional uint32 Num    = 		1; 	
	optional uint32 Index  = 		2; 
};
// *************************************************************************** //
// CMessageThrowStorItemRequest   游戏客户端->游戏服务器
// *************************************************************************** //
message CMessageThrowStorItemResponse
{
	optional uint32 Index  = 		1; 
	optional uint32 Num    = 		2; 		
};
// *************************************************************************** //
// CMessageThrowStorItemRequest   游戏服务器->游戏客户端
// *************************************************************************** //
message CMessageNotifyItemBinded
{
	optional uint32 ItemPos    = 		1; 		 //  物品的放置位置ESlotType
	optional uint32 ItemIndex  = 		2;        //  物品在包裹里面的索引
	optional uint32	BindStatus =		3;
};

// *************************************************************************** //
// CMessageNotifyStorItem   游戏服务器->游戏客户端
// *************************************************************************** //
message CMessageNotifyStorItem
{			
	optional PBItemBox		Storage			= 1; 			// 玩家仓库	
	optional uint32			StorMoney		= 2;			// 存储在仓库的非绑定金钱
	optional uint32			StorBindMoney	= 3;			// 存储在仓库里面的绑定金钱	
};

// *************************************************************************** //
// CMessageTaskCollectRequest   游戏客户端->游戏服务器
// *************************************************************************** //
message CMessageTaskCollectRequest
{
	optional uint32 EntityID  = 		1; 
};

// *************************************************************************** //
// CMessageChangeHonorNotify   游戏服务器->游戏客户端
// *************************************************************************** //
message CMessageChangeHonorNotify
{
	optional uint32 KillNum			 = 		1; 	   //  杀敌数	
	optional int32 ChangeHonor	  	 = 		2;        //  功勋的改变值
	optional uint32 TotalHonor       = 		3; 	   //  功勋的当前值
};

// *************************************************************************** //
// CMessageHonorToExpRequest   游戏客户端->游戏服务器
// *************************************************************************** //
message CMessageHonorToExpRequest
{
	optional int32 ChangeHonor		 = 		1; 		// 兑换的功勋值
};
// *************************************************************************** //
// CMessageChangeHelpStaus   游戏客户端->游戏服务器
// *************************************************************************** //
message CMessageChangeHelpStaus
{
	optional uint32 Status  = 		1; 	
	optional uint32 Index   = 		2; 
};

// *************************************************************************** //
// CMessageObtainDiploidTimeRequest   游戏客户端->游戏服务器
// *************************************************************************** //

message CMessageObtainDiploidTimeRequest
{
	optional uint32	Index  = 		1; 			// 领取方式序号 
	optional uint32 NpcEntityID  = 		2;   // 玩家正在操作的绑定该服务的NPC实体ID
};

//******************************************************
// CMessageObtainDiploidResponse
//******************************************************

message CMessageObtainDiploidResponse
{
	optional uint32	Time	 = 		1; 	// 成功:双倍时间累计;失败:本周还剩的双倍(单位:毫秒)
	optional uint32	RtnCode  = 		2; 	// 领取成功(true)?失败(false)
};

//******************************************************
// CMessageFreezeDiploidRequest
//******************************************************

message CMessageFreezeDiploidRequest
{
	optional uint32 NpcEntityID  = 		1;     // 玩家正在操作的绑定该服务的NPC实体ID
	optional uint32 Type  = 		2; 			// 冻结: true; 解冻: false
};

//******************************************************
// CMessageFreezeDiploidResponse
//******************************************************

message CMessageFreezeDiploidResponse
{
	optional uint32 Time  = 		1; 			// 时间(单位:毫秒)
	optional uint32 Type  = 		2; 			// 冻结: true; 解冻: false
};

//******************************************************
// CMessageQueryRemainDiploid
//******************************************************

message CMessageQueryRemainDiploid
{
	optional uint32 NpcEntityID  = 		1;     // 玩家正在操作的绑定该服务的NPC实体ID
};

//******************************************************
// CMessageRemainDiploidNotice
//******************************************************

message CMessageRemainDiploidNotice
{
	optional uint32 Time  = 		1; 		// 剩余时间 (单位:毫秒)
};

message CMessageActivityDiploidNotice
{
	optional uint32	Type = 1;	// true:开启;false:关闭
	optional uint32 BeginTime = 2;	// 时*60+分钟(如： 9：30为9*60+30),分钟
	optional uint32	EndTime = 3;	// 持续时间，分钟
};
//// *************************************************************************** //
//// CMessageClickBindSeviceRequest   游戏客户端->游戏服务器
//// *************************************************************************** //
//message CMessageClickBindSeviceRequest
//{
//	optional uint32 EntityID = 1;
//};
//// *************************************************************************** //
//// CMessageClickBindServiceResponse   游戏服务器->游戏客户端
//// *************************************************************************** //
//message CMessageClickBindServiceResponse
//{
//	optional uint32 EntityID = 1;
//};

// *************************************************************************** //
// CMessageOpenServiceRequest   游戏客户端->游戏服务器
// *************************************************************************** //
message CMessageOpenServiceRequest
{
	optional uint32 ServiceID	 = 		1;  // 服务名
	optional uint32 EntityID	 = 		2;  // 如果是点击NPC，这里填npc id.
}

// *************************************************************************** //
// CMessageClickBindServiceResponse   游戏服务器->游戏客户端
// *************************************************************************** //
message CMessageOpenServiceResponse
{
	optional uint32 ServiceID	 = 		1;  // 服务名
	optional int32	ErrorCode	 = 		2;  // 错误码； 0 成功
}

// *************************************************************************** //
// CMessageAddEqtForResumeBindReq   游戏客户端->游戏服务器
// 放置需要取消解除手动绑定的物品的请求
// *************************************************************************** //
message CMessageAddEqtForResumeBindRequest
{
	optional uint32 EqtIndex  = 		1; 
};
// *************************************************************************** //
// CMessageAddEqtForResumeBindResponse   游戏服务器->游戏客户端
// 放置需要取消解除手动绑定的物品的回应
// *************************************************************************** //
message CMessageAddEqtForResumeBindResponse
{
	optional uint32 EqtIndex  = 		1; 
}; 

// *************************************************************************** //
// CMessageTakeEqtForResumeBindRequest   游戏客户端->游戏服务器
// 取下需要取消解除手动绑定的物品的请求
// *************************************************************************** //
message CMessageTakeEqtForResumeBindRequest
{
	optional uint32 EqtIndex  = 		1; 
};

// *************************************************************************** //
// CMessageTakeEqtForResumeBindResponse   游戏客户端->游戏服务器
// 取下需要取消解除手动绑定的物品的回应
// *************************************************************************** //
message CMessageTakeEqtForResumeBindResponse
{
	optional uint32 EqtIndex  = 		1; 	
};

// *************************************************************************** //
// CMessageResumeBindRequest   游戏客户端->游戏服务器
// 取消解除手动绑定的请求
// *************************************************************************** //
message CMessageResumeBindRequest
{
	optional uint32 EqtIndex  = 		1; 
};

// *************************************************************************** //
// CMessageResumeBindResponse   游戏客户端->游戏服务器
// 取消解除手动绑定的回应
// *************************************************************************** //
message CMessageResumeBindResponse   
{
	optional uint32 EqtIndex  = 		1; 
};

// *************************************************************************** //
// CMessageServerRefreshNotify   游戏服务器->游戏客户端
// *************************************************************************** //
message CMessageServerRefreshNotify   
{
};

// *************************************************************************** //
// CMessageUseContinuedSkillRequest 游戏客户端 -> 游戏服务器
// *************************************************************************** //
message CMessageUseContinuedSkillRequest
{
	optional uint32 SkillID  = 		1; 		// 技能ID
	optional uint32 PosY  = 		2; 			// 技能中心点Y坐标
	optional uint32 Index  = 		3; 			// 技能索引 ( 在SkillList中的数组下标 )
	optional uint32 EntityID  = 		4; 		// 技能使用者ID
	optional uint32 ListID  = 		5; 			// 客户端用于表示技能先后的序列号
	repeated uint32 EntityIDList  = 		6; 	// 群攻被作用实体列表
	optional uint32 PosX  = 		7; 			// 技能中心点X坐标
};

// *************************************************************************** //
// CMessageUseContinuedSkillNotify 游戏服务器  -> 游戏客户端
// *************************************************************************** //
message CMessageUseContinuedSkillNotify
{
	optional uint32 SkillID  = 		1; 		// 技能ID
	optional uint32 PosY  = 		2; 			// 技能中心点Y坐标
	optional uint32 PosX  = 		3; 			// 技能中心点X坐标
	optional uint32 EntityID  = 		4; 		// 技能使用者ID
};

// *************************************************************************** //
// CMessageClickRedStoneRequest    游戏客户端->游戏服务器
// 使用内丹的请求
// *************************************************************************** //
message CMessageClickRedStoneRequest   
{
	optional uint32   RedStoneID  = 		1; 		// 内丹的实体ID
};

// *************************************************************************** //
// CMessageClickRedStoneResponse   游戏服务器->游戏客户端
// 使用内丹的回应
// *************************************************************************** //
message CMessageClickRedStoneResponse   
{
	optional uint32	  Result	  = 		1; 		// 使用结果：1表示可以使用，0 表示不能使用
	optional uint32   RedStoneID  = 		2; 		// 内丹的实体ID
};

// *************************************************************************** //
// CMessageStopRedStoneTimer   游戏服务器->游戏客户端
// 停止内丹读秒
// *************************************************************************** //
message CMessageStopRedStoneTimer
{
	optional uint32   RedStoneID  = 		1; 		// 内丹的实体ID	
};


// *************************************************************************** //
// CMessageRedStoneTimerEnd   游戏客户端->游戏服务器
// 内丹读秒结束
// *************************************************************************** //
message CMessageRedStoneTimerEnd
{
	optional uint32   RedStoneID  = 		1; 		// 内丹的实体ID
};

// *************************************************************************** //
// CMessageExpMagicEnd   游戏服务器->游戏客户端
// 噬炼结束
// *************************************************************************** //
message CMessageExpMagicEnd
{
	optional uint32   RedStoneID  = 		1; 		// 内丹的实体ID
};

// *************************************************************************** //
// CMessageChangeRestoneStaus   游戏服务器->游戏客户端
// 或取内丹经验的通知，通知加载特效
// *************************************************************************** //
message CMessageChangeRestoneStaus
{
	optional uint32   Status		 = 		1; 		// 1表示玩家处于噬炼状态，0表示没有噬炼状态
	optional uint32   RedstoneTmpID	 = 		2; 		// 内丹的模板ID
	optional uint32	  EntityID		 = 		3; 		// 实体ID
};


// *************************************************************************** //
// CMessageClickWelfareRequest   游戏客户端->游戏服务器
// 点击每日问答的服务请求
// *************************************************************************** //
message CMessageClickWelfareRequest
{
	optional uint32   EntityID  = 		1; 		// npc的实体id
};

// *************************************************************************** //
// CMessageClickWelfareResponse  游戏服务器->游戏客户端
// 点击每日问答的服务的回应
// *************************************************************************** //
message CMessageClickWelfareResponse
{
	optional uint32   Result    = 		1; 		// 结果	1 表示可以答题 0已经答过题 ，其他表示不正确的npc状态  
	optional uint32   EntityID  = 		2; 		// npc的实体id
};


// *************************************************************************** //
// CMessageChangeCampRequest  游戏服务器->游戏客户端
// 改变阵营请求
// *************************************************************************** //
message CMessageChangeCampRequest
{
	optional uint32		CampID  = 		1; 
};

message CMessageChangeCampResponse
{
	optional uint32		CampID  = 		1; 
};


message PBPropertyValue
{
	optional uint32		InitValue	 = 		1; 
	optional uint32		DeltaValue	 = 		2; 
};

message PBInformation
{
	optional PBPropertyValue		PDefence	  = 		1; 
	optional uint32					RemainPoint	  = 		2;  // 剩余潜能点
	repeated uint32					SkillIndex	  = 		3; 
	optional PBPropertyValue		NormalDuck	  = 		4; 
	optional uint32					AddFixDamage  = 		5; 
	optional uint32					DecPerPDamage = 		6; 
	optional uint32					DecFixMDamage = 		7; 
	optional PBPropertyValue		IgnoreDuck	  = 		8; 
	repeated uint32					ResDef		  = 		9; 
	optional uint32					NormalHitRate = 		10; 
	optional uint32					SkillHitRate  = 		11; 
	optional PBPropertyValue		Con			  = 		12; 
	optional uint32					AttackType	  = 		13; 
	optional uint32					StealHP		  = 		14; 
	repeated uint32					ResAtt		  = 		15; 
	optional uint32					DecPerMDamage = 		16; 
	optional PBPropertyValue		DeathValue	  = 		17; 
	optional PBPropertyValue		Wis			  = 		18; 
	optional PBPropertyValue		Str			  = 		19; 
	optional PBPropertyValue		MaxMP		  = 		20; 
	optional uint32					DecFixPDamage = 		21; 
	optional uint32					StealMP		  = 		22; 
	optional PBPropertyValue		Speed		  = 		23; 
	repeated uint32					IncLevel	  = 		24; 	
	optional PBPropertyValue		Sta			  = 		25; 
	optional PBPropertyValue		Spr			  = 		26; 
	optional PBPropertyValue		DeathRate	  = 		27; 
	optional PBPropertyValue		SkillDuck	  = 		28; 
	optional uint32					AddPerDamage  = 		29; 
	optional uint32					ReduceLevel	  = 		30; 
	repeated uint32					ReduceCool	  = 		31;
	optional PBPropertyValue		MDefence	  = 		32; 
	optional PBPropertyValue		DeathDefense  = 		33; 
	optional PBPropertyValue		MaxAP		  = 		34; 
	optional PBPropertyValue		MaxHP		  = 		35; 
	optional uint32					RemainSkillPoint  = 	36;  // 剩余技能点（只有人才有）
	optional PBPropertyValue		PAttackMax	  =			37;
	optional PBPropertyValue		PAttackMin	  =			38;
	optional PBPropertyValue		MAttackMax	  =			39;
	optional PBPropertyValue		MAttackMin	  =			40;
	repeated uint32					EquipWizard	  =			41;
};

// *************************************************************************** //
// CMessageRefreshPropertyNotify   游戏服务器->游戏客户端
// *************************************************************************** //
message CMessageRefreshPropertyNotify   
{
	optional PBInformation			Information	  = 		1; 	
	optional uint32					EntityID  = 		2; 
	optional uint32					CurHP  = 		3; 
	optional uint32					CurMP  = 		4; 
	optional uint32                 Level  = 		5; 
	optional uint32					CurAP  =		6;
};

// *************************************************************************** //
// CMessageChangePKTypeNotify   游戏服务器->游戏客户端
// *************************************************************************** //
message CMessageChangePKTypeNotify   
{
	optional uint32		EntityID  = 		1; 
	optional uint32		PKType	  = 		2; 
};

// *************************************************************************** //
// CMessageRedstoneStarted   游戏服务器->游戏客户端
// 内丹被成功开启
// *************************************************************************** //
message CMessageRedstoneStarted  
{
	optional string Name  = 		1;     // 玩家名字	  
};

// *************************************************************************** //
// CMessageActiveRemoveBuffRequest   游戏客户端->游戏服务器
// *************************************************************************** //
message CMessageActiveRemoveBuffRequest
{
	optional uint32 BuffID  = 		1;     // 需要手动清除的BUFFID	
	repeated uint32 ListID	=		2;
};
// *************************************************************************** //
// CMessageBeginRedstoneMagicNotify   游戏服务器->游戏客户端
// 内丹开始噬炼的通知
// *************************************************************************** //
message CMessageBeginRedstoneMagicNotify
{
	optional uint32 EntityID  = 		1;     // 内丹的实体ID	  
};

// *************************************************************************** //
// CMessageListIbItemsRequest 商城物品列表请求 
// *************************************************************************** //
/*
message CMessageListIbItemsRequest
{
};

message CMessageListIbItemsResponse
{
	repeated PBIbItemOnSale ibitems  = 		1;         // 商城物品列表
	repeated PBIbMoney Money  = 		2;               // 其帐号上的金钱
};

message PBLocation
{
	optional uint32 page  = 		1;                    // 页面
	optional uint32 row  = 		2;                     // 行号
	optional uint32 col  = 		3;                     // 列号
};

message PBIbItem2Client
{
	optional uint32 ActiveFlag  = 		1;              // 物品是否生效
	optional uint32 id  = 		2;                      // 物品ID
	optional uint32 PriceUnit  = 		3;               // 售价币种
	optional uint32 PriceValue  = 		4;              // 物品的单价
	optional PBLocation location  = 		5;            // 物品的位置
};

message CMessageListIbItemsResponse
{
	optional int32 CurrentMoney  = 		1;             // 玩家当前的钱数
	repeated PBIbItem2Client ibitems  = 		2;        // 商城物品列表
};

// *************************************************************************** //
// CMessagePurchaseIbItemRequest
// *************************************************************************** //
message CMessagePurchaseIbItemRequest
{
	optional uint32 IbItemID  = 		1;        //  欲购买的物品ID
	optional uint32 IbItemCount  = 		2;     //  欲购买的物品数量
};

// *************************************************************************** //
// CMessagePurchaseIbItemResponse
// *************************************************************************** //
//
message CMessagePurchaseIbItemResponse
{
	optional int32 error  = 		1;              // 错误代码, 0 成功, 负数 失败.
	repeated PBIbMoney money  = 		2;          // 交易后剩余的金钱
};

message CMessagePurchaseIbItemResponse
{
	optional int32 RemainedMoney  = 		1;      // 交易后剩余的金钱
	optional int32 error  = 		2;              // 错误代码, 0 成功, 负数 失败.
};
*/

// *************************************************************************** //
// CMessageChangeEntityPKTypeNotify   游戏服务器->游戏客户端
// 实体PK类型改变通知
// *************************************************************************** //
message CMessageChangeEntityPKTypeNotify
{
	optional uint32 EntityID  = 		1;     // 实体ID	  
	optional uint32 NewType   = 		2; 	 // 新PK类型
};


//**********************************
// CMessageBrushOgreNotify 刷怪通知
//**********************************

message CMessageBrushOgreNotify
{
	optional uint32	Code  = 		1; 	// 信息Key
};



//**********************************
// CMessageSetPassWordRequest 
// 设置密码的请求
//**********************************
message CMessageSetPasswordRequest
{
	optional string PassWord  = 		1; 		// 2 级密码	
}	

//**********************************
// CMessageSetPassWordRequest 
// 设置密码的回应消息
//**********************************
message CMessageSetPasswordResponse
{
	optional uint32 Errcode		=	1; 	 // 0表示设置成功,其他表示错误码
	optional uint32	Password	=	2;	 // 设置的密码
	optional uint32	Status		=	3;	 // 密码状态
};

//**********************************
// CMessageCheckPasswordRequest 
// 检查密码的请求
//**********************************
message CMessageCheckPasswordRequest
{	
	optional string PassWord  = 		1; 	// 待检查的密码
}

//**********************************
// CMessageCheckPasswordRequest 
// 检查密码的回应
//**********************************
message CMessageCheckPasswordResponse
{
	optional uint32 Errcode  =	1; 	// 0 表示成功其他表示错误码
	optional uint32	Status	 =	2;	// 密码状态	
}

//**********************************
// CMessageModifyPasswordRequest 
// 修改密码的请求
//**********************************
message CMessageModifyPasswordRequest
{
	optional string OrigPassword  = 		1; 		// 原始密码
	optional string NewPassword	  = 		2; 		// 新密码		
}

//**********************************
// CMessageModifyPasswordRequest 
// 修改密码的回应
//**********************************
message CMessageModifyPasswordResponse
{
	optional uint32 Errcode		 = 	1; 	   // 0表示成功其他表示错误码 	
	optional uint32	Status		 =	2;	   // 密码状态	 	
}

//**********************************
// CMessageSetPasswordOptionNotify 
// 设置密码选项
//**********************************
message CMessageSetPasswordOptionNotify
{
	optional uint32 Option    = 		1; 		 // 密码设置	
};

//**********************************
// 打开炉石传送表请求 
//**********************************

message CMessageClickTeleSymbolRequest
{
	optional uint32	Index	  = 		1; 		// 包裹索引
};

//**********************************
// 请求回复
//**********************************
message CMessageClickTeleSymbolResponse
{
	optional uint32	Index    = 		1; 		// 包裹索引
};

// *************************************************************************** //
// CMessageAddSlotRequest   游戏客户端->游戏服务器
// 增加凹槽请求
// *************************************************************************** //
message CMessageAddSlotRequest
{
	optional uint32 EquipIndex	=	1;		// 装备的位置
	optional uint32 ItemIndex	=	2;		// 物品的位置
};

// *************************************************************************** //
// CMessageAddSlotResponse   游戏服务器->游戏客户端
// 增加凹槽回应
// *************************************************************************** //
message CMessageAddSlotResponse   
{
	optional uint32 Slot  = 		1; 		// 增加的槽数
	optional uint32 Result  = 		2; 		// 增加凹槽的结果
};

// *************************************************************************** //
// CMessageRemoveJewelRequest   游戏客户端->游戏服务器
// 摘除宝石请求
// *************************************************************************** //
message CMessageRemoveJewelRequest
{
	optional uint32 JewelIndex  = 		1; 		// 摘除宝石的位置
	optional uint32 EquipIndex	=		2;		// 装备的位置
	optional uint32	ItemIndex	=		3;		// 符的位置
};

// *************************************************************************** //
// CMessageRemoveJewelResponse   游戏服务器->游戏客户端
// 摘除宝石回应
// *************************************************************************** //
message CMessageRemoveJewelResponse   
{
	optional uint32 Result  = 		1; 		// 摘除宝石的结果
};

// *************************************************************************** //
// CMessageJudgeRequest   游戏客户端->游戏服务器
// 鉴定装备请求
// *************************************************************************** //
message CMessageJudgeRequest
{
	optional uint32	EquipIndex	=	1;			// 装备的位置
	optional uint32	ItemIndex	=	2;			// 物品的位置
};

// *************************************************************************** //
// CMessageJudgeResponse   游戏服务器->游戏客户端
// 鉴定装备回应
// *************************************************************************** //
message CMessageJudgeResponse   
{
	optional uint32 Property3  = 		1; 	// 鉴定增加的数值
	optional uint32 Result  = 		2; 		// 鉴定装备的结果
	optional uint32 Property1  = 		3; 	// 鉴定增加的数值
	optional uint32 Property2  = 		4; 	// 鉴定增加的数值
};

// *************************************************************************** //
// CMessagePushAddSlotRequest   游戏客户端->游戏服务器
// 增加凹槽放入请求
// *************************************************************************** //
message CMessagePushAddSlotRequest
{
	optional uint32 Index  = 		1; 		// 放入的索引
};

// *************************************************************************** //
// CMessagePushAddSlotResponse   游戏服务器->游戏客户端
// 增加凹槽放入回应
// *************************************************************************** //
message CMessagePushAddSlotResponse   
{
	optional uint32 Index1  = 		1; 		// 放入的索引
	optional uint32 Result  = 		2; 		// 放入的结果
	optional uint32 Index2  = 		3; 		// 目标位置
};

// *************************************************************************** //
// CMessagePopAddSlotRequest   游戏客户端->游戏服务器
// 增加凹槽拿出请求
// *************************************************************************** //
message CMessagePopAddSlotRequest
{
	optional uint32 Index  = 		1; 		// 拿出的索引
};

// *************************************************************************** //
// CMessagePopAddSlotResponse   游戏服务器->游戏客户端
// 增加凹槽拿出回应
// *************************************************************************** //
message CMessagePopAddSlotResponse   
{
	optional uint32 Index2  = 		1; 		// 拿出位置
	optional uint32 Index1  = 		2; 		// 拿出的索引
	optional uint32 Result  = 		3; 		// 拿出的结果
};

// *************************************************************************** //
// CMessagePushRemoveJewelRequest   游戏客户端->游戏服务器
// 摘除宝石放入请求
// *************************************************************************** //
message CMessagePushRemoveJewelRequest
{
	optional uint32 Index  = 		1; 		// 放入的索引
};

// *************************************************************************** //
// CMessagePushRemoveJewelResponse   游戏服务器->游戏客户端
// 摘除宝石放入回应
// *************************************************************************** //
message CMessagePushRemoveJewelResponse   
{
	optional uint32 Index2  = 		1; 		// 目标位置
	optional uint32 Result  = 		2; 		// 放入的结果
	optional uint32 Index1  = 		3; 		// 放入的索引
};

// *************************************************************************** //
// CMessagePopRemoveJewelRequest   游戏客户端->游戏服务器
// 摘除宝石拿出请求
// *************************************************************************** //
message CMessagePopRemoveJewelRequest
{
	optional uint32 Index  = 		1; 		// 拿出的索引
};

// *************************************************************************** //
// CMessagePopRemoveJewelResponse   游戏服务器->游戏客户端
// 摘除宝石拿出回应
// *************************************************************************** //
message CMessagePopRemoveJewelResponse   
{
	optional uint32 Result  = 		1; 		// 拿出的结果
	optional uint32 Index1  = 		2; 		// 拿出的索引
	optional uint32 Index2  = 		3; 		// 拿出位置
};

// *************************************************************************** //
// CMessagePushJudgeRequest   游戏客户端->游戏服务器
// 鉴定装备放入请求
// *************************************************************************** //
message CMessagePushJudgeRequest
{
	optional uint32 Index  = 		1; 		// 放入的索引
};

// *************************************************************************** //
// CMessagePushJudgeResponse   游戏服务器->游戏客户端
// 鉴定装备放入回应
// *************************************************************************** //
message CMessagePushJudgeResponse   
{
	optional uint32 Index2  = 		1; 		// 目标位置
	optional uint32 Result  = 		2; 		// 放入的结果
	optional uint32 Index1  = 		3; 		// 放入的索引
};

// *************************************************************************** //
// CMessagePopJudgeRequest   游戏客户端->游戏服务器
// 鉴定装备拿出请求
// *************************************************************************** //
message CMessagePopJudgeRequest
{
	optional uint32 Index  = 		1; 		// 拿出的索引
};

// *************************************************************************** //
// CMessagePopJudgeResponse   游戏服务器->游戏客户端
// 鉴定装备拿出回应
// *************************************************************************** //
message CMessagePopJudgeResponse   
{
	optional uint32 Index1  = 		1; 		// 拿出的索引
	optional uint32 Result  = 		2; 		// 拿出的结果
	optional uint32 Index2  = 		3; 		// 拿出位置
};

//**********************************
// CMessageStorMoneyRequest 
// 存储金钱的消息请求
//**********************************
message CMessageStorMoneyRequest
{	 
	optional int32	StorMoney		 = 		1; 		// 存储的非绑定金钱
	optional int32	StorBindMoney	 = 		2; 		// 存储的绑定金钱
	optional uint32	NpcID			 =		3;		// 仓库服务器npc的实体ID
};	

//**********************************
// CMessageStorMoneyResponse 
// 存储金钱的消息请求
//**********************************
message CMessageStorMoneyResponse
{
	optional uint32 Errcode		 = 		1; 	   // 0表示成功其他表示错误码
	optional int32 Money		 = 		2; 	   // 仓库中现有的非绑定金钱的数量
	optional int32 BindMoney	 = 		3; 	   // 仓库中现有的绑定金钱的数量
	optional uint32 MoneyLmt	 = 		4; 	   // 限制的金钱 配合错误码使用
};

//**********************************
// CMessageTakeStorMoneyRequest 
// 从仓库取出金钱的请求
//**********************************
message CMessageTakeStorMoneyRequest
{	
	optional int32	Money		 = 	1; 			// 要从仓库取出的金钱的数量	
	optional int32	BindMoney	 = 	2; 			// 要从仓库取出的绑定金钱的数量
	optional uint32	NpcID		 =	3;			// npc实体ID
};	

//**********************************
// CMessageTakeStorMoneyResponse 
// 从仓库取出金钱的回应消息
//**********************************
message CMessageTakeStorMoneyResponse
{ 
	optional uint32 Errcode		 = 		1; 		// 0表示成功其他表示错误码
	optional int32 Money		 = 		2; 		// 仓库里面现在的非绑定金钱数量
	optional int32 BindMoney	 = 		3; 		// 仓库里面现在的绑定金钱的数量
	optional uint32 MoneyLmt	 = 		4; 	   // 限制的金钱 配合错误码使用
};

//**********************************
// CMessageAddKitBagIndexRequest 
// 在仓库的背包栏位放置背包的消息请求
//**********************************
message CMessageAddKitBagRequest
{
	optional uint32 SrcSlot		 =		1;			// 源位置	
	optional uint32 BagIndex	 = 		2; 			// 背包在包裹中的索引
	optional uint32 DesIndex	 = 		3; 			// 目标栏位索引
	optional uint32 DesSlot		 =		4;			// 目标位置是在仓库背包还是在包裹背包
};

//**********************************
// CMessageAddKitBagIndexResponse 
// 在仓库的背包栏位放置背包的消息回应
//**********************************
message CMessageAddKitBagResponse
{
	optional uint32 BagIndex	 = 		1; 			// 背包在包裹中的索引
	optional uint32 Errocode	 = 		2; 			// 0表示成功其他表示错误码
	optional uint32 DesIndex	 = 		3; 			// 目标栏位索引
	optional uint32 DesSlot		 =		4;			// 背包放置的位置仓库背包还是包裹背包
	optional uint32	ExpiredTime	 =		5;			// 背包的到期时间
	optional uint32	BindStatus	 =		6;			// 背包的绑定状态
};

//**********************************
// CMessageOpenKitBagRequest 
// 打开仓库背包的请求消息
//**********************************
message CMessageOpenKitBagRequest
{
	optional uint32 Index	= 		1; 					// 仓库背包栏位格子索引，无用的参数	
	optional uint32 DesSlot =		2;					// 仓库背包还是包裹背包	
};

//**********************************
// CMessageOpenKitBagResponse 
// 打开仓库背包的回应消息
//**********************************
message CMessageOpenKitBagResponse
{  
	optional uint32		IfClean			= 		1; 			 // 是否覆盖客户端数据,用EM_KITBAGSTATUS表示
	optional PBItemBox	ItemBox			= 		2; 			 // 背包物品
	optional uint32		Errcode			= 		3; 			 // 0表示成功其他表示错误码	
	optional uint32		IfInfoEnd		=		4;			 //	消息是否发送结束
	optional uint32		DesSlot			=		5;			 // 仓库背包还是包裹背包ESlotType 
	optional uint32		KitBagIndex		=		6;			 // 背包栏位索引
};

//**********************************
// CMessageMoveKitBagRequest 
// 打开仓库背包的回应消息
//**********************************
message CMessageMoveKitBagRequest
{
	optional uint32		DesIndex  = 		1; 
	optional uint32		SrcIndex  = 		2; 
};

// *************************************************************************** //
// CMessagePushChangeRequest   游戏客户端->游戏服务器
// 转化抗性放入请求
// *************************************************************************** //
message CMessagePushChangeRequest
{
	optional uint32 Index  = 		1; 		// 放入的索引
};

// *************************************************************************** //
// CMessagePushChangeResponse   游戏服务器->游戏客户端
// 转化抗性放入回应
// *************************************************************************** //
message CMessagePushChangeResponse   
{
	optional uint32 Result  = 		1; 		// 放入的结果
	optional uint32 Index2  = 		2; 		// 目标位置
	optional uint32 Index1  = 		3; 		// 放入的索引
};

// *************************************************************************** //
// CMessagePopChangeRequest   游戏客户端->游戏服务器
// 转化抗性拿出请求
// *************************************************************************** //
message CMessagePopChangeRequest
{
	optional uint32 Index  = 		1; 		// 拿出的索引
};

// *************************************************************************** //
// CMessagePopChangeResponse   游戏服务器->游戏客户端
// 转化抗性拿出回应
// *************************************************************************** //
message CMessagePopChangeResponse   
{
	optional uint32 Index2  = 		1; 		// 拿出位置
	optional uint32 Result  = 		2; 		// 拿出的结果
	optional uint32 Index1  = 		3; 		// 拿出的索引
};

// *************************************************************************** //
// CMessageChangeRequest   游戏客户端->游戏服务器
// 转化抗性请求
// *************************************************************************** //
message CMessageChangeRequest
{
	optional uint32 Type		=	1; 		// 转化的类型
	optional uint32	EquipIndex	=	2;		// 装备的位置
	optional uint32 ItemIndex	=	3;		// 转化石的位置
};

// *************************************************************************** //
// CMessageChangeResponse   游戏服务器->游戏客户端
// 转化抗性回应
// *************************************************************************** //
message CMessageChangeResponse   
{
	optional uint32 Type  = 		1; 		// 转化的类型
	optional uint32 Result  = 		2; 		// 转化的结果
};

// *************************************************************************** //
// CMessagePushJewComRequest   游戏客户端->游戏服务器
// 宝石合成放入请求
// *************************************************************************** //
message CMessagePushJewComRequest
{
	optional uint32 Index  = 		1; 		// 放入的索引
};

// *************************************************************************** //
// CMessagePushJewComResponse   游戏服务器->游戏客户端
// 宝石合成放入回应
// *************************************************************************** //
message CMessagePushJewComResponse   
{
	optional uint32 Result  = 		1; 		// 放入的结果
	optional uint32 Index1  = 		2; 		// 放入的索引
	optional uint32 Index2  = 		3; 		// 目标位置
};

// *************************************************************************** //
// CMessagePopJewComRequest   游戏客户端->游戏服务器
// 宝石合成拿出请求
// *************************************************************************** //
message CMessagePopJewComRequest
{
	optional uint32 Index  = 		1; 		// 拿出的索引
};

// *************************************************************************** //
// CMessagePopJewComResponse   游戏服务器->游戏客户端
// 宝石合成拿出回应
// *************************************************************************** //
message CMessagePopJewComResponse   
{
	optional uint32 Result  = 		1; 		// 拿出的结果
	optional uint32 Index2  = 		2; 		// 拿出位置
	optional uint32 Index1  = 		3; 		// 拿出的索引
};

// *************************************************************************** //
// CMessageJewComRequest   游戏客户端->游戏服务器
// 宝石合成请求
// *************************************************************************** //
message CMessageJewComRequest
{
	repeated uint32	JewelIndex		=	1;	// 宝石的位置
	optional uint32	ItemIndex		=	2;	// 符的位置
};

// *************************************************************************** //
// CMessageJewComResponse   游戏服务器->游戏客户端
// 宝石合成回应
// *************************************************************************** //
message CMessageJewComResponse   
{
	optional uint32 Result  = 		1; 		// 合成的结果
};

//**********************************
// CMessageListActivityMenuRequest
// 获取活动菜单请求
//**********************************
message CMessageListActivityMenuRequest
{

};

//**********************************
// CMessageListActivityMenuResponse
// 获取活动菜单请求
//**********************************
message PBOutlineActivity
{
	optional uint32 MenuID  = 		1;  // 菜单ID
	optional string Desc  = 		2; 	// 活动名称
};

message CMessageListActivityMenuResponse
{
	repeated PBOutlineActivity Activity  = 		1; 
};

//**********************************
// CMessageListActivityItemRequest
// 获取某菜单下的活动物品请求
//**********************************
message CMessageListActivityItemRequest
{
	optional uint32 MenuID  = 		1;  
};

//**********************************
// CMessageListActivityItemResponse
// 获取某菜单下的活动物品请求
//**********************************
message PBItemIDCount
{
    optional uint32 ActivityID = 1; // 活动ID
    optional string ItemCode   = 2; // 物品模板ID
    optional uint32 ItemCount  = 3; // 物品数量
};

message CMessageListActivityItemResponse
{
	optional uint32 MenuID  = 1; 	// 菜单ID
	repeated PBItemIDCount Items  = 2;  // 物品列表
};

//**********************************
// CMessageMinusActivityItemRequest
// 获取某活动物品请求
//**********************************
message CMessageMinusActivityItemRequest
{
	optional uint32      MenuID = 1;  // 菜单ID
	optional PBItemIDCount item = 2;	// 物品ID
};

//**********************************
// CMessageMinusActivityItemResponse
// 获取某活动物品回应
//**********************************
message CMessageMinusActivityItemResponse
{
	optional int32 Result  = 		1; 	// 0: 成功; 1: 失败
};

//**********************************
// CMessageInterruptContiuneSkillRequest
// 客户端主动发起打断持续施法技能请求
//**********************************
message CMessageInterruptContiuneSkillRequest
{
	optional uint32 SkillIndex	 = 		1; 	// 技能索引
	optional uint32	SkillID		 = 		2; 	// 技能ID
};


//**********************************
// CMessageInterruptContiuneSkillNotify
// 服务器通知打断持续施法技能
//**********************************
message CMessageInterruptContiuneSkillNotify
{
	optional uint32	SkillID		 = 		1; 	// 技能ID
	optional uint32	EntityID	 = 		2; 	// 实体ID
};

// *************************************************************************** //
// CMessageMultiComRequest   游戏客户端->游戏服务器
// 复杂合成请求
// *************************************************************************** //
message CMessageMultiComRequest
{
	optional uint32 ComposeID		= 		1; 		// 合成的功能
	repeated uint32 ComposeIndex	=		2;		// 合成的位置
	optional uint32	EntityID		=		3;		// npc兑换的实体ID
	optional uint32	ItemNum			=		4;		// 要对换的数量(目前只有家族军团兑换使用)
	optional uint32 ComposeType		=		5;		// 合成的类型（绑定，非绑定）
	optional uint32	EquipIndex		=		6;		// 需要升级的装备索引
};

// *************************************************************************** //
// CMessageMultiComResponse   游戏服务器->游戏客户端
// 复杂合成回应
// *************************************************************************** //
message CMessageMultiComResponse   
{
	optional uint32 ComposeID  = 		1; 	// 合成的功能
	optional uint32 Result  = 		2; 		// 合成的结果
	optional uint32	EquipIndex	=	3;		// 装备升级时装备的索引
};


//**********************************
// CMessageNotifyModifyPKValue
// 通知客户端修改PK值
//**********************************

message CMessageNotifyModifyPKValue
{
	optional int32	Value		 = 	1; 	// PK值。
	optional uint32	EntityID	 = 	2; 	// 玩家ID
	optional uint32	State		 =	3;	// 红1、2、3、4, 白名	 
};

//**********************************
// CMessageYellowStateNotify
// 通知客户端玩家为黄名状态
//**********************************

message CMessageYellowStateNotify
{
	optional uint32	EntityID	 = 		1; 	// 玩家ID
	optional uint32	State		 = 		2; 
};


//**********************************
// CMessageChangeAppertainings
// 通知客户端改变物品归属
//**********************************
message CMessageChangeAppertainings
{
	optional uint32 EntityID		 = 		1; 	// 物品实体ID
	repeated uint32 Appertainings	 = 		2; 	// 修改后的归属(charid),列表为空表示所有人
}

// *************************************************************************** //
// CMessageEventNotify   游戏客户端->游戏服务器
// 场景事件通知
// *************************************************************************** //
message CMessageEventNotify
{
	optional uint32 EventID = 1;		// 通知的错误码
	repeated uint32 IntParam = 2;		// 整型参数
	repeated string StringParam = 3;	// 字符型参数	
	optional uint32 CountryID	= 4;	// 发送到的阵营
};

// *************************************************************************** //
// CMessageActiveCardNotify   游戏服务器->游戏客户端
// 激活卡片通知
// *************************************************************************** //
message CMessageActiveCardNotify   
{
	optional uint32 Type = 1;		// 激活的册类别
	optional uint32 Pos = 2;		// 激活的册位置
	optional uint32 ItemID = 3;		// 激活的卡片ID
};

// *************************************************************************** //
// CMessagePushCardRequest   游戏客户端->游戏服务器
// 放入卡片请求
// *************************************************************************** //
message CMessagePushCardRequest
{
	optional uint32 Index = 1;		// 放入的索引
};

// *************************************************************************** //
// CMessagePushCardResponse   游戏服务器->游戏客户端
// 放入卡片回应
// *************************************************************************** //
message CMessagePushCardResponse   
{
	optional uint32 Result = 1;		// 放入的结果
	optional uint32 ItemID = 2;		// 放入的ID
};

// *************************************************************************** //
// CMessageTalentChangeNotify   游戏服务器->游戏客户端
// 修为值造化点改变通知
// *************************************************************************** //
message CMessageTalentChangeNotify   
{
	optional uint32 TalentExp = 1;		// 修为值当前值
	optional uint32 TalentPoint = 2;	// 造化点当前值
	optional uint32 TalentChange = 3;	// 修为值改变
};

// *************************************************************************** //
// CMessageClickTalentPointRequest   游戏客户端->游戏服务器
// 点击造化点请求
// *************************************************************************** //
message CMessageClickTalentPointRequest
{	
	optional uint32 Row = 1;		// 技能树纵坐标
	optional uint32 Col = 2;		// 技能树横坐标
};

// *************************************************************************** //
// CMessageClickTalentPointResponse   游戏服务器->游戏客户端
// 点击造化点回应
// *************************************************************************** //
message CMessageClickTalentPointResponse   
{
	optional uint32 Result = 1;		// 点击的结果
	optional uint32 Row = 2;		// 技能树纵坐标
	optional uint32 Col = 3;		// 技能树横坐标
	optional uint32 TalentExp = 4;	// 修为值当前值
	optional uint32 TalentPoint = 5;// 造化点当前值
	optional uint32 TalentStep = 6;	// 当前所在小阶
};

// *************************************************************************** //
// CMessageIncreaseTalentRequest   游戏客户端->游戏服务器
// 造化进阶请求
// *************************************************************************** //
message CMessageIncreaseTalentRequest
{	
	optional uint32 TalentStep = 1;		// 请求进的小阶
};

// *************************************************************************** //
// CMessageIncreaseTalentResponse   游戏服务器->游戏客户端
// 造化进阶回应
// *************************************************************************** //
message CMessageIncreaseTalentResponse   
{
	optional uint32 Result = 1;		// 进阶的结果
	optional uint32 TalentExp = 2;	// 修为值当前值
	optional uint32 TalentPoint = 3;// 造化点当前值
	optional uint32 TalentStep = 4;	// 当前所在小阶
};


// *************************************************************************** //
// CMessageAddPotentialRequest   游戏客户端->游戏服务器
// 增加法宝潜能点请求
// *************************************************************************** //
message CMessageAddPotentialRequest
{
	optional uint32 KeenPoints = 1;		// 锐力加点数
	optional uint32 CalmPoints = 2;		// 定力加点数
	optional uint32 MagicPoints = 3;	// 法力加点数
	optional uint32 BierPoints =  4;	// 灵力加点数
	optional uint32 ImmortalPoints = 5;	// 仙力加点数
};

// *************************************************************************** //
// CMessageAddPotentialResponse   游戏服务器->游戏客户端
// 法宝潜能加点回应
// *************************************************************************** //
message CMessageAddPotentialResponse
{
	optional uint32 Result		= 1; 	// 加点结果  和人物加点一样
};


// *************************************************************************** //
// CMessageChangeExpScaleRequest   游戏客户端->游戏服务器
// 玩家调整额外经验分配比例(百分比)请求
// *************************************************************************** //
message CMessageChangeExpScaleRequest
{
	optional uint32 Percent = 1;		// 百分比
};


// *************************************************************************** //
// CMessageChangeExpScaleResponse   游戏服务器->游戏客户端
// 玩家调整额外经验分配比例(百分比)回应
// *************************************************************************** //
message CMessageChangeExpScaleResponse
{
	optional uint32 Result		= 1; 	// 结果
};

// *************************************************************************** //
// CMessageMagicWeaponRefreshValueNotify   游戏服务器->游戏客户端
// 法宝相关数值刷新
// *************************************************************************** //
message CMessageMagicWeaponRefreshValueNotify
{
	optional uint32         MWLevel         = 1;			// 法宝等级
	optional uint64         Exp			    = 2;			// 法宝经验
	optional uint32         Mp				= 3;			// 法力值
	optional uint32         Hp				= 4;			// 灵气值
	optional uint32         CurKeenpoints   = 5;			// 当前锐力
	optional uint32         CurCalmpoints	= 6;			// 当前定力
	optional uint32         CurMagicpoints	= 7;			// 当前法力
	optional uint32         CurBierpoints   = 8;			// 当前灵力
	optional uint32         CurImmortalpoints = 9;			// 当前仙力
	optional uint32         LeftPoints      = 10;			// 当前剩余潜能点
	optional uint32         ProSpir			= 11;			// 灵性
	optional uint32         ProcessVal      = 12;			// 当前进度值
	optional uint32         LeftSkillPoints = 13;			// 当前剩余技能点
	optional uint32         IncMp		    = 14;			// 法力值每次恢复点数
	optional uint32         IncHp		    = 15;			// 灵气值每次恢复点数
	optional uint32         ProBase         = 16;			// 属性基础值
	optional uint32         ProUp			= 17;			// 属性成长值
	optional uint32         ProEff			= 18;			// 属性影响度
	optional int32          Index			= 19;			// 包裹索引 -1表示身上的
	optional uint32         MaxMp			= 20;			// 法力值上限
	optional uint32         MaxHp			= 21;			// 灵气值上限
	optional uint32			NextActBoxIndex	= 22;			// 下一个要激活的box索引
	optional uint32			PrintedQL		= 23;			// 印记的器灵ID
};

// *************************************************************************** //
// CMessageUpdateTitleNotice   游戏服务器->游戏客户端
// 通知客户端更新称号(直接替换原有称号、更新称号列表)
// *************************************************************************** //

message CMessageUpdateTitleNotice
{
	optional uint32	IsBatch = 1;	// 是否是批量更新称号列表
	optional uint32	Type = 2;		// 称号更新类型：0-直接替换，1-加入称号列表，2-称号作废
	optional uint32	TitleID	 =3;	// 称号ID，具体称号信息请从模板自己取	
	repeated uint32	TitleList = 4;	// 如果是批量更新称号列表，请从这个里面取称号ID
	optional uint32	Time	= 5;	// 称号 截止日期
	repeated uint32	TimeList = 6;	// 称号 截止日期列表	（批量处理时）
};

message CMessageSelectTitleRequest
{
	optional uint32	TitleID = 1;	// 称号ID
};

// 客户端请求选择称号之后，服务器会将选择的称号广播可见范围
message CMessageUpdateCurrentTitleNotice
{
	optional uint32	CharID		= 1;	// 更换称号的主角
	optional uint32	TitleID		= 2; // 称号ID
	optional string	TitleName	= 3; // 称号名称(像结拜这样不同人同一称号不同显示)
};

// *************************************************************************** //
// CMessageActiveSkillBoxNotify   游戏服务器->游戏客户端
// 激活技能框通知(没技能 刚激活只是个空框)
// *************************************************************************** //
message CMessageActiveSkillBoxNotify
{
	repeated uint32	Index = 1;	// 索引 0开始
};

// *************************************************************************** //
// CMessageInsertSkillNotify   游戏服务器->游戏客户端
// 法宝添加新技能通知 在鉴定、自行领悟后会发这个消息通知客户端新加技能或替换现有技能
// *************************************************************************** //
message CMessageInsertSkillNotify
{
	optional uint32	Index	 = 1;	// 索引 0开始
	optional uint32	SkillID  = 2;	// 技能ID
	optional uint32	CurLevel = 3;	// 技能当前等级
	optional uint32	Reason   = 4;	// 原因：1=自行领悟 2=手工鉴定
	optional uint32	ActedNum = 5;	// 已鉴定次数
	optional uint32	BookID   = 6;	// 已使用的技能书的模板ID
	optional uint32	BagIndex = 7;	// 法宝在包里的索引
};

// *************************************************************************** //
// CMessageMWAddSkillPointsRequest   游戏客户端->游戏服务器
// 法宝 加技能点请求
// *************************************************************************** //
message CMessageMWAddSkillPointsRequest
{
	optional uint32	Index	= 1;	// 技能框索引 0开始
};
// *************************************************************************** //
// CMessageMWAddSkillPointsResponse   游戏服务器->游戏客户端
// 法宝 加技能点回应
// *************************************************************************** //
message CMessageMWAddSkillPointsResponse
{
	optional uint32	Index	= 1;	// 技能框索引 0开始
};

// *************************************************************************** //
// CMessageMWJudgeSkillRequest   游戏客户端->游戏服务器
// 法宝 鉴定技能请求(激活了才能鉴定)
// *************************************************************************** //
message CMessageMWJudgeSkillRequest
{
	optional uint32	Index	= 1;	// 技能框索引 0开始
	optional uint32 NpcEntityID		= 2;	// 进行操作的NPC实体ID
};


// *************************************************************************** //
// 玩家放法宝上去 请求
// *************************************************************************** //
message CMessageLockMWInBagRequest
{
	optional uint32 ItemIndex			 = 		1; 	// 包裹索引
	optional uint32 LockType			 = 		2; 	// 锁定类型  见enMWLockType
};

// *************************************************************************** //
// 玩家放法宝上去 锁定包裹 回应
// *************************************************************************** //
message CMessageLockMWInBagResponse
{
	optional uint32 ItemIndex			 = 		1; 	// 索引
	optional uint32 LockType			 = 		2; 	// 锁定类型  见enMWLockType
};


// *************************************************************************** //
// 玩家取下法宝 解锁包裹 请求
// *************************************************************************** //
message CMessageMWTakeOffItemRequest
{
	optional uint32 LockType			 = 		1; 	// 锁定类型  见enMWLockType
};

// *************************************************************************** //
// CMessageMWJudgePropertyRequest   游戏客户端->游戏服务器
// 法宝 鉴定属性请求
// *************************************************************************** //
message CMessageMWJudgePropertyRequest
{
	optional uint32  NpcEntityID		= 1;	// 进行操作的NPC实体ID
	optional uint32  MWIndex			= 2;	// 法宝在包裹里的索引
	optional uint32  StoneIndex			= 3;	// 石头在包裹里的索引
};


// *************************************************************************** //
// CMessageMWJudgePropertyResponse   游戏客户端->游戏服务器
// 法宝 鉴定属性回应
// *************************************************************************** //
message CMessageMWJudgePropertyResponse
{
	optional uint32  Result		  =1;		// 鉴定结果 0=成功
	optional uint32  BagIndex     =2;		// 法宝在包裹里的索引
};

// *************************************************************************** //
// CMessageMWUseBookRequest   游戏客户端->游戏服务器
// 法宝 使用技能书请求
// *************************************************************************** //
message CMessageMWUseBookRequest
{
	optional uint32  MWIndex	  =1;		// 法宝索引
	optional uint32  BoxIndex	  =2;		// 法宝技能框索引
	optional uint32  BagIndex     =3;		// 技能书在包裹里的索引
};


// *************************************************************************** //
// CMessageInitialLifeSkillRequest   游戏客户端->游戏服务器
// 生活技能的入门请求
// *************************************************************************** //
message CMessageInitialLifeSkillRequest
{
	optional uint32	LifeSkillType	= 1;		// 生活技能
	optional uint32	EntityID		= 2;		// 实体ID
};

// *************************************************************************** //
// CMessageInitialLifeSkillResponse   游戏服务器->游戏客户端
// 生活技能的入门回应
// *************************************************************************** //
message CMessageInitialLifeSkillResponse
{
	optional uint32 Errcode				= 1;		// 错误码
	optional uint32	LifeSkillType		= 2;		// 生活技能类型
	optional uint32 LifeSkillLevel		= 3;		// 生活技能等级
	optional uint32 SpecializeDegree	= 4;		// 熟练度
	optional uint32 LifeSkillKind		= 5;		// 技能种类主技能还是辅修技能
};			


// *************************************************************************** //
// CMessageLevelUPLifeSkillRequest   客户端->服务器 
// 生活技能进阶的请求
// *************************************************************************** //
message CMessageLevelUPLifeSkillRequest
{
	optional uint32 LifeSkillType		= 1;		// 生活技能类型
	optional uint32	EntityID			= 2;		// 实体ID
};										

// *************************************************************************** //
// CMessageLevelUPLifeSkillResponse   服务器->客户端
// 生活技能进阶的回应
// *************************************************************************** //
message CMessageLevelUPLifeSkillResponse
{
	optional uint32 Errcode				= 1;	// 错误码
	optional uint32 LifeSkillType		= 2;	// 技能类型
	optional uint32 Specializedegree	= 3;	// 熟练度
	optional uint32 LifeSkillLevel		= 4;	// 生活技能等级
};

// *************************************************************************** //
// CMessageStudyComposeRequest   客户端->服务器
// 学习新的技能配方的请求
// *************************************************************************** //
message CMessageStudyComposeRequest
{
	optional uint32 ComPoseID		= 1;		// 配方ID
	optional uint32 StudyWay		= 2;		// 学习配方的方式(EM_ComposeGetWay)
	optional uint32 EntityID		= 3;		// npc实体ID,同过npc学习用
	optional uint32 Value			= 4;		// 如果创造新配方的话,是火候度	
	optional uint32	LifeSkillType	= 5;		// 生活技能类型
	optional uint32 SkillKind		= 6;		// 创造新配方时的分类，如烹饪的煎，炒等
	repeated uint32	MatierialIndex	= 7;		// 材料索引
};

// *************************************************************************** //
// CMessageStudyComposeResponse   服务器->客户端
// 学习新的技能配方的回应消息
// *************************************************************************** //
message CMessageStudyComposeResponse
{
	optional uint32 Errcode				=	1;		// 错误码
	optional uint32 ComposeID			=	2;		// 配方ID
	optional uint32 SpecializeDegree	=	3;		// 熟练度
	optional uint32 LifeSkillType		=	4;		// 生活技能类型	
	optional uint32 StudyWay			=	5;		// 学习配方的方式(EM_ComposeGetWay)
};															 

// *************************************************************************** //
// CMessageSetMagicStone   客户端->服务器
// 附魔的请求消息
// *************************************************************************** //
message CMessageSetMagicStoneRequest
{  
	optional uint32	MagicStoneIndex		=	1;		// 附魔石的索引
	optional uint32 EquipIndex			=	2;		// 装备的索引
	optional uint32	EquipSlot			=	3;		// 装备所在的包裹类型
};

// *************************************************************************** //
// CMessageSetMagicStoneResponse   服务器->客户端
// 附魔的消息回应
// *************************************************************************** //
message CMessageSetMagicStoneResponse
{
	optional uint32 Errcode				=	1;		// 错误码	
	optional uint32 EquipIndex			=	2;		// 装备的索引	
	optional uint32 AttributeID			=	3;		// 属性ID
	optional uint32	MagicNum			=	4;		// 装备附魔后的属性值
	optional uint32	MagicStoneID		=	5;		// 附魔石ID		
	optional uint32	EquipSlot			=	6;		// 装备所在的包裹类型
};

// *************************************************************************** //
// CMessageForgetSkillRequest   客户端->服务器
// 遗忘生活技能的请求
// *************************************************************************** //
message CMessageForgetSkillRequest
{  
	optional uint32	LifeSkillType		=	1;		// 生活技能类型	
};

// *************************************************************************** //
// CMessageForgetSkillRequest   客户端->服务器
// 遗忘生活技能的回应
// *************************************************************************** //
message CMessageForgetSkillResponse
{  
	optional uint32 Errcode				=	1;			// 错误码
	optional uint32 LifeSkillType		=	2;			// 生活技能类型
};

// *************************************************************************** //
// CMessageDecompoundEquipRequest   客户端->服务器
// 分解装备的请求
// *************************************************************************** //
message CMessageDecompoundEquipRequest
{  
	optional uint32	EquipIndex		=	1;		// 装备的索引	
	optional uint32 Slot			=	2;		// 包裹类型
};

// *************************************************************************** //
// CMessageDecompoundEquipRequest   客户端->服务器
// 分解装备的回应消息
// *************************************************************************** //
message CMessageDecompoundEquipResponse
{  
	optional uint32 Errcode			=	1;		// 错误码	
	optional uint32	EquipIndex		=	2;		// 装备的索引	
	optional uint32 Slot			=	3;		// 包裹类型
};

// *************************************************************************** //
// CMessageAddMaterialRequest   客户端->服务器
// 放置材料的请求
// *************************************************************************** //
message CMessageAddMaterialRequest
{		
	optional uint32	MaterialIndex		=	1;		// 材料的索引	
	optional uint32	PosIndex			=	2;		// 放置的位置索引
	optional uint32 Slot				=	3;		// 包裹类型
};

// *************************************************************************** //
// CMessageAddMaterialResponse   服务器->客户端
// 放置材料的回应
// *************************************************************************** //
message CMessageAddMaterialResponse
{  
	optional uint32	Errcode				=	1;		// 错误码
	optional uint32	MaterialIndex		=	2;		// 材料的索引	
	optional uint32	PosIndex			=	3;		// 放置的位置索引
};


// *************************************************************************** //
// CMessageTakeMaterialRequest   客户端->服务器
// 取下材料的请求
// *************************************************************************** //
message CMessageTakeMaterialRequest
{
	optional uint32	PosIndex			=	1;		// 待取下的材料所在的位置索引
};

// *************************************************************************** //
// CMessageAddMaterialResponse   服务器->客户端
// 取下材料的回应
// *************************************************************************** //
message CMessageTakeMaterialResponse
{ 
	optional uint32	Errcode			=	1;		// 错误码 
	optional uint32	PosIndex		=	2;		// 待取下的材料所在的位置索引	
};

// *************************************************************************** //
// CMessageLifeSkillAttributeNotify   服务器->客户端
// 生活技能属性改变的通知
// *************************************************************************** //
message CMessageLifeSkillAttributeNotify
{ 
	optional uint32	LifeSkillType			=	1;		// 生活技能类型 
	optional uint32	SpecializeDegree		=	2;		// 数量度		
};



//
// *************************************************************************** //
// CMessageMultiBuffRequest  客户端 - > 服务器
//  buff组合上报消息
// *************************************************************************** //
message PBEntityBuffEvent
{
	optional uint32 CharID	= 1; // 角色ID
	optional uint32 Param1	= 2; // 参数1( 师徒四人->BuffID ; )
};

message CMessageMultiBuffRequest
{
	optional uint32	EventID						= 1;	// 事件类型
	repeated PBEntityBuffEvent	BuffEventList	= 2;	// 作用实体列表
	optional uint32 NeedRefresh					= 3;	// (0-只刷新时间; 1-重新计算作用效果)
};


// *************************************************************************** //
// CMessageChangeCamp   服务器->客户端
// 生活技能属性改变的通知
// *************************************************************************** //
message CMessageChangeCamp
{
	optional uint32 EntityID			= 1;	
	optional uint32	CampID				= 2;
	optional uint32 CharID				= 3;
	optional uint32 TempID				= 4;
};

// *************************************************************************** //
// CMessageChangePKDropMode   客户端->服务器
// 改变PK掉落( 包括功勋等 )
// *************************************************************************** //
message CMessageChangePKDropMode
{
	optional bool IsDrop			= 1;	
};

// *************************************************************************** //
// CMessageGetEquipExchangeListRequest
// 获取装备兑换列表的消息请求
// *************************************************************************** //
message CMessageGetEquipExchangeListRequest
{
	optional uint32	EntityID	=	1;		 //	npc实体ID
};


// *************************************************************************** //
// CMessageGetEquipExchangeListRequest
// 获取装备兑换列表的消息请求
// *************************************************************************** //
message CMessageGetEquipExchangeListResponse
{
	repeated uint32	ComposeID	=	1;		// 配方ID
};

// 自动挂机请求
message CMessageOperateAutoHookRequest
{
	optional uint32	Action	= 1;	// 1:开始,0:取消
	optional uint32	RedPhyicsID = 2;	// 红药模板ID
	optional uint32	BluePhyicsID = 3;	// 蓝药模板ID
	optional uint32	EnergyPhyicsID = 4;	// 活力模板ID
};

//自动挂机回复
message CMessageOperateAutoHookResponse
{
	optional uint32 Action  = 1;
	optional uint32	RtnCode = 2; 
};

// 自动挂机时买药
message CMessageBuyPhysicRequest
{
	optional uint32	Index = 1;	// 药品索引
	optional uint32	Number = 2;	// 药品数量
	optional uint32	Type   = 3;	// 药品类型
	optional uint32	BuyType = 4;	// 购买方式(绑定元宝、非绑定元宝、金钱)
};

message CMessageBuyPhysicResponse
{
	optional uint32	RtnCode = 1;	// 购买回复
};

// *************************************************************************** //
// CMessagePushMultiRequest   游戏客户端->游戏服务器
// 复杂合成放入请求
// *************************************************************************** //
message CMessagePushMultiRequest
{
	optional uint32 Index  = 		1; 		// 放入的索引
};

// *************************************************************************** //
// CMessagePushMultiResponse   游戏服务器->游戏客户端
// 复杂合成放入回应
// *************************************************************************** //
message CMessagePushMultiResponse   
{
	optional uint32 Result  = 		1; 		// 放入的结果
	optional uint32 Index1  = 		2; 		// 放入的索引
	optional uint32 Index2  = 		3; 		// 目标位置
};

// *************************************************************************** //
// CMessagePopMultiRequest   游戏客户端->游戏服务器
// 复杂合成拿出请求
// *************************************************************************** //
message CMessagePopMultiRequest
{
	optional uint32 Index  = 		1; 		// 拿出的索引
};

// *************************************************************************** //
// CMessagePopMultiResponse   游戏服务器->游戏客户端
// 复杂合成拿出回应
// *************************************************************************** //
message CMessagePopMultiResponse   
{
	optional uint32 Result  = 		1; 		// 拿出的结果
	optional uint32 Index2  = 		2; 		// 拿出位置
	optional uint32 Index1  = 		3; 		// 拿出的索引
};


// *************************************************************************** //
// CMessageAddEcompoundEquipRequest   游戏客户端->游戏服务器
// 放置待分解装备的消息请求
// *************************************************************************** //
message CMessageAddEcompoundEquipRequest
{
	optional uint32	 EquipmentIndex = 1;			//	装备索引	
};

// *************************************************************************** //
// CMessageAddEcompoundEquipResponse   游戏服务器->游戏客户端
// 放置待分解装备的消息回应
// *************************************************************************** //
message CMessageAddEcompoundEquipResponse
{
	optional uint32	Errcode			=	1;		// 错误码
	optional uint32	EquipmentIndex	=	2;		// 装备索引
};

// *************************************************************************** //
// CMessageTakeDecompoundEquipRequest   游戏客户端->游戏服务器
// 拿下待分解装备的消息请求
// *************************************************************************** //
message CMessageTakeDecompoundEquipRequest
{
	optional uint32 MsgCode	=	1;		// 无用参数
};

// *************************************************************************** //
// CMessageTakeDecompoundEquipResponse   服务器->客户端 
// 拿下待分解装备的消息回应
// *************************************************************************** //
message CMessageTakeDecompoundEquipResponse
{ 
	optional uint32	Errcode		=	1;		// 错误码	
	optional uint32	EquipIndex	=	2;		// 装备索引
};

// *************************************************************************** //
// CMessagePlayerFcmStatusNotify   服务器->客户端 
// 防沉迷状态通知
// *************************************************************************** //
enum EFcmStatus
{
	em_none_rate = 0;				// 0 收益
	em_half_rate = 1;				// 50%收益
	em_normal_rate = 2;				// 正常收益
};

message CMessagePlayerFcmStatusNotify
{ 
	optional EFcmStatus  FcmStatus  =1;			//当前的状态
	optional int32       ValidSecond=2; 	    //当前状态剩余的有效时间秒数
};

// 法宝洗点消息
message CMessageMagicWeaponClearPointRequest
{
	optional  uint32     ClearType	=	1;			// 洗点类型 见enMW_ClearType
	optional  uint32     MWBagIndex	=	2;			// 法宝包裹索引
	optional  uint32     StoneBagIndex	= 3;		// 洗点石包裹索引
	optional  uint32	 NpcEntityID	= 4;	// 进行操作的NPC实体ID
};


// 法宝洗点消息回应
message CMessageMagicWeaponClearPointResponse
{
	optional  uint32     Result		=	1;		// 洗点回应  0成功
	optional  uint32     ClearType	=	2;		// 洗点类型 见enMW_ClearType
	optional  PBItemObject	MWObj	=	3;		// 新的法宝结构 覆盖即可
	optional  uint32     MWBagIndex	=	4;		// 法宝包裹索引
};

// 法宝洗点 放法宝请求
message CMessageMagicWeaponClearPointAddMWRequest
{
	optional  uint32     MWBagIndex	=	1;			// 法宝包裹索引
};


// 法宝洗点 放法宝回应
message CMessageMagicWeaponClearPointAddMWResponse
{
	optional  uint32     NewPoints			=	1;		// 新加的潜能点
	optional  uint32     NewSkillPoints		=	2;		// 新加的技能点
};

// *************************************************************************** //
// CMessageAddWizardPointRequest   游戏客户端->游戏服务器
// 增加才华等级请求
// *************************************************************************** //
message CMessageAddWizardPointRequest
{
	optional uint32 Type			= 		1; 		// 增加的类型
};

// *************************************************************************** //
// CMessageAddWizardPointResponse   游戏服务器->游戏客户端
// 增加才华等级回应
// *************************************************************************** //
message CMessageAddWizardPointResponse   
{
	optional uint32 Result			= 		1; 		// 增加的结果
	optional uint32 Type			= 		2; 		// 增加的类型
	optional uint32 Increase		=		3;		// 当前的增加点数
	optional uint32 CommonLeft		=		4;		// 剩余天赋点
};

// *************************************************************************** //
// CMessageHonorToWizardRequest   游戏客户端->游戏服务器
// *************************************************************************** //
message CMessageHonorToWizardRequest
{
	optional int32 ChangeHonor		 = 		1; 		// 兑换的功勋值
	optional int32 Type				 = 		2; 		// 兑换的类别
};

// *************************************************************************** //
// CMessageGetTrendRequest   游戏客户端->游戏服务器
// 算命请求
// *************************************************************************** //
message CMessageGetTrendRequest
{

};

// *************************************************************************** //
// CMessageGetTrendResponse   游戏服务器->游戏客户端
// 算命回应
// *************************************************************************** //
message CMessageGetTrendResponse   
{
	optional uint32 Result			= 		1; 		// 算命的结果
	optional uint32 Trend			= 		2; 		// 得到的算命值
};



// *************************************************************************** //
// CMessageClientRunTimePara   游戏服务器->游戏客户端
// 通知客户端运行时参数
// *************************************************************************** //
message PBRuntimeParam
{
	optional uint32 FunID	= 1;	// 功能ID
	optional uint32 OnOFF	= 2;	// 开关
	optional uint32 Param1	= 3;	// 参数1
	optional uint32 Param2	= 4;	// 参数2
	optional string Param3	= 5;	// 参数3
};


message CMessageClientRunTimeParamNotify  
{
	optional uint32 Version				= 		1; 		// 版本号
	repeated PBRuntimeParam Functions	= 		2; 		// 运行时参数
};

// 客户端请求帮助数据
message CMessageGetHelpDataRequest
{
};

// 活动时间
message PBActivityTime
{
	optional uint32		StartTime	=	1;	// 活动开始时间
	optional uint32		EndTime		=	2;	// 活动结束时间
	optional uint32		ShowFlag	=	3;	// 显示标志
};

// 帮助系统 单个数据
message PBSingleHelpData
{
	optional uint32		HelpID	=	1;	// 帮助ID
	repeated uint32		Params	=	2;	// 参数们(作废)
	optional uint32		Completed=	3;	// 已经完成XX次
	optional uint32		TotalTimes=	4;	// 总计XX次
	repeated PBActivityTime		TimeList =	5;	// 获得时间表
	optional uint32		WeekTimes=	6;	// 本周次数
};

// 今日XX提升
message PBTodayIncItem
{
	optional uint32		LevelUp	=	1;	// 今日等级提升
	optional uint32		ExpUp   =	2;	// 今日经验提升
	optional uint32		HonorUp =	3;	// 今日功勋提升
	optional uint32		AchiveUp =	4;	// 今日成就点数提升
	optional uint32		KillUp =	5;	// 今日荣誉击杀提升
};

// 客户端请求帮助数据 回应
message CMessageGetHelpDataResponse
{
	repeated PBSingleHelpData HelpDatas = 1;
	optional PBTodayIncItem   TodayUp	= 2; // 今日提升
};

message CMessagePickItemNotify
{
	optional uint32		EntityID	=	1;	
} ;

// 监狱倒计时通知
message CMessageProPrisonTimeNotice
{
	optional uint32	Time = 1;
};


// 拿下背包的消息请求
message CMessageTakeKitBagRequest
{
	optional uint32 SrcSlot		 =		1;			// 目标位置是在仓库背包还是在包裹背包
	optional uint32 SrcIndex	 = 		2; 			// 背包在背包栏位中的索引
	optional uint32 DesIndex	 = 		3; 			// 目标格子索引
	optional uint32 DesSlot		 =		4;			// 源位置		
};

// 拿下背包的消息回应
message CMessageTakeKitBagResponse
{	
	optional uint32 Errocode	 = 		1; 			// 0表示成功其他表示错误码
	optional uint32 SrcSlot		 = 		2; 			// 包裹背包还是仓库背包
	optional uint32 SrcIndex	 = 		3; 			// 背包在背包栏位的索引
	optional uint32 DesSlot		 =		4;			// 放置的目标是背包还是仓库
	optional uint32 DesIndex	 =		5;			// 背包在仓库或者仓库中的位置索引
};

// 获取传送坐标列表请求
message CMessageGetTeleListRequest
{
	optional uint32  BagIndex				= 1;	// 包裹索引
};

// 获取传送坐标列表回应
message CMessageGetTeleListResponse
{
	repeated PBTelePostion	TeleList		= 1;			// 飞行道具记忆点列表
	optional uint32  BagIndex				= 2;	// 包裹索引
};

// 记录传送点请求
message CMessageAddTelePointRequest
{
};

// 记录传送点回应
message CMessageAddTelePointResponse
{
	optional PBTelePostion	TelePoint		= 1;
};

// 删除传送点请求
message CMessageDelTelePointRequest
{
	optional uint32  Index					= 1;	// 要删除的索引 从0开始
};

// 删除传送点回应
message CMessageDelTelePointResponse
{
	optional uint32  Index					= 1;	// 删除的索引 从0开始
};

// 使用传送道具传送请求
message CMessageUseTeleItemRequest
{
	optional uint32  Index					= 1;	// 索引 从0开始
	optional uint32  BagIndex				= 2;	// 包裹索引
};

// vip 权限通知
message CMessageProVipNotice
{
	optional uint32	CharID = 1;
	optional uint32	Status = 2;		// 0 : 关闭， 1 : 开通
	optional uint32	Time	= 3;	// 会员到期时间
};

// 查询全区未领元宝余额请求
message CMessageQueryGoldenYBRequeset
{
	optional uint32	EntityID	= 1;	// NPC实体ID	
};

// 查询全区未领元宝余额回应
message CMessageQueryGoldenYBResponse
{
	optional int32 GoldenYB = 1;		// 金元宝余额
};

// 发送领取的元宝值
message CMessageDrawGoldenYBRequest
{
	optional uint32	EntityID	= 1;	// NPC实体ID	
	optional int32 RequiredGoldenYB = 2;	// 领取的元宝数
};

// 服务器回应是否领取成功
message CMessageDrawGoldenYBResponse
{
	optional int32 ErrorCode = 1;		// 消息码
};

// 使用道具广播（XX使用了XX道具）
message CMessagePlayerUseItemNotify
{
	optional uint32	EntityID	= 1;	// 玩家实体ID	
	optional uint32	ItemID		= 2;	// 物品ID
};

// 客户端->服务器 设置密码安全保护时间的请求
message CMessageSetPwdProtectTimeRequest
{
	optional uint32	ProtectTime	= 1;		
};

// 服务器->客户端 设置密码安全保护时间的消息回应
message CMessageSetPwdProtectTimeResponse
{
	optional uint32	Errcode		= 1;	// 错误码
	optional uint32	ProtectTime	= 2;	// 安全保护时间				
};


// 服务器->客户端 活动开始/结束通知
message SingleActivityNewState
{
	optional uint32 ActivityID		= 1;
	optional uint32 ActivityState   = 2; // 新状态 见enActivityState枚举
	optional uint32 CompleteTimes   = 3; // 已完成次数
};

// 网关通知场景XXX活动开始/结束了
message CMessageActivityChange2CliNotify
{
	repeated SingleActivityNewState ActivityList = 1;
};


// 使用鲜花请求
message CMessageUseFlowerRequest
{
	optional uint32 BagIndex = 1;	// 鲜花的包裹索引
	optional string DesName  = 2;	// 对方的角色名字
};

// 使用鲜花回应
message CMessageUseFlowerResponse
{
	optional uint32 ResultCode     = 1;	// 0成功 不然是错误码
	optional uint32 TodayLeftTimes = 2;	// 今天剩余赠送次数
};

// 获得鲜花通知
message CMessageObtainFlowerNotify
{
	optional uint32 CharID	 = 1;	// 获赠方的角色ID
	optional string SrcName  = 2;	// 赠送人的名称
	optional string DesName	 = 3;	// 获赠方的名字
	optional uint32 ItemID	 = 4;	// 使用的物品ID
};

// 【服务器之间】使用鲜花请求
message CMessageS2SPlayerUseFlowerRequest
{
	optional CMessageRouter Router			 = 		1;    
	optional uint32			BagIndex		 =		2; // 包裹索引
	optional uint32			ItemID			 =		3; // 使用的鲜花ID
};

// 【服务器之间】使用鲜花回应给场景服务器
message CMessageS2SPlayerUseFlowerResponse
{
	optional CMessageRouter Router			 = 		1;    
	optional uint32			ResultCode		 =		2; // 结果 ，0成功
	optional uint32			BagIndex		 =		3; // 包裹索引
};


// >客户端->服务器 普通绑定的消息请求
message CMessageCommonBindRequest
{	
	optional uint32	BindStoneIndex	= 1;		// 绑定石的索引
	optional uint32	ItemIndex		= 2;		// 物品的索引
	optional uint32	ItemSlot		= 3;		// 装备所在的包裹类型
};

// 服务器->客户端 普通绑定的消息回应	
message CMessageCommonBindResponse
{
	optional uint32	Errcode		=	1;	// 错误码
	optional uint32	ItemIndex	=	2;	// 物品索引
	optional uint32	ItemSlot	=	3;	// 物品所在的包裹类型
};

// 远程修理装备请求
message CMessageRemoteMendAllEquipRequest
{

};

message CMessageRemoteMendAllEquipResponse
{
	optional uint32	 RtnCode = 1;	// 返回码
};

// 客服端 -> 服务器 读进度条的请求
message CMessageStartProgressRequest
{
	optional uint32	FuncType	=	1;	// 读进度条的是那个模块(Em_ProgressFunctype)	
};

// 服务器->客户端 读进度条的消息回应
message CMessageStartProgressResponse
{
	optional uint32	Errcode	=	1;		// 错误码	
};

// *************************************************************************** //
// CMessageResetTalentRequest 游戏服务器-> 游戏客户端
// *************************************************************************** //
message CMessageResetTalentRequest
{
};


// *************************************************************************** //
// CMessageResetTalentResponse 游戏服务器-> 游戏客户端
// *************************************************************************** //
message CMessageResetTalentResponse
{
	optional uint32 Ret  = 		1; 	// 错误码
	optional uint32	Step =		2;	// 当前小阶
	optional uint32	Point =		3;	// 剩余点数
};


// 客户端获得活动信息
message CMessageGetActivityInfoRequest
{
	repeated uint32  ActivityID	= 1;	// 要索取的活动ID
	optional uint32	 TypeID		= 2;	// 区别码 （父节点ID）
};

message ActivityInfo_4MSG
{
	optional uint32  ActivityID = 1;	// 活动ID
	optional uint32  Times		= 2;	// 次数
};

// 客户端获得活动次数 回应
message CMessageGetActivityTimesResponse
{
	repeated ActivityInfo_4MSG	Info		= 1;	// 要索取的活动ID
	optional uint32				TypeID		= 2;	// 区别码 （父节点ID）
};

message CMessageChangeEquipProRequest
{
	optional uint32				EquipIndex	= 1;
	optional uint32				ItemIndex	= 2;
	optional uint32				NpcEntityID	= 3;
};

message CMessageChangeEquipProResponse
{
	optional uint32				Result		= 1;
	optional uint32				EquipIndex	= 2;
	optional uint32				ItemIndex	= 3;
	repeated uint32				PropertyID	= 4;
};

// 客服端->服务器 使用绣花针修理背包的消息请求
message CMessageUseKitbagNeedleRequest
{
	optional uint32	NeedleIndex = 1;		// 绣花针索引
	optional uint32	KitbagSlot	= 2;		// 背包所在的包裹类型(ESlotType)
	optional uint32	KitbagIndex	= 3;		// 背包所在的位置索引
};	

// 服务器->客户端 使用绣花针修理背包的消息回应
message CMessageUseKitbagNeedleResponse
{
	optional uint32	Errcode		= 1;		// 错误码
	optional uint32	KitbagSlot	= 2;		// 背包所在的包裹类型(ESlotType)
	optional uint32	KitbagIndex = 3;		// 背包所在的位置索引
	optional uint32	ExpiredTime	= 4;		// 背包的过期时间
	optional uint32	NeedleIndex = 5;		// 绣花针的索引	
	optional uint32	TimeLength	= 6;		// 使用绣花针后背包延长的使用时间
};

message CMessageOpenStoneRequest
{
	optional uint32	NpcEntityID		=	1;	// 点击的NPCID
};

message CMessageOpenStoneResponse
{
	optional uint32	Result			=	1;
	optional uint32	NpcEntityID		=	2;
};

// 安全保护密码重新锁定的消息请求
message CMessageLockPasswordRequest
{
		
};

// 安全保护密码重新锁定的消息回应
message CMessageLockPasswordResponse
{
	optional uint32	Errcode	=	1;	// 错误码
	optional uint32 Status	=	2;	// 现在的状态
};

message CMessageLearnOgreSkillRequest
{
	optional uint32	CardID		=	1;	// 卡片ID
};

message CMessageLearnOgreSkillResponse
{
	optional uint32	Result			=	1;
	optional uint32	CardID			=	2;
	optional uint32 Index1			=	3;
	optional uint32 Index2			=	4;
};

message CMessagePlayerPickItemNotify
{
	optional string PlayerName		=	1;
	optional uint32	ItemID			=	2;
};

message CMessageQueryPedalRequest
{
	optional uint32	NPCEntityID		=	1;		// NPCID
};

message CMessageQueryPedalResponse
{
	optional uint32 NPCEntityID		=	1;		// NPCID
	optional uint32 ExistTime		=	2;		// 存活时间
	repeated uint32 Hour			=	3;		// 刷新时间
};

message CMessageAutoBuyItemRequest
{
};

message CMessageAutoBuyItemResponse
{
};

message CMessageUseSummonItemRequest
{
	optional uint32	ItemIndex		=	1;
	optional string CharName		=	2;
};

message CMessageUseSummonItemResponse
{
	optional uint32	ItemIndex		=	1;
	optional string CharName		=	2;
	optional uint32 Result			=	3;
};

message CMessageSplitEquipRequest
{
	optional uint32	EquipIndex		=	1;
	optional uint32	ItemIndex		=	2;
};

message CMessageSplitEquipResponse
{
	optional uint32	EquipIndex		=	1;
	optional uint32	ItemIndex		=	2;
	optional uint32	Result			=	3;
};

message CMessageGetStarRequest
{
	optional uint32	EquipIndex		=	1;
	optional uint32	ItemIndex		=	2;
	optional uint32	NPCEntityID		=	3;
};

message CMessageGetStarResponse
{
	optional uint32	EquipIndex		=	1;
	optional uint32	ItemIndex		=	2;
	optional uint32	Result			=	3;
};

message CMessageAddStarRequest
{
	optional uint32	EquipIndex		=	1;
	optional uint32	ItemIndex		=	2;
};

message CMessageAddStarResponse
{
	optional uint32	EquipIndex		=	1;
	optional uint32	ItemIndex		=	2;
	optional uint32	Result			=	3;
	optional uint32	Star			=	4;		// 加星后的等级
};

// 装备过期通知
message CMessageEquipmentOverdueNotify
{
	optional uint32	EquipIndex		=	1;		// 装备索引
	optional uint32	pos_type		=	2;		// 物品位置 见EPosType枚举
};

// 物品开始过期计时通知
message CMessageItemStartTimerNotify
{
	optional uint32	pos_type		=	1;		// 物品位置 见EPosType枚举
	optional uint32 item_index		=	2;		// 索引
	optional uint32 overdue_time	=	3;		// 未来过期时间
};

message CMessageAddExtraRequest
{
	optional uint32	EquipIndex		=	1;
	optional uint32	ItemIndex		=	2;
	optional uint32	NPCEntityID		=	3;
};

message CMessageAddExtraResponse
{
	optional uint32	EquipIndex		=	1;
	optional uint32	ItemIndex		=	2;
	optional uint32	Result			=	3;
	optional uint32	ExtraType		=	4;		// 生成的类型
};

message CMessageChangeOwnCampNotify
{
	optional uint32	NPCEntityID		=	1;		// NPC ID
	optional uint32 CampID			=	2;		// 所属阵营ID
	optional uint32	PosX			=	3;
	optional uint32	PosY			=	4;
	optional uint32	TempID			=	5;
};

message CMessageWarOperateRequest
{
	optional uint32	NPCEntityID		=	1;		// NPC ID
	optional uint32	OpType			=	2;		// 操作类型
};

message CMessageWarOperateResponse
{
	optional uint32	NPCEntityID		=	1;		// NPC ID
	optional uint32	OpType			=	2;		// 操作类型
	optional uint32	Result			=	3;
};

// 客户端->服务器 是否可以使用采集道具的请求
message CMessageCanUseAutoCollectItemRequest
{	
	optional uint32 ItemIndex = 1;	// 道具索引	
};

// 服务器->客户端 是否可以使用采集道具的消息回应
message CMessageCanUseAutoCollectItemResponse
{	
	optional uint32	Errcode	  = 1;	// 错误码
	optional uint32	ItemIndex = 2;	// 道具索引	
};

// 客户端->服务器 使用自动采集道具采集的消息请求
message CMessageCollectByAutoItemRequest
{		
	optional uint32 ItemIndex = 1;	// 道具索引	
};

// 服务器->客户端 使用自动采集道具采集的消息回应
message CMessageCollectByAutoItemResponse
{	
	optional uint32	Errcode		= 1;	// 错误码
	optional uint32 ItemIndex	= 2;	// 道具索引	
	optional uint32 LeftUseTimes= 3;	// 剩余使用次数
};

message CMessageLearnFangCunSkillRequest
{
	optional uint32	SkillStage		=	1;
	optional uint32	SkillIndex		=	2;
	optional uint32	ItemIndex		=	3;
	optional uint32	NPCEntityID		=	4;
};

message CMessageLearnFangCunSkillResponse
{
	optional uint32	Result			=	1;
	optional uint32	SkillID			=	2;
	optional uint32	ItemIndex		=	3;
	optional uint32	ObjIndex		=	4;	// 技能索引
};

message CMessagePlayerTransformNotify
{
	optional uint32	EntityID		=	1;
	optional PBTransform Transform	=	2;
};

message CMessageHonorToItemRequest
{
	optional int32 ItemID		 = 		1; 		// 兑换的物品
	optional int32 ItemNum		 = 		2; 		// 兑换的个数
};

message CMessageUseBlackPillsNotify
{	
	optional uint32	PillIndex			= 1;		// 黑狗丸在包裹中的索引
	optional uint32	PillLeftTime		= 2;		// 黑狗丸的剩余使用时间
	optional uint32 OffLineTime			= 3;		// 玩家的剩余有效离线时间	
	optional uint32	Exp					= 4;		// 获取的经验
	optional uint32	OffLineTimeConsumed	= 5;		// 消耗的离线时间
	optional uint32	BlackPillsID		= 6;		// 离线经验丹的模板ID
	optional uint32	Errcode				= 7;		// 错误码
};

message CMessageExpContainerNotify
{
	optional uint32	CurrentExp			= 1;		// 修炼经验丹现在的经验
	optional uint32	ExpAdded			= 2;		// 修炼经验丹增加的经验
	optional uint32	Index				= 3;		// 修炼经验丹在背包中的索引	
	optional uint32	ExpPillsID			= 4;		// 修炼经验丹经验满时转换成的经验丹
};


message CMessageUpdateYuanBaoNotice
{
	optional uint32 MoneyType		= 1;//应只有5 #define BIND_MONEY 1; UNBIND_MONEY  2;  LINEKONG_MONEY   3; SILVER_YUANBAO   4; GOLDEN_YUANBAO   5
	optional int32  Amount			= 2;//加钱为+，减钱为-
};

message CMessageUpgAndJewNotify
{
	optional string	PlayerName			= 1;
	optional uint32	Type				= 2;		// 升级/合成的类型
	optional PBItemObject	ItemInfo	= 3;		// 物品属性
	optional uint32	CharID				= 4;		
};

message CMessageJewelCarveRequest
{
	optional uint32 JewelIndex		= 1;		// 宝石位置(装备)
	optional uint32 CardIndex		= 2;		// 符的位置
	optional uint32	JewelPos		= 3;		// 装备上放置宝石的位置
};

message CMessageJewelCarveResponse
{
	optional uint32	Result			= 1;
	optional uint32 JewelIndex		= 2;		// 宝石位置(装备)
	optional uint32 CardIndex		= 3;		// 符的位置
	optional uint32	JewelPos		= 4;		// 装备上放置宝石的位置
	optional uint32	EquipJewel		= 5;		// 装备上的宝石ID
};

//符文第二期
//符文转换
message CMessageRequestChangeRune
{
	optional uint32	RuneIndex	    =	1;			// 符文的位置
	optional uint32 RuneChangeIndex =   2;			// 转换符的位置
	optional uint32	BagSlot	= 3;					// 背包所在的包裹类型(ESlotType)
	
}

message CMessageResponseChangeRune
{
	optional uint32	Result			=	1;			// 符文转换结果 0 为成功
	optional uint32 SrcItemid		=	2;          // 目标 id
	optional uint32 RuneIndex		=   3;			// 道具的位置
	optional uint32 BagSlot			=   4;			// 背包所在的包裹类型(ESlotType)

}
//符文升级
// *************************************************************************** //
// CMessageRuneComRequest   游戏客户端->游戏服务器
// 符文升级请求
// *************************************************************************** //
message CMessageRequestRuneCom
{
	repeated uint32	RuneIndex		=	1;	// 符文的位置
	optional uint32	ItemIndex		=	2;	// 升级符的位置
};

message CMessageResponseRuneCom																	
{
	optional uint32	Result	 =	1;			// 符文转换结果 1 为成功 0 为失败
	optional uint32 Errorcod =  2 ;			// 错误码

}

message CMessageQueryBossRequest
{
};

message PBBoss
{
	optional uint32	TempID			= 1;		// 模板ID
	optional uint32 MapID			= 2;		// 地图ID
	optional uint32	PosX			= 3;		// 地图坐标X
	optional uint32	PosY			= 4;		// 地图坐标Y
	optional uint32	HP				= 5;		// 血量实际值
	optional uint32	ConfigTime		= 6;		// 配置的刷新时间（Hour*60+Min）
	optional uint32	NextTime		= 7;		// 下次复活时间（time_t）
};

message CMessageQueryBossResponse
{
	repeated PBBoss BossData		= 1;		
};

// *************************************************************************** //
// CMessageStartDigitalFireworksRequest   游戏客户端->游戏服务器
// 放数字礼花的消息请求
// *************************************************************************** //
message PBDigitalWords
{	
	optional uint32 PosX		= 1;		// 相对人物的横坐标
	optional uint32 PosY		= 2;		// 相对人物的纵坐标
	optional uint32	DigitalWord = 3;		// 字符	
};
message CMessageStartDigitalFireworksRequest
{	
	optional uint32			ItemIndex		= 1;			// 数字礼花在包裹中的坐标
	optional uint32			StyleID			= 2;			// 字符的特效ID
	repeated PBDigitalWords	DigitalWords 	= 3;			// 播放数字礼花信息
};

// *************************************************************************** //
// CMessageStartDigitalFireworksResponse  游戏服务器->游戏客户端
// 放数字礼花的消息回应
// *************************************************************************** //
message CMessageStartDigitalFireworksResponse
{	
	optional uint32	Errcode = 1;		// 错误码
};

// *************************************************************************** //
// CMessageStartDigitalFireworksResponse  游戏服务器->游戏客户端
// 放数字礼花的消息回应
// *************************************************************************** //
message CMessageStartDigitalFireworksNotify
{	
	optional uint32				EntityID	= 1;		// 放烟花的玩家的实体ID
	optional uint32				PosX		= 2;		// 放烟花的玩家的横坐标
	optional uint32				PosY		= 3;		// 放烟花的玩家的纵坐标
	optional uint32				StyleID		= 4;		// 字符的特效ID
	repeated PBDigitalWords		DigitalWords= 5;		// 烟花的字符信息
};


// *************************************************************************** //
// 玩家分解法宝 请求
// *************************************************************************** //
message CMessageDecomposeMWRequest
{
	optional uint32 ItemIndex			 = 		1; 	// 包裹索引
};

// *************************************************************************** //
// 玩家分解法宝 回应
// *************************************************************************** //
message CMessageDecomposeMWResponse
{
	optional uint32 ItemIndex			 = 		1; 	// 索引
	optional uint32 RetCode				 = 		2; 	// 返回码 0成功 否则错误码
};


// *************************************************************************** //
// 法宝器灵印记 请求
// *************************************************************************** //
message CMessageMWQLPrintRequest
{
	optional uint32 MWIndex				= 		1; 	// 法宝索引
	optional uint32 QLIndex				= 		2; 	// 器灵索引
	optional uint32 ItemIndex			= 		3; 	// 额外物品索引
};

// *************************************************************************** //
// 法宝器灵印记 回应
// *************************************************************************** //
message CMessageMWQLPrintResponse
{
	optional uint32 MWIndex			 = 		1; 	// 法宝索引
	optional uint32 QLID			 = 		2; 	// 器灵ID
	optional uint32 RetCode			 = 		3; 	// 返回码 0成功 否则错误码
};


// *************************************************************************** //
// 法宝飞升 请求
// *************************************************************************** //
message CMessageQuickUpRequest
{
	optional uint32 MWIndex				= 		1; 	// 法宝索引
	optional uint32 ItemIndex			= 		2; 	// 碎片索引
	optional uint32 UpType				= 		3; 	// 0：飞升  1：升品
	optional uint32 MWIndexEx			= 		4; 	// 副法宝索引
};

// *************************************************************************** //
// 法宝飞升 回应
// *************************************************************************** //
message CMessageQuickUpResponse
{
	optional uint32 MWIndex			 = 		1; 	// 法宝索引
	optional uint32 RetCode			 = 		2; 	// 返回码 0成功 否则错误码
	optional uint32 UpType			 = 		3; 	// 0：飞升  1：升品
};


// *************************************************************************** //
// 器灵合成 请求
// *************************************************************************** //
message CMessageQilinComposeRequest
{
	repeated uint32 QLIndex				= 		1; 	// 器灵索引
};

// *************************************************************************** //
// 器灵合成 回应
// *************************************************************************** //
message CMessageQilinComposeResponse
{
	optional uint32 RetCode			 = 		1; 	// 返回码 0成功 否则错误码
};

// *************************************************************************** //
// 法宝 获得经验 通知
// *************************************************************************** //
message CMessageMWObtainExpNotify
{
	optional uint32 ExpVal			 = 		1; 	// 数值
};


message CMessageQueryAroundPlayerRequest
{
	optional uint32 QueryRange = 1;				// 1 查找范围为附近， 2查找范围在整个地图
};

message AroundPlayer
{
	optional uint32 MapID			= 1;		// 地图ID
	optional string CharName		= 2;		// 用户名
	optional uint32 Sex				= 3;		// 性别
	optional uint32 HaveTeam		= 4;		// 是否有队伍
	optional uint32 TeamNum			= 5;		// 队伍人数
	optional uint32 Level			= 6;		// 等级
	optional uint32 MetierID		= 7;		// 门派ID
	optional uint32 CharID			= 8;		// 角色id
	optional uint32 EntityID		= 9;		// 实体ID
};

message CMessageQueryAroundPlayerResponse
{
	optional uint32 QueryRange		= 1;		// 1 查找范围为附近， 2查找范围在整个地图
	optional uint32 InRepetion		= 2;		//是否在副本内 0否，非0 是
	repeated  AroundPlayer 	PlayerData	= 3;	//玩家信息	
};

message CMessagePlayerRepetData
{
	optional uint32 RepetIndex		= 1;		// 副本索引
	optional uint32	RepetData		= 2;		// 玩家副本数据，暂时用单个数据

};

message CMessageSyncFameNotify
{
	optional uint32	CombatFame		= 1;		// 当前竞技声望
	optional uint32	RepeFame		= 2;		// 当前副本声望
	optional uint32	CommonFame		= 3;		// 当前历练声望
	optional uint32	CombatDelta		= 4;		// 新增竞技声望
	optional uint32	RepeDelta		= 5;		// 新增副本声望
	optional uint32	CommonDelta		= 6;		// 新增历练声望
	optional uint32	CombatToday		= 7;		// 今日竞技声望
	optional uint32	RepeToday		= 8;		// 今日副本声望
	optional uint32	CommonToday		= 9;		// 今日历练声望
};


// *************************************************************************** //
// CMessgeSpiritOpenRequest  游戏客户端->游戏服务器
// 启灵请求
// *************************************************************************** //
message CMessageSpiritOpenRequest
{
	optional uint32 PositionIndex	= 1;	// 部位
	optional uint32 SpiritType		= 2;	// 系别
	optional uint32	SlotIndex		= 3;	// 孔索引
}

// *************************************************************************** //
// CMessgeSpiritOpenResponse  游戏服务器->游戏客户端
// 启灵响应
// *************************************************************************** //
message CMessageSpiritOpenResponse
{
	optional uint32 Result	= 1;	// 结果
}

// *************************************************************************** //
// CMessgeSpiritCastingRequest  游戏客户端->游戏服务器
// 铸灵请求
// *************************************************************************** //
message CMessageSpiritCastingRequest
{
	optional uint32 PositionIndex	= 1;	// 部位
	optional uint32 SpiritType		= 2;	// 系别
	repeated uint32	SlotIndex		= 3;	// 孔索引
	repeated uint32	ItemIndex		= 4;	// 真灵道具在包裹中的索引
}

// *************************************************************************** //
// CMessgeSpiritCastingResponse  游戏服务器->游戏客户端
// 铸灵响应
// *************************************************************************** //
message CMessageSpiritCastingResponse
{
	optional uint32 Result		= 1;	// 结果
	repeated uint32	ItemIndex	= 2;	// 铸灵成功的真灵索引
}

// *************************************************************************** //
// CMessgeSpiritUpgradeRequest  游戏客户端->游戏服务器
// 聚灵请求
// *************************************************************************** //
message CMessageSpiritUpgradeRequest
{
	optional uint32 UpgradeItemIndex		= 1;	// 聚灵珠在包裹中的索引
	repeated uint32 SpiritItemIndex			= 2;	// 真灵在包裹中的索引
	optional uint32 IsBatch					= 3;	// 是否是批量
}

// *************************************************************************** //
// CMessgeSpiritUpgradeResponse  游戏服务器->游戏客户端
// 聚灵响应
// *************************************************************************** //
message CMessageSpiritUpgradeResponse
{
	optional uint32 Result		= 1;	// 结果
}

// *************************************************************************** //
// CMessgeSpiritDowngradeRequest  游戏客户端->游戏服务器
// 退灵请求
// *************************************************************************** //
message CMessageSpiritDowngradeRequest
{
	optional uint32 PositionIndex			= 1;	// 部位
	optional uint32 SpiritType				= 2;	// 系别
	optional uint32	SlotIndex				= 3;	// 孔索引
	optional uint32	DowngradeItemIndex		= 4;	// 退灵道具在包裹中的索引
}

// *************************************************************************** //
// CMessgeSpiritDowngradeResponse  游戏服务器->游戏客户端
// 退灵响应
// *************************************************************************** //
message CMessageSpiritDowngradeResponse
{
	optional uint32 Result		= 1;	// 结果
}

// *************************************************************************** //
// CMessgeSpiritInfoRequest  游戏服务器->游戏客户端
// 铸灵信息请求
// *************************************************************************** //
message CMessageSpiritInfoRequest
{
}

// *************************************************************************** //
// CMessgeSpiritInfoRequest  游戏服务器->游戏客户端
// 铸灵信息通知
// *************************************************************************** //

message CMessageSpiritInfoNotify
{
	repeated uint32 Point						= 1;	// 每个系别属性增加值
	optional PBSpiritInfo SpiritInfo			= 2;	// 每个部位的开孔、是否已铸灵信息
	optional uint32 Score						= 3;	// 总评分
}