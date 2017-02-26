#include "servertool.h"
#include "protype.h"
#include "tinyxml.h"
#include "servercore.h"
#include "promessage_pb.hxx.pb.h"
#include "scenelogic.h"
// 函数说明：从输入的一个概率数组中根据概率随机抽取，返回数组索引
// 注意：概率之和要等于10000
// 参数：数组指针，数量
int GetRand( int arr[], int num )
{
	int	tRand		= RAND(SERVER_PERCENT_INT);
	int	tLowRan		= SERVER_PERCENT_INT;
	int	tHighRan	= SERVER_PERCENT_INT;

	tLowRan -= arr[num - 1];
	if ( tRand > tLowRan && tRand < tHighRan )
		return num - 1;

	for ( int j = num - 1; j >= 1 ; j -- )
	{
		tLowRan		-= ( j == 1 ) ? tLowRan : arr[ j - 1 ];
		tHighRan	-= arr[ j ] ;
		if ( tRand > tLowRan && tRand < tHighRan )
		{
			return j - 1;
			break;
		}
	}
	return 0;
}

void CClientControlData::LoadFromXml( const char* pFileName )
{
	m_Data.clear();
	TiXmlDocument tTplDoc;
	tTplDoc.LoadFile( pFileName );
	TiXmlElement* tpRoot = tTplDoc.FirstChildElement("SystemParamList");
	if( tpRoot == NULL)
	{
		LOG_ERROR( "default", "in SystemParamList, No Root Element Found" );
		return ;
	}

	TiXmlElement* tpElement = tpRoot->FirstChildElement( );
	for( ; tpElement != NULL; tpElement = tpElement->NextSiblingElement( ) )
	{
		std::string tEleName = tpElement->Value( );
		if ( tEleName != "SystemParam" )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] SystemParamList Err, First Element isn't SystemParam", __LK_FILE__,
				__LINE__, __FUNCTION__ );
			continue;
		}

		const char* tpFuncID	= tpElement->Attribute( "FunctionID" );
		const char* tpOnOff		= tpElement->Attribute( "OnOFF" );
		const char* tpParam1	= tpElement->Attribute( "Param1" );
		const char* tpParam2	= tpElement->Attribute( "Param2" );
		const char* tpParam3	= tpElement->Attribute( "Param3" );

		if (  tpFuncID == NULL || tpOnOff == NULL || tpParam1 == NULL || tpParam2 == NULL || tpParam3 == NULL )
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ]SystemParam Err2", __LK_FILE__, 
				__LINE__, __FUNCTION__ );

			continue;
		}

		SingleControlData _data;
		_data.nFunID = ::atoi(tpFuncID);
		_data.nOnOFF = ::atoi(tpOnOff);
		_data.nParam1= ::atoi(tpParam1);
		_data.nParam2= ::atoi(tpParam2);
		strncpy( _data.nParam3, tpParam3, CLI_CTRL_PARM3_LEN-1 );
		m_Data.push_back( _data );
	}
	Send2AllPlayers( );
}

void CClientControlData::FillMessage( CMessageClientRunTimeParamNotify* pMsg )
{
	if (pMsg==NULL)
		return;

	for (CLIENT_CONTROL_TYPE::iterator it = m_Data.begin(); it != m_Data.end(); ++it)
	{
		PBRuntimeParam* tpSingle = pMsg->add_functions();
		tpSingle->set_funid(it->nFunID);
		tpSingle->set_onoff(it->nOnOFF);
		tpSingle->set_param1(it->nParam1);
		tpSingle->set_param2(it->nParam2);
		tpSingle->set_param3(it->nParam3);
	}
}

// 把系统配置参数广播给本服务器所有玩家
void CClientControlData::Send2AllPlayers( )
{
	CMessageClientRunTimeParamNotify msg;
	FillMessage(&msg);
	CMessage tMessage;
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_RUNTIMEPARAM );
	tMessage.set_msgpara( (long) &msg );

	// 遍历玩家
	std::vector<CEntityPlayer*> tPlayers;
	CSceneLogic::GetSingleton().GetPlayersInScene( tPlayers );
	for ( std::vector<CEntityPlayer*>::iterator it = tPlayers.begin();
		it != tPlayers.end(); ++it )
	{
		CSceneLogic::GetSingleton().Send2Player( *it, &tMessage );
	}
}


int GetYYYYMMDD(time_t nTm)
{
	struct tm* tTm = localtime( &nTm );
	LK_ASSERT( tTm != NULL, return 0);
	return (1900+tTm->tm_year)*10000 + (1+tTm->tm_mon)*100 + tTm->tm_mday;
}
