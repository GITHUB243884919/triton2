--
--Apex2 Server Test 

--扫描器的一些配置------------------------------------------------------------------------------------------
function AddScannerConfig()
		var = {};
		var.strLibFName 	= "./scannerlib.SL";
		var.strLibKey			= "39061Ecc9FA549E6A4DA2F4F445F5ECB";
		var.strMulLib     = "./multilib.SL";
		var.nOpenSwapLib  = 0;
		var.nSwapInterval = 3600000;
		var.nDefaultMsg		= 1;
		var.nStartScanner	= 1;			--非0 ：启动扫描器
		var.uTryExit			= 0;			--非0 : 如果扫描器发生异常，则退出扫描器
		var.uSendTry			= 1;			--非0 : 如果扫描器发生异常，则向服务器报告
		var.uUnlimitRet   = 1;      --非0 : 允许客户端无限制的发信息，否则会踢掉，主要为了测试使用
		var.uScanTimeOut  = 0; --启动超时时限，若大于该时限便踢出
		LuaData( "ScannerConfig", var );
end;

AddScannerConfig();

--添加扫描结果处理逻辑--------------------------------------------------------------------------------------
function AddRetMan()
		var = {};
		var.m_uMaxSendRetTimes =4;
		var.m_vecRet = {};
		--uDealFlag 支持1和2
		var.m_vecRet[21] = { uScanRetID = 201, uDealFlag = 2, uKillNo = 63012 };			--jtw 4.8 wnd
		var.m_vecRet[22] = { uScanRetID = 112, uDealFlag = 1, uKillNo = 63014 };			--jtw 4.8 wnd
		var.m_vecRet[23] = { uScanRetID = 113, uDealFlag = 1, uKillNo = 63013 };			--jtw 4.8 wnd
		
		var.m_vecRet[2] = { uScanRetID = 221, uDealFlag = 1, uKillNo = 63021 };			--jtlh 3.6
		var.m_vecRet[3] = { uScanRetID = 222, uDealFlag = 1, uKillNo = 63022 };			--jqm 3.61
		var.m_vecRet[4] = { uScanRetID = 223, uDealFlag = 1, uKillNo = 63023 };			--猪儿修改版：ollydbg
		--var.m_vecRet[5] = { uScanRetID = 224, uDealFlag = 1, uKillNo = 63024 };			--jtbl 3.6.3
		
		var.m_vecRet[11] = { uScanRetID = 331, uDealFlag = 1, uKillNo = 63031 };			--jqm 3.61
		var.m_vecRet[12] = { uScanRetID = 332, uDealFlag = 1, uKillNo = 63032 };			--jtbl
		
		--如果VM执行扫描方式失败，则可能是外挂捣鬼，可以踢掉
		var.m_vecVMErr = {};
		--uDealFlag支持1
		--var.m_vecVMErr[1] = { nOpID = 1, nVMErr = 0x01, uDealFlag = 1, uKillNo = 62002 };
		
		LuaData( "RetMan", var );
end;

AddRetMan();
