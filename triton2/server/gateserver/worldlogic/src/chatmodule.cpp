#include "fcmservice.h"
#include "gateobj_manager.h"
#include "gatelogic.h"
#include "servermessage_in.h"
#include "nameservice.h"
#include "chatmodule.h"
#include "chatmessage_pb.hxx.pb.h"
#include "chattype.h"
#include "core_service.h"
#include "family_manager.h"
#include "corpsmanager.h"
#include "protype.h"
#include "gateteammanager.h"
#include "log_tools.h"
#include "dirtyword.h"

using namespace GateServer;	
template<> CChatmodule* CSingleton< CChatmodule >::spSingleton = NULL;

void CChatmodule::OnLaunchServer()
{  

}

void CChatmodule::OnExitServer()
{

}

void CChatmodule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{	
	
}



unsigned int CChatmodule::CountSize()
{
	unsigned int tSize = sizeof( CChatmodule );	
	return tSize;
}


CChatmodule::CChatmodule()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )		
	{
		Initialize();
	}else
	{
		Resume();
	}
}

CChatmodule::~CChatmodule()
{

}

int CChatmodule::Initialize()
{
	
	return 0;
}

int CChatmodule::Resume()
{
	return 0;
}

void* CChatmodule::operator new( size_t size )    
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CChatmodule::operator delete( void* pointer )
{

}




