#ifndef _SKILL_LIST_H_
#define _SKILL_LIST_H_

#include "lk_vector.h"
#include "coretype.h"
#include "skill.h"
#include "timermanager.h"

#define MAX_SKILL_NUM 250

class CPropertyCharacter;
class PBSkillObject;
class PBSkillList;

struct BuffState 
{
	BuffState()
	{
		if(CObj::msCreateMode)
		{
			Initialize();
		}
	}

	~BuffState() {}

	void Initialize()
	{
		mBuffID = 0;
		mRand = 0;
		mTime = 0;
		mTarget = 0;
		mHP = 0;
	}

	int mBuffID;		// 特殊状态的BUFFID
	int mRand;			// 出现的概率
	int mTime;			// 冷却时间
	int mTarget;		// 目标
	int	mHP;			// HP百分比
};

typedef lk::vector< BuffState, STATENUM > StateList;


//////////////////////////////////////////////////////////////////////////

/**
** 技能列表接口类，配合模板类使用
**
*/
class CSkillList
{
public:

	// 得到技能数
	virtual int GetSkillNum( ) = 0;

	// 得到指定系别的技能等级总和
	virtual int GetGenerPoint( int vGenerID ) = 0;

	// 通过技能ID找到技能对象
	virtual CSkillObject* GetSkillObjectByID( unsigned int vSkillID ) = 0;

	// 通过技能ID找到技能索引( >=0 有效 )
	virtual int GetSkillIndexByID( unsigned int usSkillID ) = 0;

	// 通过技能索引找到技能对象
	virtual CSkillObject* GetSkillObjectByIndex( unsigned int unSkillIndex ) = 0;

	// 插入技能
	virtual bool InsertSkill( unsigned int vSkillID, int vMaxLevel, int nMWSkillID = 1 ) = 0;

	// 删除技能
	virtual void RemoveSkill( unsigned int vSkillID ) = 0;

	// 从PB结构创建数据
	virtual void CreateFromTlv2( PBSkillList* pBuffer ) = 0;

	// 把数据赋值给PB结构
	virtual void GetTlvBuffer2( PBSkillList* pBuffer ) = 0;

	// 获得技能增加数值固定值
	virtual unsigned int  GetFixNumber( CTemplateSkill::PASSINC_TYPE vType ) = 0;

	// 获得技能增加数值百分比
	virtual int  GetPerNumber( CTemplateSkill::PASSINC_TYPE vType ) = 0;

	// 获得某个技能改变量
	virtual int GetValueByID( unsigned int vSkillID, CTemplateSkill::PASSINC_TYPE vType ) = 0;

	// 获得技能增加的状态ID
	virtual int	GetStateID( StateList& vBuffList, CTemplateSkill::PASSINC_TYPE vType ) = 0;

	// 清空技能列表
	virtual void Clean( ) = 0;

	// 清空技能CD
	virtual void ClearCooldown( ) = 0;

	// 刷新技能CD
	virtual void RefreshCooldown() = 0;
};


//////////////////////////////////////////////////////////////////////////

template< int SIZE >
class CSkillListInstance : public CSkillList
{
private:
	unsigned short	mSkillNumber;
	CSkillObject	mSkillList[ SIZE ];					// 技能列表

public:
	CSkillListInstance( );
	~CSkillListInstance( );

	int Initialize( );
	int Resume( );

	// 清空技能列表
	void Clean( );

	// 清空技能列表2
	int PartClean( );

	// 得到技能数
	int GetSkillNum( ) { return mSkillNumber; }

	// 得到技能列表最大值
	int MaxSkillSize( ) { return SIZE; };

	// 得到指定系别的技能等级总和
	int GetGenerPoint( int vGenerID );

	// 通过技能ID找到技能对象
	CSkillObject* GetSkillObjectByID( unsigned int vSkillID );

	// 通过技能ID找到技能索引( >=0 有效 )
	int GetSkillIndexByID( unsigned int usSkillID );

	// 通过技能索引找到技能对象
	CSkillObject* GetSkillObjectByIndex( unsigned int unSkillIndex );

