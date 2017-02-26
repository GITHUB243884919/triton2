#include "skill.h"
#include "entity.h"

int	CSkillObject::mCommonCDTimerID  = 0;

int CSkillObject::Initialize( )
{
	mSkillID = 0;
	mSkillLevel = 0;
	mCooldown = 0;
	mIncLevel = 0;

	mReduceCoolFix = 0;
	mDeathRateFix = 0;
	mDeathValueFix = 0;

	mCDTimerID = INVALID_OBJ_ID;
	mOriginalCooldown = 0;

	ClearContiunePara();
	return 0;
}


// 得到技能攻击距离
unsigned int CSkillObject::GetAttackDistance()
{
	CTemplateSkill* tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( mSkillID, mSkillLevel );
	LK_ASSERT ( tpSkillTpl != NULL, return 0 );

	// 无限远
	const unsigned int cunLimited = (unsigned int)(-1);

	switch( tpSkillTpl->mSkillTemplateType )
	{
	case CTemplateSkill::SKILL_TEMPLATE_TYPE_AUREOLE: // 光环技能
		{
			return cunLimited;
		}
	case CTemplateSkill::SKILL_TEMPLATE_TYPE_NORMAL: // 普通技能
		{
			CTemplateNormalSkill* tpNormalSkill = (CTemplateNormalSkill*)tpSkillTpl;
			return tpNormalSkill->mMinDistance;
		}
	case CTemplateSkill::SKILL_TEMPLATE_TYPE_TRIP: // 陷阱技能
		{
			return cunLimited;
		}
	case CTemplateSkill::SKILL_TEMPLATE_TYPE_SUMMONBEAST: // 召唤兽技能
		{
			return cunLimited;
		}
	default:
		{
			LOG_ERROR( "pro", "[%s:%d]wrong type %d", __FUNCTION__, __LINE__, tpSkillTpl->mSkillTemplateType );
			return 0;
		}
	}

}

CTemplateSkill::SKILL_TYPE CSkillObject::GetAttackType()
{
	CTemplateSkill* tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( mSkillID, mSkillLevel );
	LK_ASSERT ( tpSkillTpl != NULL, return CTemplateSkill::SKILLTYPE_ATTACK );

	return tpSkillTpl->mSkillType;
}


bool CSkillObject::IsSingeTargetSkill()
{
	CTemplateSkill* tpSkillTpl = (CTemplateSkill*) CDataStatic::SearchSkill( mSkillID, mSkillLevel );
	if( tpSkillTpl->mSkillTemplateType == CTemplateSkill::SKILL_TEMPLATE_TYPE_NORMAL )
	{
		CTemplateNormalSkill* tpNormalSkill = (CTemplateNormalSkill*) tpSkillTpl;
		// 如果只能对单体使用
		return ( tpNormalSkill->mAttackRange == CTemplateNormalSkill::RANGE_SINGLE );
	}

	return false;
}

// 得到技能模板(包括增加的技能等级)
CTemplateSkill* CSkillObject::GetSkillTpl( )
{
	return (CTemplateSkill*) CDataStatic::SearchSkill( mSkillID, mSkillLevel );
}

// 得到原始技能模板(不包括增加的技能等级)
CTemplateSkill* CSkillObject::GetOriginalSkillTpl( )
{
	return (CTemplateSkill*) CDataStatic::SearchSkill( mSkillID, mSkillLevel - mIncLevel );
}

// 持续性群攻技能
bool CSkillObject::SkillContinue( )
{
	CTemplateNormalSkill* pNormalSkill = (CTemplateNormalSkill*) GetSkillTpl( );
	LK_ASSERT( pNormalSkill != NULL, return false );

	// 作用状态
	if( mContinuedState != SKILL_STATE_START )
	{
		return false;
	}

	// 作用次数
	if( (int)(mContinuedNum) > pNormalSkill->mContinuedNum )
	{
		return false;
	}

	// 上次作用时间
	if( LINEKONG::GetTickCount() - mContinuedLastTime < (unsigned int)pNormalSkill->mContinuedInterval )
	{
		return false;
	}

	// 作用时间重置
	mContinuedLastTime = LINEKONG::GetTickCount();

	// 作用次数重置
	if( (int)( ++mContinuedNum ) > pNormalSkill->mContinuedNum )
	{
		ClearContiunePara();
	}


	return true;
}

// 是否是持续性技能
bool CSkillObject::IsContinueSkill()
{
	CTemplateSkill::SKILL_TYPE tType = GetAttackType();

	return ( tType == CTemplateSkill::SKILLTYPE_CONTINUEDATTACK ) 
		|| (tType == CTemplateSkill::SKILLTYPE_CONTINUEDATTACK) 
		|| (tType == CTemplateSkill::SKILLTYPE_CONTINUEDATTACK);
}

// 让持续性技能开始作用
void CSkillObject::StartContinueSkill()
{
	// 在下一次使用该技能时重置一下状态和参数，防止因为持续性技能上一次没有正常结束，导致的状态问题。
	ClearContiunePara();
	mContinuedState = SKILL_STATE_START;
}

// 让持续性技能停止作用
void CSkillObject::StopContinueSkill()
{
	mContinuedState = SKILL_STATE_STOP;
	mContinuedLastTime = time(NULL);		
	mContinuedNum = 0;
}

// 清除持续性技能动态参数
void CSkillObject::ClearContiunePara()
{
	mContinuedState = SKILL_STATE_STOP;			
	mContinuedLastTime = time(NULL);		
	mContinuedNum = 0;			
}

