#ifndef _SKILL_H_
#define _SKILL_H_

#include "template.h"
#include "servertool.h"

// ********************************************************************** //
// CSkillObject
// ********************************************************************** //
class CSkillObject 
{
private:

	// 需要存盘的技能数据
	unsigned int	mSkillID;		// 技能ID
	unsigned int	mSkillLevel;	// 技能等级
	int				mCooldown;		// 技能冷却时间, 0表示目前技能可以使用
	unsigned int	mIncLevel;		// 增加的技能等级

	// 以下是动态数据，无需存盘
	enum 
	{
		SKILL_STATE_START,		// 技能正在作用
		SKILL_STATE_STOP,		// 技能停止
	};


	char			mContinuedState;	// 持续类技能状态量 ( 用于打断判断 )
	char			mContinuedNum;		// 持续作用次数
	unsigned int	mContinuedLastTime;	// 上一次作用时间
	int				mCDTimerID;			// 技能冷却定时器ID
	int				mReduceCoolFix;		// 冷却时间减少
	int				mDeathRateFix;		// 暴击率增加
	int				mDeathValueFix;		// 暴击伤害增加
	int				mOriginalCooldown;	// 原始冷却时间( 从模板读入，为了节省查询时间 )

public:

	static int		mCommonCDTimerID;		// 公共冷却定时器ID


public:
	CSkillObject( unsigned short vSkillID , unsigned short vSkillLevel ) : mSkillID( vSkillID ), mSkillLevel( vSkillLevel ), mCooldown( 0 ),
		mReduceCoolFix( 0 ), mDeathRateFix( 0 ), mDeathValueFix( 0 ), mOriginalCooldown( 0 )
	{
	}

	CSkillObject( )
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
	~CSkillObject( ) {}

	int Initialize( );

	int Resume( )
	{
		return 0;
	}

	// Get & Set SkillID
	inline unsigned int GetSkillID() const { return mSkillID; }
	inline void SetSkillID(unsigned int val) { mSkillID = val; }

	// Get & Set SkillLevel
	inline unsigned int GetSkillLevel() const { return mSkillLevel; }
	inline void SetSkillLevel(unsigned int val) { mSkillLevel = val; }

	// Geet & Set Cooldown
	inline int GetCooldown() const { return mCooldown ; }
	inline void SetCooldown(int val) { mCooldown = val; }

	// Get & Set IncLevel
	inline unsigned int GetIncLevel() const { return mIncLevel; }
	inline void SetIncLevel(unsigned int val) { mIncLevel = val; }

	// Get & Set CDTimer
	inline int GetCDTimer( ) const { return mCDTimerID; }
	inline void SetCDTimer( int nTimer ) { mCDTimerID = nTimer; }

	// Get & Set CommonCDTimer
	static int GetCommonCDTimer( )  {return mCommonCDTimerID;}
	static void SetCommonCDTimer( int nTimer ) { mCommonCDTimerID = nTimer ;}

	// Get & Set ReduceCoolFix
	inline int GetReduceCoolFix( ) const { return mReduceCoolFix; }
	inline void SetReduceCoolFix( int nCoolFix ) { mReduceCoolFix = nCoolFix; }

	// Get & Set DeathRateFix
	inline int GetDeathRateFix( ) const { return mDeathRateFix; }
	inline void SetDeathRateFix( int nDeathRate ) { mDeathRateFix = nDeathRate; }

	// Get & Set DeathValueFix
	inline int GetDeathValueFix( ) const { return mDeathValueFix; }
	inline void SetDeathValueFix( int nDeathValue ) { mDeathValueFix = nDeathValue; }

	// Get & Set OriginalCooldown
	inline int GetOriginalCooldown( ) const { return mOriginalCooldown; }
	inline void SetOriginalCooldown( int nOriCooldown ) { mOriginalCooldown = nOriCooldown; }

	// 得到技能攻击距离
	unsigned int GetAttackDistance();

	// 得到技能的作用类型
	CTemplateSkill::SKILL_TYPE GetAttackType();

	// 是否是单体作用技能
	bool IsSingeTargetSkill();

	// 是否是持续性技能
	bool IsContinueSkill();

	// 得到技能模板(包括增加的技能等级)
	CTemplateSkill* GetSkillTpl( );

	// 得到原始技能模板(不包括增加的技能等级)
	CTemplateSkill* GetOriginalSkillTpl( );

	// 持续性群攻技能
	bool SkillContinue( );

	// 清除持续性技能动态参数
	void ClearContiunePara();

	// 让持续性技能开始作用
	void StartContinueSkill();

	// 让持续性技能停止作用
	void StopContinueSkill();

public:

	bool IsCoolingDown() { return mCooldown > 0 ; }

	bool IsCommonCDSkill( )
	{
		CTemplateSkill* tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( mSkillID, mSkillLevel );
		if( tpSkillTpl == NULL )
		{
			return NULL;
		}
		// 受公共冷却影响
		return  ( tpSkillTpl->mAutoCoolingSkill == 0 );
	}
};



#endif // _SKILL_H_