	// 插入技能  第三个参数：法宝技能初始等级
	bool InsertSkill( unsigned int vSkillID, int vMaxLevel, int nMWSkillID = 1 );

	// 降低技能等级 
	bool SkillLevelDown( unsigned int vSkillID, int nLevel =1 );

	// 删除技能
	void RemoveSkill( unsigned int vSkillID );

	// 从PB结构创建数据
	void CreateFromTlv2( PBSkillList* pBuffer );

	// 把数据赋值给PB结构
	void GetTlvBuffer2( PBSkillList* pBuffer );

	// 获得技能增加数值固定值
	unsigned int  GetFixNumber( CTemplateSkill::PASSINC_TYPE vType );

	// 获得技能增加数值百分比
	int  GetPerNumber( CTemplateSkill::PASSINC_TYPE vType );

	// 获得某个技能改变量
	int GetValueByID( unsigned int vSkillID, CTemplateSkill::PASSINC_TYPE vType );
	
	// 获得技能增加的状态ID
	int	GetStateID( StateList& vBuffList, CTemplateSkill::PASSINC_TYPE vType );

	// 清空技能CD
	void ClearCooldown( );

	// 刷新技能CD
	virtual void RefreshCooldown() ;

};



//////////////////////////////////////////////////////////////////////////
// CSkillList
//////////////////////////////////////////////////////////////////////////
template< int SIZE >
CSkillListInstance<SIZE>::CSkillListInstance( )
{
	if ( CObj::msCreateMode )
	{
		Initialize( );
	} 
	else
	{
		Resume( );
	}
}

template< int SIZE >
CSkillListInstance<SIZE>::~CSkillListInstance( )
{
}

template< int SIZE >
int CSkillListInstance<SIZE>::Initialize( )
{
	Clean( );
	return 0;
}

template< int SIZE >
int CSkillListInstance<SIZE>::Resume( )
{
	return 0;
}

template< int SIZE >
void CSkillListInstance<SIZE>::Clean( )
{
	mSkillNumber = 0;
	::memset( mSkillList, 0, sizeof( mSkillList ) );
}

// 得到指定系别的技能等级总和
template< int SIZE >
int CSkillListInstance<SIZE>::GetGenerPoint( int vGenerID )
{
	int tSkillPoint = 0;
	for ( int i = 0; i < mSkillNumber; i ++ )
	{
		CTemplateSkill* tpTempSkill = (CTemplateSkill*) CDataStatic::SearchSkill( mSkillList[ i ].GetSkillID(), mSkillList[ i ].GetSkillLevel() - mSkillList[ i ].GetIncLevel() );
		if ( tpTempSkill == NULL )
			continue;

		if ( tpTempSkill->mGenerID == vGenerID )
			tSkillPoint += mSkillList[ i ].GetSkillLevel() - mSkillList[ i ].GetIncLevel();
	}

	return tSkillPoint;
}

template< int SIZE >
CSkillObject* CSkillListInstance<SIZE>::GetSkillObjectByID( unsigned int vSkillID )
{
	for ( int i = 0; i < mSkillNumber; i ++ )
	{
		if ( mSkillList[ i ].GetSkillID() == vSkillID )
			return &mSkillList[ i ];
	}

	return NULL;
}

template< int SIZE >
int CSkillListInstance<SIZE>::GetSkillIndexByID( unsigned int usSkillID )
{
	for ( int i = 0; i < mSkillNumber; i ++ )
	{
		if ( mSkillList[ i ].GetSkillID() == usSkillID )
			return i;
	}

	return -1;
}

template< int SIZE >
CSkillObject* CSkillListInstance<SIZE>::GetSkillObjectByIndex( unsigned int unSkillIndex )
{
	if( unSkillIndex >= mSkillNumber )
	{
		return NULL;
	}

	return &mSkillList[ unSkillIndex ];
}


