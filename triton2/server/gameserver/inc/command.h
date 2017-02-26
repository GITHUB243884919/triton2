
/******************************************************************************
	版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

******************************************************************************
	文件名          : command.h
	版本号          : 1.0
	作者            : ZHL
	生成日期        : 2009-06-24
	最近修改        : 
	功能描述        : 命令模式结合观察者模式，这里先留出接口，为以后重构做准备。
	函数列表        : 
	修改历史        : 
	1 日期          : 2009-06-24
	作者			: ZHL
	修改内容		: 生成
*******************************************************************************/
#pragma once

#include "event.h"




// 命令类型
enum enCmdType
{
	ECMD_SENDPLAYERINFTOGATE	= 1,
};


class CCommand : public CEventObserver
{
public:

	CCommand() {}
	virtual ~CCommand() {}

	virtual int Do() = 0;

	virtual int OnEventTrigger( CEvent* pEvent ) = 0;
	
};


// 通知玩家个人信息改变到网关
class CCommandSendPlayerDirtyDataToGate : public CCommand
{
public:

	CCommandSendPlayerDirtyDataToGate( unsigned int unPlayerID ): mPlayerID( unPlayerID ), mDirtyDataBit(0) {}
	virtual ~CCommandSendPlayerDirtyDataToGate() {}


	virtual int Do();

	virtual int OnEventTrigger( CEvent* pEvent ) ;
	virtual int OnEventTrigger( int nBitFlag );

	enum
	{
		DIRTYDATA_LEVEL		= 0x0001,
		DIRTYDATA_LINEID	= 0x0002,
		DIRTYDATA_MAPID		= 0x0004,
		DIRTYDATA_METIER	= 0x0008,
		DIRTYDATA_HP		= 0x0010,
		DIRTYDATA_MP		= 0x0020,
		DIRTYDATA_RED		= 0x0040,
	};

private:

	unsigned int mPlayerID;	// 存ID是为了以后放在共享内存里
	int mDirtyDataBit;
};