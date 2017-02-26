
--有关反外挂服务器的一些不好归类的内容都放在此处
-- AddSRID2RIDMap() 添加扫描返回ID(opid * 100 + sendid)和要踢掉的任务RunID
function AddSRID2RIDMap()
	var = {};
	var.m_vecSR2TMap = {};
	--var.m_vecSR2TMap[0] = { uSRID = 101, uRunID = 1005 };
		
	LuaData( "SR2RID", var );
end;

AddSRID2RIDMap();