//************************************
// Method:    InsertSkill
// FullName:  CSkillList::InsertSkill
// Access:    public 
// Returns:   bool
// Qualifier: 插入技能时重新修改技能冷却时间
// Parameter: unsigned short vSkillID
// Parameter: int vMaxLevel
// Parameter: unsigned int & vpSkillID
// Parameter: int & vIndex
//************************************
template< int SIZE >
bool CSkillListInstance<SIZE>::InsertSkill( unsigned int vSkillID, int vMaxLevel, int nMWSkillID /*= 1*/ )
{
	CTemplateNormalSkill* tpSkill = ( CTemplateNormalSkill* )CDataStatic::SearchSkill( vSkillID, vMaxLevel );
	if( tpSkill == NULL )
	{
		LOG_ERROR( "pro", "[%s:%d] skill id %d level %d not found", __FUNCTION__, __LINE__, vSkillID, vMaxLevel );
		return false;
	}

	for ( unsigned int i = 0; i < ARRAY_CNT( mSkillList ); i ++ )
	{

		if ( mSkillList[ i ].GetSkillID() == vSkillID )
		{
			if ( mSkillList[ i ].GetSkillLevel() - mSkillList[ i ].GetIncLevel( ) == (unsigned int)vMaxLevel )
				return false;

			mSkillList[ i ].SetSkillLevel( mSkillList[ i ].GetSkillLevel() + 1 ) ;
			mSkillList[ i ].SetOriginalCooldown( tpSkill->mCDTime );

			return true;
		}

		if ( mSkillList[ i ].GetSkillID() == 0 )
		{
			mSkillList[ i ].SetSkillID( vSkillID );
			mSkillList[ i ].SetCooldown( 0 );
			mSkillList[ i ].SetSkillLevel( nMWSkillID );
			mSkillList[ i ].SetOriginalCooldown( tpSkill->mCDTime );
			mSkillNumber = i + 1;						// changed by lixi

			return true;
		}
	}

	return false;
}


//************************************
// Method:    SkillLevelDown	
// Qualifier: 技能等级降低 法宝的小洗功能用
//************************************
template< int SIZE >
bool CSkillListInstance<SIZE>::SkillLevelDown( unsigned int vSkillID, int nLevel )
{
	for ( unsigned int i = 0; i < ARRAY_CNT( mSkillList ); i ++ )
	{
		if ( mSkillList[ i ].GetSkillID() == vSkillID )
		{
			mSkillList[ i ].SetSkillLevel( mSkillList[ i ].GetSkillLevel() - nLevel);
			return true;
		}
	}

	return false;
}



template< int SIZE >
void CSkillListInstance<SIZE>::RemoveSkill( unsigned int vSkillID )
{
	for ( int i = 0; i < mSkillNumber; i ++ )
	{
		if ( mSkillList[ i ].GetSkillID() == vSkillID )
		{
			// 不是最后一个，需要保持连续存储
			// TODO: 装备插入的技能,因为可能从中间删除,所以留待以后处理
			if( i != mSkillNumber-1 )
			{
				for ( int j = i; j < mSkillNumber; j ++ )
					mSkillList[ j ] = mSkillList[ j + 1 ];
			}
			mSkillList[ mSkillNumber-1 ].Initialize();// 初始化掉最后1个节点
			mSkillNumber --;
			break;
		}
	}
}

template< int SIZE >
void CSkillListInstance<SIZE>::CreateFromTlv2( PBSkillList* pBuffer )
{
	mSkillNumber = pBuffer->skillnumber();
	if ( mSkillNumber > ARRAY_CNT(mSkillList) )
	{
		LOG_ERROR( "pro", "[%s:%d] skill num %d over max %d ", __FUNCTION__, __LINE__, mSkillNumber, ARRAY_CNT(mSkillList) );
		mSkillNumber = ARRAY_CNT(mSkillList);
	}

	for ( int i = 0; i < mSkillNumber && i < pBuffer->skillobjects_size(); i ++ )
	{
		PBSkillObject* tpPBSkillObject = pBuffer->mutable_skillobjects( i );
		mSkillList[ i ].SetSkillID( tpPBSkillObject->skillid() );
		mSkillList[ i ].SetSkillLevel( tpPBSkillObject->skilllevel() );
		mSkillList[ i ].SetCooldown( tpPBSkillObject->cooldown() );
		mSkillList[ i ].SetIncLevel( tpPBSkillObject->inclevel() );
	}
}


