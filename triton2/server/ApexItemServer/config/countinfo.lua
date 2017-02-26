--用于初始统计信息，现统计踢人用户比例以限定踢人动作。
function AddCountInfo()
var = {};
	  var.uPersentUser    	= 100; --最大踢人的百分比
	  var.uNumUserKillMin 	= 0; --最多踢人数目
	  var.uNumUserLoginMin  = 0; --最少登录的人数
	  var.uTimeInterval   	= 60000;--限定的时间，在时间内执行该功能,超过这个时间段正常踢人
	  LuaData( "CountInfo", var );
end;
----------------------------------------
AddCountInfo();
