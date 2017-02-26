#include "stdafx.hpp"
#include "event.h"
#include "servercore.h"
#include "property.h"
#include "entity.h"
#include "propertymodule.h"
#include "mapmodule.h"
#include "taskmodule.h"

//int CEventHandler::OnEvent( CEvent vEvent )
//{
//	switch( vEvent.mEventID )
//	{
//	case EVENT_DROPITEM:
//		{
//			CEntity* tOwnerEntity = (CEntity*)vEvent.mParam1;
//			CEntity* tDesEntity = (CEntity*)vEvent.mParam2;
//			CTemplateOgre* tTempOgre = (CTemplateOgre*)vEvent.mParam3;
//			CPropertyModule::GetSingletonPtr()->DropItemByNpcDie( tOwnerEntity, tDesEntity, tTempOgre );
//			break;
//		}
//	default:
//		break;
//	}
//
//	return 0;
//}