template< int SIZE >
void CSkillListInstance<SIZE>::GetTlvBuffer2( PBSkillList* pBuffer )
{
	// 防止数组越界
	if( mSkillNumber > ARRAY_CNT(mSkillList) )
	{
		mSkillNumber = ARRAY_CNT( mSkillList );
	}

	pBuffer->set_skillnumber( mSkillNumber );

	for ( int i = 0; i < mSkillNumber; i ++ )
	{
		PBSkillObject* tpPBSkillObject = pBuffer->add_skillobjects();

		tpPBSkillObject->set_skillid( mSkillList[ i ].GetSkillID() );
		tpPBSkillObject->set_skilllevel( mSkillList[ i ].GetSkillLevel() );
		if( mSkillList[ i ].GetCooldown() != 0 )
			tpPBSkillObject->set_cooldown( mSkillList[ i ].GetCooldown() );
		if( mSkillList[ i ].GetIncLevel() != 0 )
			tpPBSkillObject->set_inclevel( mSkillList[ i ].GetIncLevel() );
	}
}

// 获得技能增加固定数值
template< int SIZE >
unsigned int CSkillListInstance<SIZE>::GetFixNumber(  CTemplateSkill::PASSINC_TYPE vType )
{
	unsigned int tValue = 0;
	int i = 0;

	for( i = 0; i < mSkillNumber; i++ )
	{	
		int tSkillLevel = 0;													
		tSkillLevel += mSkillList[ i ].GetSkillLevel();		
		CTemplateSkill* tpSkillTempl = (CTemplateSkill*) CDataStatic::SearchSkill( mSkillList[ i ].GetSkillID(), tSkillLevel );	
		if ( tpSkillTempl == NULL )
		{
			LOG_ERROR( "pro", "[%s:%d] can't find skill id %d, level %d", __FUNCTION__, __LINE__, mSkillList[ i ].GetSkillID(), tSkillLevel );
			continue;																					
		}

		// TODO: 除非策划2，一般不会配4个funcid一样的buff, 暂时用if .. else if .. else if ...
		if ( tpSkillTempl->mSkillFuncID1 == vType )							
			tValue += tpSkillTempl->mParameter1[ 0 ];											
		else if ( tpSkillTempl->mSkillFuncID2 == vType )							
			tValue += tpSkillTempl->mParameter2[ 0 ];													
		else if ( tpSkillTempl->mSkillFuncID3 == vType )							
			tValue += tpSkillTempl->mParameter3[ 0 ];													
		else if ( tpSkillTempl->mSkillFuncID4 == vType )							
			tValue += tpSkillTempl->mParameter4[ 0 ];															
	}

	return tValue;
}


template< int SIZE >
int CSkillListInstance<SIZE>::GetPerNumber(CTemplateSkill::PASSINC_TYPE vType )
{
	int tValue = 0;

	for( int i = 0; i < mSkillNumber; i++ )
	{														
		int tSkillLevel = mSkillList[ i ].GetSkillLevel();		
		CTemplateSkill* tpSkillTempl = (CTemplateSkill*) CDataStatic::SearchSkill( mSkillList[ i ].GetSkillID(), tSkillLevel );	
		if ( tpSkillTempl == NULL )																	
		{
			LOG_ERROR( "pro", "[%s:%d] can't find skill id %d, level %d", __FUNCTION__, __LINE__, mSkillList[ i ].GetSkillID(), tSkillLevel );
			continue;																					
		}

		// TODO: 除非策划2，一般不会配4个funcid一样的buff, 暂时用if .. else if .. else if ...
		if ( tpSkillTempl->mSkillFuncID1 == vType )							
			tValue += tpSkillTempl->mParameter1[ 0 ];													
		else if ( tpSkillTempl->mSkillFuncID2 == vType )							
			tValue += tpSkillTempl->mParameter2[ 0 ];													
		else if ( tpSkillTempl->mSkillFuncID3 == vType )							
			tValue += tpSkillTempl->mParameter3[ 0 ];													
		else if ( tpSkillTempl->mSkillFuncID4 == vType )							
			tValue += tpSkillTempl->mParameter4[ 0 ];															
	}

	return tValue;
}


