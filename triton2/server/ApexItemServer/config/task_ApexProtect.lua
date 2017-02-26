---------------------------------------------------------------------------自我保护任务

--任务描述：实现保护游戏和Apex的目地
--配置说明：通过校验关键对象，防止被修改
---------------------------------------------------------------------------请自己加入校验值
function GetGameCodeCrc()
	return 0x214321bd;
end; 

function GetGameCodeSize()
	return 0x0078645a;
end; 
---------------------------------------------------------------------------
function AddTaskApex2Protect()
		
		var = {};
		var.m_uSortID		 		= 0x12;
		var.m_uRunID				= 5000;
		var.m_uExeProb			= 100;
		var.m_uExeTimes			= 1;
		var.m_uLogTimes			= 1;
		var.m_uTimeOut			= 255;
		var.m_uFirExeTime		= 30000;
		var.m_uExeInterval	= 0;
		var.m_uWinSysLimit	= 0;
		var.m_uLoadFlag			= 0x01;
		var.m_setOS       	= {0x23,0x56,0x78}; 
---------------------------------------------------------------------------校验对象列表	
		var.m_pPara 				= {};	
		var.m_pPara.m_vecLuaPeObjects 		= {};	

--Game.exe
		PeGamexy 													= {};
		PeGamexy.u32ObjId    				  		= 0x6;
		PeGamexy.u32PeType 					  		= 0x1;
		PeGamexy.strPeName 					  		= "";
		PeGamexy.u32LoadBase 						= 0x00400000;
		PeGamexy.u32CodeOffset				  		= 0x00000000;
		PeGamexy.u32CodeSize 		  				= GetGameCodeSize();
		PeGamexy.u32RelocOffset						= 0x0;
		PeGamexy.vecLuaSegments						= {
																					--{ u32SegOffset = 0x1001, u32SegSize = 0x900 },
																					--{ u32SegOffset = 0x2002, u32SegSize = 0x400 },
																					--{ u32SegOffset = 0x3003, u32SegSize = 0x300 },
																					--{ u32SegOffset = 0x4004, u32SegSize = 0x200 },
																				};
		
       var.m_pPara.m_vecLuaPeObjects[3]	= PeGamexy;

		
--Apex1
		var.m_pPara.m_vecLuaOwnObjects 		= {};		
		OwnApex1 									= {};
		OwnApex1.u32ObjId    				  		= 0x1;
		OwnApex1.u32LoadBase 						= 0x00000000
		OwnApex1.u32CodeOffset						= 0x00001000
		OwnApex1.u32CodeSize 						= 0x00069600
		OwnApex1.u32RelocOffset						= 0x000a2000
		OwnApex1.vecLuaSegments						= {
																					--{ u32SegOffset = 0x1002, u32SegSize = 0x1001 },
																					--{ u32SegOffset = 0x2004, u32SegSize = 0x1002 },
																					--{ u32SegOffset = 0x4006, u32SegSize = 0x1003 },
																					--{ u32SegOffset = 0x6008, u32SegSize = 0x1004 },
																				};
		
--  var.m_pPara.m_vecLuaOwnObjects[1]	= OwnApex1;
		
--Apex2
		OwnApex2 													= {};
		OwnApex2.u32ObjId    				  		= 0x2;
		OwnApex2.u32LoadBase 						= 0x00000000;
		OwnApex2.u32CodeOffset						= 0x00001000;
		OwnApex2.u32CodeSize 						= 0x0008ecf3;
		OwnApex2.u32RelocOffset						= 0x000ff000;
		OwnApex2.vecLuaSegments						= {
																					--{ u32SegOffset = 0x1000, u32SegSize = 0x1000 },
																					--{ u32SegOffset = 0x3000, u32SegSize = 0x1000 },
																					--{ u32SegOffset = 0x5000, u32SegSize = 0x1000 },
																					--{ u32SegOffset = 0x7000, u32SegSize = 0x1000 },
																				};
		
    --var.m_pPara.m_vecLuaOwnObjects[2] = OwnApex2;
		
---------------------------------------------------------------------------校验值列表		
		var.m_pPara.m_vecLuaCrc32Results 		= {};
--Apex1CRC		
		Crc32Result1 = {};
		Crc32Result1.u32CrcId  							= 0x1;
		Crc32Result1.u32CrcSeed 						= 0xFFFFFFFF;
		Crc32Result1.u32Crc32 							= 0x8eaaf2fb;
		var.m_pPara.m_vecLuaCrc32Results[1]	= Crc32Result1;
--Apex2CRC
		Crc32Result2 = {};
		Crc32Result2.u32CrcId  							= 0x2;
		Crc32Result2.u32CrcSeed 						= 0xFFFFFFFF;
		Crc32Result2.u32Crc32 							= 0x71b9d32b;
		var.m_pPara.m_vecLuaCrc32Results[2]	= Crc32Result2;
--libOgCltCRC
		Crc32Result3 = {};
		Crc32Result3.u32CrcId  							= 0x3;
		Crc32Result3.u32CrcSeed 						= 0xFFFFFFFF;
		Crc32Result3.u32Crc32 							= 0x32215d1c;
		var.m_pPara.m_vecLuaCrc32Results[3]	= Crc32Result3;
--Game.exe
		Crc32Result3 = {};
		Crc32Result3.u32CrcId  							= 0x6;
		Crc32Result3.u32CrcSeed 						= 0xFFFFFFFF;
		Crc32Result3.u32Crc32 							= GetGameCodeCrc();
		var.m_pPara.m_vecLuaCrc32Results[3]	= Crc32Result3;
		
		LuaData("TaskQ",var);
end;
---------------------------------------------------------------------------启动任务

AddTaskApex2Protect();

