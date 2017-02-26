#include "stdafx.hpp"
#include "entity.h"
#include "entity_searcher.h"
#include "sceneobj_manager.h"
#include "propertymodule.h"

bool AttackableEntityChecker::operator ()(CEntity *pTarget)
{
	if ( pTarget->IsAlive() == false )
	{
		return false;
	}
	return CPropertyModule::GetSingletonPtr()->EntityPKValid( m_pSrcEntity, pTarget, NULL, false );

}