template< int SIZE >
int CSkillListInstance<SIZE>::GetValueByID(  unsigned int vSkillID, CTemplateSkill::PASSINC_TYPE vType )
{
	unsigned int tValue = 0;
	for( int i = 0; i < mSkillNumber; i++ )
	{	
		int tSkillLevel = mSkillList[ i ].GetSkillLevel();		
		CTemplateSkill* tpSkillTempl = (CTemplateSkill*) CDataStatic::SearchSkill( mSkillList[ i ].GetSkillID(), tSkillLevel );	
		if ( tpSkillTempl == NULL )
		{
			LOG_ERROR( "pro", "[%s:%d] can't find skill id %d, level %d", __FUNCTION__, __LINE__, mSkillList[ i ].GetSkillID(), tSkillLevel );
			continue;																					
		}

		if ( tpSkillTempl->mSkillFuncID1 == vType )	
		{
			if ( vSkillID == tpSkillTempl->mParameter1[ 0 ] )
				tValue += tpSkillTempl->mParameter1[ 1 ];											
		}
		else if ( tpSkillTempl->mSkillFuncID2 == vType )							
		{
			if ( vSkillID == tpSkillTempl->mParameter2[ 0 ] )
				tValue += tpSkillTempl->mParameter2[ 1 ];													
		}
		else if ( tpSkillTempl->mSkillFuncID3 == vType )							
		{
			if ( vSkillID == tpSkillTempl->mParameter3[ 0 ] )
				tValue += tpSkillTempl->mParameter3[ 1 ];													
		}
		else if ( tpSkillTempl->mSkillFuncID4 == vType )							
		{
			if ( vSkillID == tpSkillTempl->mParameter4[ 0 ] )
				tValue += tpSkillTempl->mParameter4[ 1 ];															
		}
	}

	return tValue;
}


template< int SIZE >
int	CSkillListInstance<SIZE>::GetStateID(StateList& vBuffList, CTemplateSkill::PASSINC_TYPE vType )
{
	for( int i = 0; i < mSkillNumber; i++ )
	{	
		int tSkillLevel = mSkillList[ i ].GetSkillLevel();		
		CTemplateSkill* tpSkillTempl = (CTemplateSkill*) CDataStatic::SearchSkill( mSkillList[ i ].GetSkillID(), tSkillLevel );	
		if ( tpSkillTempl == NULL )
		{
			LOG_ERROR( "pro", "[%s:%d] can't find skill id %d, level %d", __FUNCTION__, __LINE__, mSkillList[ i ].GetSkillID(), tSkillLevel );
			continue;																					
		}

		if ( tpSkillTempl->mSkillFuncID1 == vType )	
		{
			BuffState tBuff;
			tBuff.mBuffID = tpSkillTempl->mParameter1[ 0 ];
			tBuff.mRand = tpSkillTempl->mParameter1[ 1 ];
			tBuff.mTime = tpSkillTempl->mParameter1[ 2 ];	
			tBuff.mTarget = tpSkillTempl->mParameter1[ 3 ];  // 潜规则(下同)： mTarget和mHP只会同时存在一个，目前张3
			tBuff.mHP = tpSkillTempl->mParameter1[ 3 ];		// 都用Paramter[3]来赋值，后来张3扩充了一下Paramter到5，但是策划模板还一直未改
			vBuffList.push_back( tBuff );
		}
		else if ( tpSkillTempl->mSkillFuncID2 == vType )							
		{
			BuffState tBuff;
			tBuff.mBuffID = tpSkillTempl->mParameter2[ 0 ];
			tBuff.mRand = tpSkillTempl->mParameter2[ 1 ];
			tBuff.mTime = tpSkillTempl->mParameter2[ 2 ];
			tBuff.mTarget = tpSkillTempl->mParameter2[ 3 ];
			tBuff.mHP = tpSkillTempl->mParameter2[ 3 ];
			vBuffList.push_back( tBuff );
		}
		else if ( tpSkillTempl->mSkillFuncID3 == vType )							
		{
			BuffState tBuff;
			tBuff.mBuffID = tpSkillTempl->mParameter3[ 0 ];
			tBuff.mRand = tpSkillTempl->mParameter3[ 1 ];
			tBuff.mTime = tpSkillTempl->mParameter3[ 2 ];
			tBuff.mTarget = tpSkillTempl->mParameter3[ 3 ];
			tBuff.mHP = tpSkillTempl->mParameter3[ 3 ];
			vBuffList.push_back( tBuff );
		}
		else if ( tpSkillTempl->mSkillFuncID4 == vType )							
		{
			BuffState tBuff;
			tBuff.mBuffID = tpSkillTempl->mParameter4[ 0 ];
			tBuff.mRand = tpSkillTempl->mParameter4[ 1 ];
			tBuff.mTime = tpSkillTempl->mParameter4[ 2 ];
			tBuff.mTarget = tpSkillTempl->mParameter4[ 3 ];
			tBuff.mHP = tpSkillTempl->mParameter4[ 3 ];
			vBuffList.push_back( tBuff );
		}
	}

	return 0;
}

template< int SIZE >
void CSkillListInstance<SIZE>::ClearCooldown( )
{
	for( int i = 0; i < mSkillNumber; i++ )
	{														
		mSkillList[ i ].SetCooldown( 0 );														
	}
}

template< int SIZE >
int CSkillListInstance<SIZE>::PartClean( )
{
	int tIDList[ SIZE ] = { 0 };
	int tLevelList[ SIZE ] = { 0 };
	int tNumber = 0;
	int tValue = 0;
	for( int i = 0; i < mSkillNumber; i++ )
	{	
		int tSkillLevel = mSkillList[ i ].GetSkillLevel();		
		CTemplateSkill* tpSkillTempl = (CTemplateSkill*) CDataStatic::SearchSkill( mSkillList[ i ].GetSkillID(), tSkillLevel );	
		if ( tpSkillTempl == NULL )
		{
			LOG_ERROR( "pro", "[%s:%d] can't find skill id %d, level %d", __FUNCTION__, __LINE__, mSkillList[ i ].GetSkillID(), tSkillLevel );
			continue;																					
		}
		if ( tpSkillTempl->mCanClean != 0 )
		{
			tIDList[ tNumber ] = mSkillList[ i ].GetSkillID();
			tLevelList[ tNumber ] = mSkillList[ i ].GetSkillLevel();
			tNumber++;
		}
		else
		{
			tValue += mSkillList[ i ].GetSkillLevel();
		}
	}
	Clean( );
	for ( int i = 0; i < tNumber; i++ )
	{
		for ( int j = 0; j < tLevelList[ i ]; j++ )
		{
			InsertSkill( tIDList[ i ], tLevelList[ i ], 1 );
		}
	}
	return tValue;
}



// 刷新技能CD
template< int SIZE >
void CSkillListInstance<SIZE>::RefreshCooldown( )
{
	int nLeftSec = 0;
	int nLeftMillSec = 0;

	for( int i = 0; i < mSkillNumber; i++ )
	{														
		if( mSkillList[ i ].GetCDTimer() != INVALID_OBJ_ID )
		{
			int nGetTimerRet = CTimerMng::GetSingletonPtr()->GetLeftTime( mSkillList[ i ].GetCDTimer(), nLeftSec, nLeftMillSec );
			if( nGetTimerRet == 0)
			{
				mSkillList[ i ].SetCooldown( nLeftMillSec );
			}
		}
	}

}



#endif // _SKILL_LIST_H_