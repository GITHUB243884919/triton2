#include "google/protobuf/text_format.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/unknown_field_set.h"
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/stubs/strutil.h"

#include "gm_command.h"
#include "gm_service.h"
#include "gmmessage_pb.hxx.pb.h"
#include "message_factory.h"
#include "servermessage_in.h"
#include "db_msg_pb.hxx.pb.h"
#include "property_pb.hxx.pb.h"
#include "template.h"
#include <iconv.h>
#include "logservertype.h"
#include "template_pb.hxx.pb.h"
#include "loginobj_manager.h"
#include "mailmessage_pb.hxx.pb.h"

#include <list>

using namespace std;
typedef std::list< const char* > CTokenList;

enum 
{
	EM_DEFAULT_SESSIONID = 0,
};

#define LABEL(a, b) string(CGmServer::GetSingleton().Label((a),(b))).c_str()
#define EXLABEL(a, b, c, d) string(CGmServer::GetSingleton().Label((a),(b), (&c), (&d))).c_str()
#define TOKEN2STRING(a) CGmServer::GetSingleton().Token2String((a))
#define DOMAIN(pDomain, pValue) (pDomain ? LABEL(pDomain, atoi(pValue)) : pValue)

typedef ::google::protobuf::Descriptor Descriptor;
typedef ::google::protobuf::Reflection Reflection;
typedef ::google::protobuf::FieldDescriptor FieldDescriptor;

Message* CreateGmMessage(const char* szCmdName, int nMessageID, CTokenList& params );

// 装备详细信息
int EquipDetail(const PBItemObject& pbItem, const CTplItem* ptpl, C2KString& desc);
int EquipSimple(const PBItemObject& pbItem, const CTplItem* ptpl, C2KString& desc);


const char* ItemID2Name(int nItemID)
{          
	static char szTplName[48];
	sprintf(szTplName, "#t_tpl_%d_1", nItemID);

	return TOKEN2STRING(szTplName);
}

const char* TaskID2Name(int nTaskID)
{
	static char sTaskTplName[48];
	sprintf(sTaskTplName, "#td_%d_name", nTaskID);

	return TOKEN2STRING(sTaskTplName);
}

const char* NpcID2Name(int nNpcID)
{
	CTemplateOgre *pNpcTpl = (CTemplateOgre *) CDataStatic::SearchTpl(nNpcID);
	return pNpcTpl ? TOKEN2STRING(pNpcTpl->mName) : LABEL("EGlobeDefine", "UNKNOWN_NAME");
}

const char* FromUnixTime(int nParam)
{
	static char szTime[64];

	time_t nSeconds = nParam; 
	struct tm* ptmnow = localtime( &nSeconds);
	strftime(szTime, sizeof(szTime) -1, "%F %T", ptmnow);

	return szTime;
}

void escape4xml(const char* pins, C2KString& out)
{
	for (; pins != NULL && *pins != 0; ++ pins)
	{
		if (iscntrl(*pins))
		{
			out.append("&#%d;", *pins);
		}else
		{
			out.append("%c", *pins);
		}
	}
}

const char* ID2ElemLevel(int nID) { return LABEL("EElemLevel", nID); }
const char* ID2EquipAttr(int nID) { return ItemID2Name(nID); }
const char* ID2EquipResist(int nID) { return LABEL("EEquipResist", nID); }
const char* ID2EquipAttack(int nID) { return LABEL("EEquipAttack", nID);  }
const char* ID2BindStatus(int nID) { return LABEL("EBindStatus", nID); }
const char* ID2JudgeResult(int nID) 
{ 
	int n = nID / 1000000;
	int a = ( nID / 10000 ) % 100;
	int b = ( nID % 10000 ) / 100;
	int c = nID % 100 ;

	static lk::CString128 res("");
	res.clear();


	if ( n== 1) 
	{
		return LABEL("EJudgeResult", a); 
	}

	if ( n == 2)
	{
		res.append("%s,",  LABEL("EJudgeResult", a));
		res.append("%s", LABEL("EJudgeResult", b)); 

		return res.c_str();
	}

	if ( n == 3)
	{
		res.append("%s,",  LABEL("EJudgeResult", a));
		res.append("%s,",  LABEL("EJudgeResult", b));
		res.append("%s", LABEL("EJudgeResult", c)); 

		return res.c_str();
	}

	return NULL;
}

const char* MoneyType2Name(int nMoneyType)
{
	return LABEL("EMoneyUnit", nMoneyType);
}

const char* MapID2Name(int nMapID)
{
	return LABEL("EMapName", nMapID);
}

const char* FormatMoney(int nMoney)
{
	static char szMoney[64];
	sprintf(szMoney, "%.4f", nMoney/10000.0);

	return szMoney;
}

void Aformat(CMaxString& generator, const char* proto_name, const char* field_name, const char* field_value)
{
	const char* pDomain = NULL;
	int nFormat = 0;

	const char* pKey = EXLABEL(proto_name, field_name, pDomain, nFormat);

	switch(nFormat)
	{
		// 时间格式
		case 1:
		{
			char szTime[64] = {0};
			time_t now = atoi(field_value);
			struct tm* ptmnow = localtime(&now);
			strftime(szTime, sizeof(szTime) -1, "%F %T", ptmnow);
			generator.append("<info name=\"%s\">%s</info>\n", pKey, szTime);
			break;
		}

		// 金钱格式
		case 3:
		{
			generator.append("<info name=\"%s\">%.4f</info>\n", pKey, atoi(field_value)/10000.0);
			break;
		}

		// 时长格式
		case 4:
		{
			generator.append("<info name=\"%s\">%d Hours,%d Minutes, %d Seconds</info>\n", 
					pKey, atoi(field_value)/3600, (atoi(field_value)%3600)/60, atoi(field_value)%60);
			break;
		}

		case 2: // 标准计数
		default:
		{
			generator.append("<info name=\"%s\">%s</info>\n", pKey, DOMAIN(pDomain, field_value));
			break;
		}
	}
}

void Bformat(CMaxString& generator, const char* proto_name, const char* field_name, const char* field_value)
{
	const char * pDomain; 
	int nFormat = 0; 
	const char *pKey = EXLABEL(proto_name, field_name, pDomain, nFormat); 

	switch(nFormat)
	{
		case 1:
		{
			char szTime[64] = {0};
			time_t now = atoi(field_value);
			struct tm* ptmnow = localtime(&now);
			strftime(szTime, sizeof(szTime) -1, "%F %T", ptmnow);
			generator.append("<info name=\"%s\" text=\"%s\">%s</info>\n", field_name,  pKey,  szTime);
			break;
		}

		// 金钱格式
		case 3:
		{
			generator.append("<info name=\"%s\" text=\"%s\">%.4f</info>\n", 
					field_name, pKey, atoi(field_value)/10000.0);
			break;
		}

		// 时长格式
		case 4:
		{
			generator.append("<info name=\"%s\" text=\"%s\"> %d Hours,%d Minutes, %d Seconds </info>\n", 
					field_name, pKey, atoi(field_value)/3600, (atoi(field_value)%3600)/60, atoi(field_value)%60);
			break;
		}

		default:
		{
			generator.append("<info name=\"%s\" text=\"%s\">%s</info>\n", 
					field_name,  pKey,  DOMAIN(pDomain, field_value));
			break;
		}
	}
}

void Cformat(CMaxString& generator, const char* proto_name, const char* field_name, const char* field_value)
{
	const char * pDomain; 
	int nFormat = 0; 
	const char *pKey = EXLABEL(proto_name, field_name, pDomain, nFormat); 

	switch(nFormat)
	{
		case 1:
		{
			char szTime[64] = {0};
			time_t now = atoi(field_value);
			struct tm* ptmnow = localtime(&now);
			strftime(szTime, sizeof(szTime) -1, "%F %T", ptmnow);
			generator.append("%s,%s,%s", field_name,  pKey,  szTime);
			break;
		}

		// 金钱格式
		case 3:
		{
			generator.append("%s,%s,%s", field_name, pKey, atoi(field_value)/10000.0);
			break;
		}

		// 时长格式
		case 4:
		{
			generator.append("%s,%s,%d Hours %d Minutes %d Seconds", 
					field_name, pKey, atoi(field_value)/3600, (atoi(field_value)%3600)/60, atoi(field_value)%60);
			break;
		}

		default:
		{
			generator.append("%s,%s,%s", field_name,  pKey,  DOMAIN(pDomain, field_value));
			break;
		}
	}
}

void Dformat(CMaxString& generator, const char* proto_name, const char* field_name, const char* field_value)
{
	const char * pDomain; 
	int nFormat = 0; 
	const char *pKey = EXLABEL(proto_name, field_name, pDomain, nFormat); 

	switch(nFormat)
	{
		case 1:
		{
			char szTime[64] = {0};
			time_t now = atoi(field_value);
			struct tm* ptmnow = localtime(&now);
			strftime(szTime, sizeof(szTime) -1, "%F %T", ptmnow);
			generator.append("<info>%s,%s,%s</info>\n", field_name,  pKey,  szTime);
			break;
		}

		// 金钱格式
		case 3:
		{
			generator.append("<info>%s,%s,%s</info>\n", field_name, pKey, atoi(field_value)/10000.0);
			break;
		}

		// 时长格式
		case 4:
		{
			generator.append("<info>%s,%s,%d Hours %d Minutes %d Seconds</info>\n", 
					field_name, pKey, atoi(field_value)/3600, (atoi(field_value)%3600)/60, atoi(field_value)%60);
			break;
		}

		default:
		{
			generator.append("<info>%s,%s,%s</info>\n", field_name,  pKey,  DOMAIN(pDomain, field_value));
			break;
		}
	}
}

void print_field(const Message& message, 
		const Reflection* pref, const FieldDescriptor* pfldes, CMaxString& generator, 
		void (*pformat)(CMaxString&, const char*, const char*, const char*));

void print_fieldvalue(const Message& message, 
		const Reflection* reflection, const FieldDescriptor* field, int index, CMaxString& generator, 
		void (*pformat)(CMaxString&, const char*, const char*, const char*));

int code_convert(const char* from_charset, const char* to_charset, char* inbuf, size_t inlen, char* outbuf, size_t* poutlen)
{  
	iconv_t   cd; 
	//int rc;
	char **pin = &inbuf;
	char **pout = &outbuf; 

	cd = iconv_open(to_charset, from_charset);  
	if   (cd==0)   return   -1; 

	memset(outbuf, 0, *poutlen);  

	if( iconv(cd, pin, &inlen, pout, poutlen) == (size_t) -1 ) return -1;  

	iconv_close(cd);

	return   0;  
}

int DecodeFromGMStream(int nCommHandle, char* szStream, CMessage* tpMessage)
{
	char* pcurr = szStream;

	const char* szGmCmdName = NULL;
	int   nSessionID = EM_DEFAULT_SESSIONID;

	int content_flag = 0;

	CTokenList all;
	for(char* pprev = NULL; *pcurr != 0; pprev=pcurr++)
	{
		switch(*pcurr)
		{
			case '\n': //new line
			{
				#ifdef LK_DEBUG
				printf("\n");
				for(CTokenList::const_iterator iter = all.begin(); iter != all.end(); ++iter)
				{
					printf("[%s]", *iter);
				}
				#endif

				CTokenList::const_iterator iter = all.begin();

				if(strcmp(*iter, "POST") == 0)
				{
					if (++iter != all.end())
					{
						szGmCmdName = *(iter);
					}
				}else if (strcmp(*iter, "Host") == 0)
				{

				}else if (strcmp(*iter, "Content-Length") == 0)
				{
					content_flag = 1;

				}else if (strcmp(*iter, "Session-ID") == 0)
				{
					if (++iter != all.end())
					{
						nSessionID = atoi(*iter);  				
					}
				}else
				{
					//no use.
				}

				*pcurr = 0;

				all.clear();
				break;
			}

			case '/':
			case '\t':
			case '=':
			case '\r':
			{
				*pcurr = 0;
				break;
			}

			case '&':
			{
				if (content_flag != 0) *pcurr = 0;
				break;
			}

			case ':':
			case ' ':
			{
				if (content_flag == 0) *pcurr = 0;
				break;
			}

			default:
			{
				if (pprev == NULL || *pprev == 0)
				{
					all.push_back(pcurr);
				}
			}
		}
	}

	char szSessionID[32+1]; 
	char szCommHandle[32+1];

	snprintf(szSessionID, 32, "%d", nSessionID);
	snprintf(szCommHandle, 32, "%d", nCommHandle);

	all.push_back("fd");
	all.push_back(szCommHandle);

	all.push_back("session_id");
	all.push_back(szSessionID);

	int nMessageID = CGmServer::GetSingleton().GetGmMessageID(szGmCmdName);
	if ( nMessageID < 0 )
	{
		return -1;
	}

	Message* pMessage = CreateGmMessage(szGmCmdName, nMessageID, all);
	if (pMessage == NULL)
	{
		return -2;		
	}

	pbmsg_sethead(tpMessage, nMessageID, 0, 0, 0, 0, 0, 0);	
	tpMessage->set_msgpara((uint32_t) pMessage);

	return 0;
}

Message* CreateGmMessage(const char* szCmdName, int nMessageID, CTokenList& params )
{
	if (nMessageID <= 0) return NULL;

	Message* pMessage = CMessageFactory::GetSingleton().CreateMessage(nMessageID);
	if (pMessage == NULL)
	{
		LOG_ERROR("gm", "Message Factory does not create message(%d:%s)!", nMessageID, szCmdName);
		return NULL;
	}

	const ::google::protobuf::Reflection *pReflection = pMessage->GetReflection();
	const ::google::protobuf::Descriptor *pDescriptor = pMessage->GetDescriptor();

	CTokenList::const_iterator iter = params.begin();
	for(; iter != params.end(); ++iter)
	{
		const char* key = *iter++;
		const char* value = (iter != params.end()) ? *iter : NULL; 

		if (key == NULL || value == NULL) 
		{
			LOG_ERROR("gm", "GmCmdStream is not complete On Msg(%s).", szCmdName);
			break;
		}

		const ::google::protobuf::FieldDescriptor *pFieldDef = pDescriptor->FindFieldByName(key);
		if ( pFieldDef == NULL)
		{
			LOG_ERROR("gm", "Message (%s) has no field (%s=%s), pls check protocol", szCmdName, key, value);
			continue;
		}

		//TODO, 杩欓噷涓嶅厑璁告秷鎭腑灞炴�ф槸repeated
		if (pFieldDef->is_repeated() ==  true)
		{
			LOG_ERROR("gm", "Messsage(%s) has repeated fileds(%s) that not be permited", szCmdName, key);
			return NULL;
		}

		switch(pFieldDef->type())
		{
			case ::google::protobuf::FieldDescriptor::TYPE_INT32:
			{
				pReflection->SetInt32(pMessage, pFieldDef, atoi(value));
				break;
			}

			case ::google::protobuf::FieldDescriptor::TYPE_UINT32:
			{
				pReflection->SetUInt32(pMessage, pFieldDef, (uint32_t)atoi(value));
				break;
			}

			case ::google::protobuf::FieldDescriptor::TYPE_UINT64:
			{
				pReflection->SetUInt64(pMessage, pFieldDef, (uint64_t)atoll(value));
				break;
			}

			case ::google::protobuf::FieldDescriptor::TYPE_STRING:
			{
				pReflection->SetString(pMessage, pFieldDef, value);
				break;
			}

			default:
			{
				LOG_ERROR("gm", "The Msg(%s) don't support field(%s) type(%d)", szCmdName, key, pFieldDef->type());
				return NULL;
			}
		}
	}

	LOG_INFO("gm", "create gm msg string(%s:%s)", szCmdName, pMessage->ShortDebugString().c_str()); 

	return pMessage;
}

int FlashToGMStream(CMessage* tpMessage, char* szStream, unsigned int unStreamLength, 
		int nSessionID, int(*pfflash)(CMessage*, CMaxString&))
{
	LK_ASSERT( tpMessage != NULL && tpMessage->has_msgpara() == true, return -1 );

	CMaxString xmlbuf("");
	int nBodyLength = pfflash(tpMessage, xmlbuf);

	if (nBodyLength < 0)
	{
		LOG_ERROR("gm", "The CMaxString Length is too short, pls expand it");
		return nBodyLength;
	}else
	{
		LOG_DEBUG("gm", "gbk code string: \%s", xmlbuf.c_str());
	}

	time_t now = time(0);
	struct tm* ptmnow = localtime(&now);

	//Tue, 16 Sep 2008 07:41:43 GMT
	char szNow [256];
	strftime(szNow, sizeof(szNow)-1, "%a, %d %b %Y %H:%M:%S GMT", ptmnow);

	int nResultLength = snprintf(szStream, unStreamLength-1, 
			CMD_RESPONSE_HEAD, szNow, MAX_HTML_LENGTH, nSessionID, xmlbuf.length(), xmlbuf.c_str()); 
	
	if (nResultLength < 0)
	{
		LOG_ERROR("gm", "The Length(%d) is too short, pls expand it", unStreamLength); 
		return nResultLength;
	}

	return nResultLength;
}

int FlashGmCmdNormalResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageCmdNormalResult* pGmCmdResult = (CGMMessageCmdNormalResult*) tpMessage->msgpara();

	xmlbuf.append(
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>%d</result>\n</Response>",
		pGmCmdResult->result()
		); 

	return xmlbuf.length();
}	

int FlashGmCmdSqlDmlResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	xmlbuf.append(
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>%d</result>\n</Response>",
		pSqlResult->resultcode()
		); 

	return xmlbuf.length();
}

int FlashGmCmdUserSocialInfoResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageUserSocialInfoResult *pproto = (CGMMessageUserSocialInfoResult*) tpMessage->msgpara();

	/*if ( !pproto->has_friends() )
	{
		xmlbuf.append(
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>%d</result>\n</Response>", 0); 
		return xmlbuf.length();
	}*/

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	xmlbuf.append(
			"<Response>\n"
			"<result>1</result>\n"
			);

	xmlbuf.append("<details_info>");
	if ( pproto->friends().friend_().playerid_size() > 0)
	{
		xmlbuf.append("<info name='friendlist' text='%s' >", LABEL( "SocialInfoType", "FriendList" ) );
		xmlbuf.append("%d", pproto->friends().friend_().playerid(0));

		for ( int i = 1; i < pproto->friends().friend_().playerid_size(); ++i)
		{
			xmlbuf.append(",%d", pproto->friends().friend_().playerid(i));
		}
		xmlbuf.append("</info>\n");
	}
	if ( pproto->has_spouseid() )
	{	
		xmlbuf.append("<info name='spouseid' text='%s' >", LABEL( "SocialInfoType", "SpouseID" ) );
		xmlbuf.append("%d",pproto->spouseid());
		xmlbuf.append("</info>\n");	
	}  
		
	if ( pproto->has_marriageid() )
	{	
		xmlbuf.append("<info name='marriageid' text='%s' >", LABEL( "SocialInfoType", "MarriageID" ));
		xmlbuf.append("%d",pproto->marriageid());
		xmlbuf.append("</info>\n");	
	} 
	
	if ( pproto->has_marriagetime() )
	{
		xmlbuf.append("<info name='marriagetime' text='%s' >", LABEL( "SocialInfoType", "MarriageTime" ) );
		xmlbuf.append("%s",pproto->marriagetime().c_str() );
		xmlbuf.append("</info>\n");	
	}						 	 
	
	if ( pproto->has_weddingmodule() )
	{		
		xmlbuf.append("<info name='weddingmodule' text='%s' >", LABEL( "SocialInfoType", "WeddingModule" ) );
		xmlbuf.append("%d",pproto->weddingmodule());
		xmlbuf.append("</info>\n");		
	} 	
	
	if ( pproto->has_weddingtime() )
	{
		xmlbuf.append("<info name='weddingtime' text='%s' >", LABEL( "SocialInfoType", "WeddingTime" ) );
		xmlbuf.append("%s",pproto->weddingtime().c_str() );
		xmlbuf.append("</info>\n");		
	}		
	
	if ( pproto->has_familyname() )
	{
		xmlbuf.append("<info name='familyname' text='%s' >", LABEL( "SocialInfoType", "Familyname" ) );
		xmlbuf.append("%s",pproto->familyname().c_str());
		xmlbuf.append("</info>\n");		
	}
	
	if (pproto->has_corpsname() )
	{
		xmlbuf.append("<info name='corpsname' text='%s' >", LABEL( "SocialInfoType", "CorpsName" ) );
		xmlbuf.append("%s",pproto->corpsname().c_str());
		xmlbuf.append("</info>\n");		
	}					 	
	
	if ( pproto->has_matrixinfo() )
	{
		if ( pproto->mutable_matrixinfo()->masterid_size() > 0 )
		{
			xmlbuf.append("<info name='masterid' text='%s' >", LABEL( "SocialInfoType", "MasterID" ) );		
			xmlbuf.append("%d",pproto->mutable_matrixinfo()->masterid( 0 ) );
			for ( int i = 1; i < pproto->mutable_matrixinfo()->masterid_size(); ++i )
			{
				xmlbuf.append(",%d",pproto->mutable_matrixinfo()->masterid( i ) );
			}		
			xmlbuf.append("</info>\n");	
		}
				
		if ( pproto->mutable_matrixinfo()->slaveid_size() > 0 )
		{
			xmlbuf.append("<info name='slaveid' text='%s' >", LABEL( "SocialInfoType", "SlaveID" ) );
			xmlbuf.append("%d",pproto->mutable_matrixinfo()->slaveid( 0 ) );
			for ( int i = 1; i < pproto->mutable_matrixinfo()->slaveid_size(); ++i )
			{
				xmlbuf.append(",%d",pproto->mutable_matrixinfo()->slaveid( i ) );
			}	
			xmlbuf.append("</info>\n");	
		}
		
		if ( pproto->mutable_matrixinfo()->slavevalue_size() > 0 )
		{
			xmlbuf.append("<info name='slavevalue' text='%s' >", LABEL( "SocialInfoType", "SlaveValue" ) );
			xmlbuf.append("%d",pproto->mutable_matrixinfo()->slavevalue( 0 ) );
			for ( int i = 1; i < pproto->mutable_matrixinfo()->slavevalue_size(); ++i )
			{
				xmlbuf.append(",%d",pproto->mutable_matrixinfo()->slavevalue( i ) );
			}	
			xmlbuf.append("</info>\n");	
		}
				
		xmlbuf.append("<info name='slavepoint' text='%s' >", LABEL( "SocialInfoType", "SlavePoint" ) );
		xmlbuf.append("%d",pproto->mutable_matrixinfo()->slavepoint(  ) );	
		xmlbuf.append("</info>\n");	
		
		xmlbuf.append("<info name='masterpoint' text='%s' >", LABEL( "SocialInfoType", "MasterPoint" ) );
		xmlbuf.append("%d",pproto->mutable_matrixinfo()->masterpoint(  ) );	
		xmlbuf.append("</info>\n");	
		
		xmlbuf.append("<info name='slavetotal' text='%s' >", LABEL( "SocialInfoType", "SlaveTotal" ) );
		xmlbuf.append("%d",pproto->mutable_matrixinfo()->slavetotal(  ) );	
		xmlbuf.append("</info>\n");	
		
		xmlbuf.append("<info name='mastertotal' text='%s' >", LABEL( "SocialInfoType", "MasterTotal" ) );
		xmlbuf.append("%d",pproto->mutable_matrixinfo()->mastertotal(  ) );	
		xmlbuf.append("</info>\n");		
		
		if ( pproto->mutable_matrixinfo()->masterpercent_size() > 0 )
		{
			xmlbuf.append("<info name='masterpercent' text='%s' >", LABEL( "SocialInfoType", "MasterPercent" ) );
			xmlbuf.append("%d",pproto->mutable_matrixinfo()->masterpercent( 0 ) );	
			for ( int i = 1; i < pproto->mutable_matrixinfo()->masterpercent_size(); ++i )
			{
				xmlbuf.append(",%d",pproto->mutable_matrixinfo()->masterpercent( i ) );
			}	
			xmlbuf.append("</info>\n");	 		
		}
						
		xmlbuf.append("<info name='mastertimes' text='%s' >", LABEL( "SocialInfoType", "MasterTimes" ) );
		xmlbuf.append("%d",pproto->mutable_matrixinfo()->mastertimes(  ) );	
		xmlbuf.append("</info>\n");
		
		xmlbuf.append("<info name='slavenum' text='%s' >", LABEL( "SocialInfoType", "SlaveNum" ) );
		xmlbuf.append("%d",pproto->mutable_matrixinfo()->slavenum(  ) );	
		xmlbuf.append("</info>\n");
		
		xmlbuf.append("<info name='mastergive' text='%s' >", LABEL( "SocialInfoType", "MasterGive" ) );
		xmlbuf.append("%d",pproto->mutable_matrixinfo()->mastergive(  ) );	
		xmlbuf.append("</info>\n");
		
		xmlbuf.append("<info name='beginnernum' text='%s' >", LABEL( "SocialInfoType", "BeginnerNum" ) );
		xmlbuf.append("%d",pproto->mutable_matrixinfo()->beginnernum(  ) );	
		xmlbuf.append("</info>\n");		 		
		
		xmlbuf.append("<info name='formalnum' text='%s' >", LABEL( "SocialInfoType", "FormalNum" ) );
		xmlbuf.append("%d",pproto->mutable_matrixinfo()->formalnum(  ) );	
		xmlbuf.append("</info>\n");			
	}
	
	
	xmlbuf.append("</details_info>");
	xmlbuf.append("</Response>\n");	
	return 0;
}

int FlashGmCmdRoleTitleResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageSqlQueryResult* pResult = (CGMMessageSqlQueryResult*) tpMessage->msgpara();
	CMessageExecuteSqlResponse3* pSqlResult = pResult->mutable_sqlresult();

	if ( pSqlResult->resultcode() == 0 || pSqlResult == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( colnum != 2 || rownum != 1 )
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	PBPlayerTitleInfo proto;
	if (!proto.ParseFromArray(pSqlResult->fieldvalue(1).c_str(), pSqlResult->fieldvaluelen(1)))
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append(
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" 
		"<Response>\n"
			"<result>1</result>\n");

	for ( int i=0; i < proto.titlelist_size(); ++i)
	{
		CTitle* tpTitle = CDataStatic::GetTitle(proto.titlelist(i));
		if ( tpTitle != NULL)
		{
			xmlbuf.append("<title>%s(%d)</title>\n", TOKEN2STRING(tpTitle->mTitle), proto.titlelist(i));
		}
	}

	xmlbuf.append("</Response>\n");
	return xmlbuf.length();
}

int FlashGmCmdRoleMailResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageSqlQueryResult* pResult = (CGMMessageSqlQueryResult*) tpMessage->msgpara();
	CMessageExecuteSqlResponse3* pSqlResult = pResult->mutable_sqlresult();

	if ( pSqlResult->resultcode() == 0 || pSqlResult == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( colnum != 14 )
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append(
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" 
		"<Response>\n"
			"<result>1</result>\n"
			"<page>%d</page>\n"
			"<details_info>\n", pResult->page());
	/*
	mail_id, mail_type, mail_status, sender_role_id, sender_role_name, 
	recver_role_id, recver_role_name, mail_title, mail_body, item_attachment, 
	money_attachment, send_time, expired_time, post_recver 
	*/

	xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_MAIL"));
	for ( int i = 0; i < rownum; ++i)
	{
		MailItemBox proto;	
		C2KString attachment("");
		if (proto.ParseFromArray(pSqlResult->fieldvalue(9 + i * colnum).c_str(), pSqlResult->fieldvaluelen(9 + i * colnum)))
		{
			for (int j = 0; j < proto.mailitem_size(); ++j)
			{
				int nItemID = proto.mailitem (j).itemid ();
				CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(nItemID);

				C2KString desc("");
				EquipSimple(proto.mailitem (j), ptpl, desc);

				attachment.append("%d * %s(%d)[%s]\n", 
						 proto.mailitem (j).number (), ItemID2Name(nItemID), nItemID, desc.c_str());
			}
		}

		char szSendTime[64] = {0};
		char szExpiryTime[64] = {0};

		strcpy(szSendTime, FromUnixTime(atoi(pSqlResult->fieldvalue(11 + i * colnum).c_str())));
		strcpy(szExpiryTime, FromUnixTime(atoi(pSqlResult->fieldvalue(12 + i * colnum).c_str())));

		// 序号,发送时间,发件人,收件人,标题,附件物品,附件金钱,过期时间,收件人支付
		xmlbuf.append("<info xml:space=\"preserve\"><![CDATA[ %s,%s,%s(%s),%s(%s),%s,%s,%s,%s,%s]]></info>\n",
			pSqlResult->fieldvalue(0 + i * colnum).c_str(),
			szSendTime,
			pSqlResult->fieldvalue(4 + i * colnum).c_str(), pSqlResult->fieldvalue(3 + i * colnum).c_str(),
			pSqlResult->fieldvalue(6 + i * colnum).c_str(), pSqlResult->fieldvalue(5 + i * colnum).c_str(),
			pSqlResult->fieldvalue(7 + i * colnum).c_str(),
			attachment.c_str(),
			pSqlResult->fieldvalue(10 + i * colnum).c_str(),
			szExpiryTime,
			pSqlResult->fieldvalue(13 + i * colnum).c_str());
	}

	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>\n");

	return xmlbuf.length();
}

int FlashGmCmdNpcDetailResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageNPCDetailResult* tpResult = (CGMMessageNPCDetailResult*) tpMessage->msgpara();

	if (tpResult->npc_detail_size() == 0)
	{
		xmlbuf.append(
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>%d</result>\n</Response>", 0); 
		return xmlbuf.length();
	}
	
	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" 
			"<Response>\n"
			"<result>%d</result>\n"
			"<count>%d</count>\n", 1, tpResult->npc_detail_size());

	for(int i = 0; i < tpResult->npc_detail_size(); ++i)
	{
		xmlbuf.append(
				"<info>\n<location>%s</location>\n<map_id>%d</map_id>\n</info>\n" ,
				tpResult->npc_detail(i).npc_location().c_str(), 
				tpResult->npc_detail(i).map_id());
	}
	
	xmlbuf.append("</Response>\n");

	return xmlbuf.length();
}

int FlashGmCmdRoleProtoResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();


	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	xmlbuf.append("<%s>\n", tpSqlResult->fieldvalue(0).c_str());

	Message* pproto = NULL;

	if (strcmp(tpSqlResult->fieldvalue(0).c_str(), "role_property") == 0)
	{
		pproto = new PBProperty;
	}else 
	if (strcmp(tpSqlResult->fieldvalue(0).c_str(), "role_skill") == 0)
	{
		pproto = new PBSkillList;
	}
	if (strcmp(tpSqlResult->fieldvalue(0).c_str(), "role_buff") == 0)
	{
		pproto = new PBBuffList;
	}
	if (strcmp(tpSqlResult->fieldvalue(0).c_str(), "role_package") == 0)
	{
		pproto = new PBItemBox;
	}
	if (strcmp(tpSqlResult->fieldvalue(0).c_str(), "role_equipment") == 0)
	{
		pproto = new PBItemBox;
	}
	if (strcmp(tpSqlResult->fieldvalue(0).c_str(), "role_storage") == 0)
	{
		pproto = new PBItemBox;
	}
	if (strcmp(tpSqlResult->fieldvalue(0).c_str(), "role_task") == 0)
	{
		pproto = new PBTaskList;
	}
	if (strcmp(tpSqlResult->fieldvalue(0).c_str(), "role_task_package") == 0)
	{
		pproto = new PBItemBox;
	}
	if (strcmp(tpSqlResult->fieldvalue(0).c_str(), "role_friend") == 0)
	{
		pproto = new PBFriend;
	}
	if (strcmp(tpSqlResult->fieldvalue(0).c_str(), "role_repetion") == 0)
	{
		pproto = new PBRepetion;
	}

	if (pproto && pproto->ParseFromArray( tpSqlResult->fieldvalue(1).c_str(), (int)tpSqlResult->fieldvaluelen(1)))
	{
		protobuf2xml(*pproto, xmlbuf, Aformat);
	}else
	{
		LOG_ERROR("gm", "[%s:%d] %s Message.ParseFromArray error, len(%d)!", 
				__FUNCTION__, __LINE__, tpSqlResult->fieldvalue(0).c_str(), tpSqlResult->fieldvaluelen(1));
	}

	xmlbuf.append("</%s>\n", tpSqlResult->fieldvalue(0).c_str());
	LOG_DEBUG("gm", "%s\n%s", pproto->ShortDebugString().c_str(), xmlbuf.c_str());


	if(pproto) delete pproto;

	return xmlbuf.length();
}


int FlashGmCmdUserStorageInfoResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);


	CGMMessageUserStorageInfoResult* pproto= (CGMMessageUserStorageInfoResult *) tpMessage->msgpara();
	const PBItemBox& tPBItemBox = pproto->items();

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	xmlbuf.append(
		"<Response>\n"
			"<result>%d</result>\n"
			"<count>%d</count>\n", 1, tPBItemBox.itemobjects_size());

	C2KString desc("");
	for(int i = 0; i < tPBItemBox.itemobjects_size(); ++i)
	{
		CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(tPBItemBox.itemobjects(i).itemid());
		//if (ptpl == NULL) continue;

		desc.clear();
		EquipSimple(tPBItemBox.itemobjects(i),  ptpl, desc);

		xmlbuf.append(
				"<info name=\"%d\" text=\"%s\" num=\"%d\" desc=\"%s\"></info>\n" ,
					//tPBItemBox.itemobjects(i).itemid(), 
					tPBItemBox.itemobjects(i).index(), 
					ptpl ? TOKEN2STRING(ptpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"),
					tPBItemBox.itemobjects(i).number(), desc.c_str());
					//ptpl ? TOKEN2STRING(ptpl->mDescribe) : LABEL("EGlobeDefine", "UNKNOWN_DESC"));
	}

	xmlbuf.append("</Response>\n" );

	return xmlbuf.length();
}

int FlashGmCmdRoleLocationResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageRoleLocationResult* tResultMsg = (CGMMessageRoleLocationResult*) tpMessage->msgpara();

	if (tResultMsg->line_id() == -1)
	{
		xmlbuf.append(
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>%d</result>\n</Response>", 0); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	xmlbuf.append(
		"<Response>\n"
			"<result>%d</result>\n"
			"<location>%s, %s, %d, %d</location>"
		"</Response>", 
		1, 
		LABEL("ECamp", tResultMsg->line_id()), 
		CGmServer::GetSingleton().GetMapName(tResultMsg->map_id()), 
		tResultMsg->x(), 
		tResultMsg->y()
		);

	return xmlbuf.length();
}

int FlashGmCmdTaskInfoResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageTaskInfoResult* tResultMsg = (CGMMessageTaskInfoResult*) tpMessage->msgpara();

	if (tResultMsg->task_detail_size() == 0)
	{
		xmlbuf.append(
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>%d</result>\n</Response>", 0); 
		return xmlbuf.length();
	}


	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	xmlbuf.append(
		"<Response>\n"
			"<result>%d</result>\n"
			"<count>%d</count>\n", 1, tResultMsg->task_detail_size());

	for(int i = 0; i < tResultMsg->task_detail_size(); ++i)
	{
		xmlbuf.append(
				"<info>\n <task_id>%d</task_id>\n<task_name>%s</task_name>\n<finish_type>%d</finish_type>\n</info>\n" ,
				tResultMsg->task_detail(i).task_id(), 
				TaskID2Name(tResultMsg->task_detail(i).task_id()),
				tResultMsg->task_detail(i).finish_type());
	}

	xmlbuf.append("</Response>\n" );

	return xmlbuf.length();
}

int FlashGmCmdIbInfoResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);
	CGMMessageIbInfoResult* tpResultMsg = (CGMMessageIbInfoResult*) tpMessage->msgpara();

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	xmlbuf.append(
		"<Response>\n"
			"<result>%d</result>\n"
			"<count>%d</count>\n", 1, tpResultMsg->ibitems_size());

	if (tpResultMsg->isall())
	{
		// 姒傝淇℃伅
		for (int i = 0; i < tpResultMsg->ibitems_size(); ++i)
		{
			CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(tpResultMsg->ibitems(i).item_id());

			xmlbuf.append(
					"<info>\n"
						"<item_id>%d</item_id>\n"
						"<item_name>%s</item_name>\n"
					"</info>\n", 
					tpResultMsg->ibitems(i).item_id(), 
					ptpl ? TOKEN2STRING(ptpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"));
		}
	}else
	{
		// 鍗曚釜鐗╁搧鐨勮缁嗕俊鎭
		LK_ASSERT(tpResultMsg->ibitems_size() == 1, return -1);

		CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(tpResultMsg->ibitems(0).item_id());

		xmlbuf.append(
				"<info>\n"
					"<item_id>%d</item_id>\n"
					"<item_name>%s</item_name>\n"
					"<item_location>%s</item_location>\n"
					"<description>%s</description>\n"
					"<pay_type>%d</pay_type>\n"
					"<item_type>%d</item_type>\n"
					"<item_limit>%d</item_limit>\n"
					"<price>%d</price>\n"
					"<discount>%d</discount>\n"
					"<start_time>%s</start_time>\n"
					"<sale_room>%d</sale_room>\n"
					"<on_sale>%d</on_sale>\n"
				"</info>\n",
					tpResultMsg->ibitems(0).item_id(),
					ptpl ? TOKEN2STRING(ptpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"),
					tpResultMsg->ibitems(0).item_location().c_str(),
					ptpl ? TOKEN2STRING(ptpl->mDescribe) : LABEL("EGlobeDefine", "UNKNOWN_DESC"),
					tpResultMsg->ibitems(0).pay_type(),
					tpResultMsg->ibitems(0).item_type(),
					tpResultMsg->ibitems(0).item_limit(),
					tpResultMsg->ibitems(0).price(),
					tpResultMsg->ibitems(0).discount(),
					tpResultMsg->ibitems(0).start_time(),
					tpResultMsg->ibitems(0).sale_room(),
					tpResultMsg->ibitems(0).on_sale()
				);
	}

	xmlbuf.append("</Response>\n" );

	return xmlbuf.length();
}

void protobuf2xml(const Message& message, CMaxString& debug_string, 
		void (*pformat)(CMaxString&, const char*, const char*, const char*))
{
	const Reflection* reflection = message.GetReflection();
	vector<const FieldDescriptor*> fields;
	reflection->ListFields(message, &fields);

	//const char* szProtoName = message.GetDescriptor()->name().c_str();
	for (unsigned int i = 0; i < fields.size(); i++) 
	{
		 print_field(message, reflection, fields[i], debug_string, pformat);
	}
}

void print_field(const Message& message, 
		const Reflection* reflection, const FieldDescriptor* field, CMaxString& generator, 
		void (*pformat)(CMaxString&, const char*, const char*, const char*)) 
{
	int count = 0;

	if (field->is_repeated()) 
	{
		count = reflection->FieldSize(message, field);
	} else if (reflection->HasField(message, field)) 
	{
		count = 1;
	}

	for (int j = 0; j < count; ++j) 
	{
		int field_index = field->is_repeated() ? j : -1;
		print_fieldvalue(message, reflection, field, field_index, generator, pformat);
	}
}

void print_fieldvalue(const Message& message, const Reflection* reflection, 
		const FieldDescriptor* field, int index, CMaxString& generator, 
		void (*pformat)(CMaxString&, const char*, const char*, const char*))
{
	LK_ASSERT( field->is_repeated() || index == -1, 
			LOG_FATAL("gm", "Index must be -1 for non-repeated fields"));

	switch (field->cpp_type()) 
	{
		#define OUTPUT_FIELD(CPPTYPE, METHOD, TO_STRING)	\
		case FieldDescriptor::CPPTYPE_##CPPTYPE:			\
		{													\
			pformat(generator, \
					message.GetDescriptor()->name().c_str(), \
					field->name().c_str(), \
					TO_STRING ( field->is_repeated() ? 	\
							reflection->GetRepeated##METHOD(message, field, index): \
							reflection->Get##METHOD(message, field)).c_str()); 		\
			break;	\
		}			\


		OUTPUT_FIELD( INT32,  Int32, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD( INT64,  Int64, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD(UINT32, UInt32, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD(UINT64, UInt64, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD( FLOAT,  Float, ::google::protobuf::SimpleFtoa);
		OUTPUT_FIELD(DOUBLE, Double, ::google::protobuf::SimpleDtoa);
		#undef OUTPUT_FIELD

		case FieldDescriptor::CPPTYPE_STRING: 
		{
			string scratch;

			const string& value = field->is_repeated() ?
				reflection->GetRepeatedStringReference(
						message, field, index, &scratch) :
					reflection->GetStringReference(message, field, &scratch);

			pformat(generator, 
					message.GetDescriptor()->name().c_str(), 
					field->name().c_str(), TOKEN2STRING(value.c_str()));
			break;
		}

		case FieldDescriptor::CPPTYPE_BOOL:
		{
			if (field->is_repeated()) 
			{
				pformat(generator, 
						message.GetDescriptor()->name().c_str(), 
						field->name().c_str(),
						reflection->GetRepeatedBool(message, field, index) ? "true" : "false");
			} else 
			{
				pformat(generator,
						message.GetDescriptor()->name().c_str(), 
						field->name().c_str(),
						reflection->GetBool(message, field) ? "true" : "false");
			}
			break;
		}

		case FieldDescriptor::CPPTYPE_ENUM:
		{
			pformat(generator, 
					message.GetDescriptor()->name().c_str(), 
					field->name().c_str(),
					field->is_repeated() ?  
						reflection->GetRepeatedEnum(message, field, index)->name().c_str() : 
						reflection->GetEnum(message, field)->name().c_str());
			break;
		}

		case FieldDescriptor::CPPTYPE_MESSAGE:
		{
			protobuf2xml(
					field->is_repeated() ?  
						reflection->GetRepeatedMessage(message, field, index) : reflection->GetMessage(message, field), 
					generator, pformat);
			break;
		}
	}
}

int FlashGmCmdRoleEquipmentResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageRoleEquipmentResult* pproto = (CGMMessageRoleEquipmentResult*) tpMessage->msgpara();

	xmlbuf.append(	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	xmlbuf.append( "<Response>\n"
						"<result>%d</result>\n"
						"<role_info>\n", 1 );

	const PBItemBox & tPBItemBox = pproto->proto();
	for(int i = 0; i < tPBItemBox.itemobjects_size(); ++i)
	{
		CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(tPBItemBox.itemobjects(i).itemid());
		//if (ptpl == NULL) continue;
		const PBItemObject& pbItem = tPBItemBox.itemobjects(i);
		unsigned int nPos = pbItem.index(); 

		// 取得装备的描述信息
		C2KString desc("");
		EquipDetail(pbItem, ptpl, desc);

		xmlbuf.append( "<info><![CDATA[ %s(%d, %s)<br/>%s ]]></info>\n" , 
					LABEL("EEquipPart", nPos), 
					nPos,
					ptpl ? TOKEN2STRING(ptpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_DESC"), 
					desc.c_str());
	}

	xmlbuf.append( "</role_info>\n"
				"</Response>\n" );

	return xmlbuf.length();
}


int FlashGmCmdRolePackInfoResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageRolePackInfoResult* pproto = (CGMMessageRolePackInfoResult*) tpMessage->msgpara();

	const PBItemBox & tPBItemBox = pproto->proto();

	xmlbuf.append(	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	xmlbuf.append( "<Response>\n"
						"<result>%d</result>\n" 
						"<count>%d</count>\n", 1, tPBItemBox.itemobjects_size());
						

	C2KString desc("");
	for(int i = 0; i < tPBItemBox.itemobjects_size(); ++i)
	{
		CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(tPBItemBox.itemobjects(i).itemid());
		//if (ptpl == NULL) continue;
		
		desc.clear();
		EquipSimple(tPBItemBox.itemobjects(i),  ptpl, desc);

		xmlbuf.append("<info name=\"%d\" text=\"%s\" num=\"%d\" desc=\"%s\"></info>\n" , 
					tPBItemBox.itemobjects(i).index(),
					ptpl ? TOKEN2STRING(ptpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"),
					tPBItemBox.itemobjects(i).number(),
					desc.c_str()
					);

	}

	xmlbuf.append("</Response>\n");

	return xmlbuf.length();
}

int FlashGmCmdRoleStorageDetailResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageRoleStorageDetailResult* pproto = (CGMMessageRoleStorageDetailResult*) tpMessage->msgpara();

	const PBItemObject& tPBItemObject = pproto->proto();
	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");

	CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(tPBItemObject.itemid());

	xmlbuf.append(
		"<Response>\n"
			"<result>%d</result>\n"
			"<detail_info>\n"
				"<info name=\"%s\">%s</info>\n"
				"<info name=\"%s\">%d</info>\n" ,
				1, 
				LABEL("EGlobeDefine", "ITEM_NAME"),
				ptpl ? TOKEN2STRING(ptpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"),
				LABEL("PBItemObject", "Number"),
				tPBItemObject.number());


	protobuf2xml(tPBItemObject, xmlbuf, Aformat);

	xmlbuf.append( "</detail_info>\n</Response>\n" );

	return xmlbuf.length();
}

int FlashGmCmdRoleEquipmentDetailResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageRoleEquipmentDetailResult* pproto = (CGMMessageRoleEquipmentDetailResult*) tpMessage->msgpara();

	const PBItemObject& tPBItemObject = pproto->proto();
	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");

	CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(tPBItemObject.itemid());

	xmlbuf.append(
		"<Response>\n"
			"<result>%d</result>\n"
			"<detail_info>\n"
				"<info name=\"%s\">%s</info>\n"
				"<info name=\"%s\">%d</info>\n" ,
				1, 
				LABEL("EGlobeDefine", "ITEM_NAME"),
				ptpl ? TOKEN2STRING(ptpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"),
				LABEL("PBItemObject", "Number"),
				tPBItemObject.number()
				);


	protobuf2xml(tPBItemObject, xmlbuf, Aformat);

	xmlbuf.append( "</detail_info>\n</Response>\n" );

	return xmlbuf.length();
}

int FlashGmCmdRoleItemInPackResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageRoleItemInPackResult* pproto = (CGMMessageRoleItemInPackResult*) tpMessage->msgpara();

	const PBItemObject& tPBItemObject = pproto->proto();
	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");

	CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(tPBItemObject.itemid());

	int nItemType =  ptpl ? ptpl->mItemType : -1;
	int nColor = (nItemType == CTplItem::ITEM_WEAPON ? ((CTemplateEquipment*) ptpl)->mShowColour : 0);

	xmlbuf.append(
		"<Response>\n"
			"<result>%d</result>\n"
			"<detail_info>\n"
				"<info name=\"%s\">%s</info>\n"
				"<info name=\"%s\">%d</info>\n" 
				"<info name=\"%s\">%d</info>\n" ,
				1, 
				LABEL("EGlobeDefine", "ITEM_NAME"),
				ptpl ? TOKEN2STRING(ptpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"),
				LABEL("PBItemObject", "Number"),
				tPBItemObject.number(),
				LABEL("PBItemObject", "Color"),
				nColor
				);


	protobuf2xml(tPBItemObject, xmlbuf, Aformat);

	xmlbuf.append( "</detail_info>\n</Response>\n" );

	return xmlbuf.length();
}

int FlashGmCmdRoleSkillInfoResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageRoleSkillInfoResult* pproto = (CGMMessageRoleSkillInfoResult*) tpMessage->msgpara();
	
	const PBSkillList& tPBSkillList = pproto->proto();

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	xmlbuf.append(
		"<Response>\n"
			"<result>%d</result>\n"
			"<count>%d</count>\n", 
			1,
			tPBSkillList.skillobjects_size());

	for (int i =0; i < tPBSkillList.skillobjects_size(); ++i)
	{
		CTemplateSkill *ptpl = (CTemplateSkill *) CDataStatic::SearchSkill(
				tPBSkillList.skillobjects(i).skillid(),
				tPBSkillList.skillobjects(i).skilllevel());

		//鍙栧緱鎶�鑳藉搴旀ā鏉
		xmlbuf.append( "<info name=\"%s\" level=\"%d\" id=\"%d\"></info>\n", 
				ptpl != NULL ? TOKEN2STRING(ptpl->mName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"),
				tPBSkillList.skillobjects(i).skilllevel(), tPBSkillList.skillobjects(i).skillid());
	}

	xmlbuf.append( "</Response>\n" );

	return xmlbuf.length();
}

int FlashGmCmdRoleDetailResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageRoleDetailResult* pproto = (CGMMessageRoleDetailResult*) tpMessage->msgpara();

	const PBRoleInfo& tPBRoleInfo = pproto->proto();

	xmlbuf.append(
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" 
		"<Response>\n"
			"<result>1</result>\n"
			"<role_info>\n"
			);

	protobuf2xml(tPBRoleInfo, xmlbuf, Bformat);
	
	xmlbuf.append(
			"</role_info>\n"
		"</Response>\n" 
		);

	return xmlbuf.length();
}

int FlashGmCmdRoleSkillDetailResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMessageRoleSkillDetailResult* pproto = (CGMMessageRoleSkillDetailResult*) tpMessage->msgpara();
	
	const PBSkillObject& tPBSkillObject = pproto->proto();
	CTemplateSkill *ptpl = (CTemplateSkill *) CDataStatic::SearchSkill(tPBSkillObject.skillid(), tPBSkillObject.skilllevel());

	if (ptpl == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
		xmlbuf.append(
			"<Response>\n"
				"<result>%d</result>\n"
			"</Response>", 0);

		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	xmlbuf.append(
		"<Response>\n"
			"<result>%d</result>\n"
			"<detail_info>\n", 1);

	switch ( ptpl->mSkillTemplateType)
	{
		case CTemplateSkill::SKILL_TEMPLATE_TYPE_AUREOLE: 
		{
			CTemplateAureoleSkill* pObjTpl = (CTemplateAureoleSkill*) ptpl;
			PBAureoleSkill pbAureoleSkill;

			pObjTpl->CreateTlvPB(true, &pbAureoleSkill);
			protobuf2xml(pbAureoleSkill, xmlbuf, Aformat);
			break;
		}
		case CTemplateSkill::SKILL_TEMPLATE_TYPE_NORMAL: 
		{
			CTemplateNormalSkill* pObjTpl = (CTemplateNormalSkill*) ptpl;
			PBNormalSkill pbNormalSkill;

			pObjTpl->CreateTlvPB(true, &pbNormalSkill);
			protobuf2xml(pbNormalSkill, xmlbuf, Aformat);

			break;
		}
		case CTemplateSkill::SKILL_TEMPLATE_TYPE_TRIP: 
		{
			CTemplateTrapSkill* pObjTpl = (CTemplateTrapSkill*) ptpl;
			PBTrapSkill pbTrapSkill;

			pObjTpl->CreateTlvPB(true, &pbTrapSkill);
			protobuf2xml(pbTrapSkill, xmlbuf, Aformat);
			break;
		}
		case CTemplateSkill::SKILL_TEMPLATE_TYPE_SUMMONBEAST: 
		{
			PBSkill pbSummonBeast;
			ptpl->CreateTlvPB(true, &pbSummonBeast);

			protobuf2xml(pbSummonBeast, xmlbuf, Aformat);
			break;
		}
	}

	xmlbuf.append( "</detail_info>\n"
				"</Response>\n" );

	return xmlbuf.length();
}

int FlashGmCmdGoodAtNpcResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	/*
	 * SELECT role_id, role_name,  role_metier, role_level,  -- 0 ~ 3
	 * op_time, -- 4 
	 * at_lineid, at_mapid, at_posx, at_posy, -- 5 ~ 8
	 * item_id, item_name , item_num, item_guid, -- 9 ~ 12
	 * op_type, op_param1, op_param2, op_param3, op_param4 --- 13 ~ 17
	 *
	 * select FROM_UNIXTIME(op_time),  item_id,  item_num, item_guid, op_type, npc_id, price
	*/

	int nTotalMoney = 0; 
	int nItemCount = 0;
	int nRecordCount = 0;

	CMaxString context("");
	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		nTotalMoney += atoi(pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str());
		nItemCount += atoi( pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str());

		int nNpcID = atoi(pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str());
		int nItemID = atoi(pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str());

		CTemplateOgre *pNpcTpl = (CTemplateOgre *) CDataStatic::SearchTpl(nNpcID);
		CTplItem *pItemTpl = (CTplItem *) CDataStatic::SearchTpl(nItemID);
		if ( pNpcTpl == NULL || pItemTpl == NULL) continue;

		context.append("<info>\n"
						"<npc_name>%s</npc_name>\n"
						"<item_id>%d:%s</item_id>\n"
						"<item_name>%s</item_name>\n"
						"<item_num>%s</item_num>\n"
						"<time>%s</time>\n"
					  "</info>\n", 
					  	pNpcTpl ? TOKEN2STRING(pNpcTpl->mName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"),
						nItemID, pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str(), 
					  	pItemTpl ? TOKEN2STRING(pItemTpl->mItemName) :LABEL("EGlobeDefine", "UNKNOWN_NAME"),
						pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str()
				);

		nRecordCount ++;
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
				"<result>1</result>\n"
				"<user_item_count>%d</user_item_count>\n"
				"<npc_money>%d</npc_money>\n"
				"<count>%d</count>\n", nItemCount, nTotalMoney, nRecordCount);

	xmlbuf.append("%s", context.c_str());

	xmlbuf.append("</Response>");
				
	return xmlbuf.length();
}

int FlashGmCmdGoodAtUsersResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
				"<result>1</result>\n"
				"<count>%d</count>\n", rownum);

	/*
	 *  0        1        				 2        3         4         5          6        7          8      
	 *  role_id, FROM_UNIXTIME(op_time), line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1
	 *
	*/
	
	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		int nOpType = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
		int nItemID = atoi(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str());

		CTplItem *pItemTpl = (CTplItem *) CDataStatic::SearchTpl(nItemID);

		switch (nOpType)
		{
			case EM_LOST_BY_SEND_MAIL:
			case EM_LOST_BY_STALL_SELL:
			case EM_LOST_BY_EXCHANGE:
			{
				xmlbuf.append("<info>\n"
							"<from>%s</from>\n"
							"<to>%s</to>\n"
							"<item_id>%s:%s</item_id>\n"
							"<item_name>%s</item_name>\n"
							"<item_count>%s</item_count>\n"
							"<time>%s</time>\n"
							"</info>\n", 
							pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
							pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str(),
							pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str(),
							pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str(),
							pItemTpl ? TOKEN2STRING(pItemTpl->mItemName) :
								pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str(),
							pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(),
							pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str()
							);
				break;
			}

			case EM_GET_BY_RECV_MAIL:
			case EM_GET_BY_STALL_BUY:
			case EM_GET_BY_EXCHANGE:
			{

				xmlbuf.append("<info>\n"
						"<from>%s</from>\n"
						"<to>%s</to>\n"
						"<item_id>%s</item_id>\n"
						"<item_name>%s</item_name>\n"
						"<item_count>%s</item_count>\n"
						"<time>%s</time>\n"
						"</info>\n", 
						pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str(),
						pItemTpl ? TOKEN2STRING(pItemTpl->mItemName) :
							pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str()
						);
				break;
			}
		}
	}

	xmlbuf.append("</Response>");
	return xmlbuf.length();
}

int FlashGmCmdGoodHistoryResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( colnum != 11 )
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	// 取得session
	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
				"<result>1</result>\n"
				"<count>%d</count>\n",
				rownum);

	static const proto_func_t funcs[] = 
	{
		{"ETaskName(",  TaskID2Name },
		{"EItemName(",  ItemID2Name },
		{"ENpcName(",   NpcID2Name  },
		{"FromUnixTime(",  FromUnixTime},
		{"EMoneyType(", MoneyType2Name},
		{"EMapName(", 	MapID2Name},
		{"FormatMoney(", FormatMoney},
		{NULL, NULL}
	};

	replace_t repl;
	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		int nItemID = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
		int nOpType = atoi(pSqlResult->fieldvalue(9 + tRowIndex * colnum).c_str());

		// 取得描述的字符串
		C2KString desc(LABEL("EItemAction", nOpType));

		repl.clear();
		repl.push_back(replace_t::value_type("$OP_PARAM1", pSqlResult->fieldvalue(4 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM2", pSqlResult->fieldvalue(5 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM3", pSqlResult->fieldvalue(6 + tRowIndex * colnum)));

		DescItemAction(desc, repl, funcs); 

		xmlbuf.append(
				"<info>\n"
					"<time>%s %s</time>\n"
					"<from>%s</from>\n"
					"<to>%s(%d)</to>\n"
					"<action>%s %s %s(%d) </action>\n"
				"</info>\n",
				pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
				LABEL("EMapName", atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str())),
				pSqlResult->fieldvalue(10 + tRowIndex * colnum).c_str(),
				nOpType < 50 ? LABEL("EGlobeDefine", "EM_GET") : LABEL("EGlobeDefine", "EM_LOST"), nOpType,
				desc.c_str(), pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(), ItemID2Name(nItemID), nItemID
		);
	}
		
	xmlbuf.append("</Response>");
	return xmlbuf.length();
}

int FlashGmCmdQueryLogoutIPResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	size_t colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( rownum == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
				"<Response>\n"
				"<result>1</result>\n"
				"<details_info>\n");
		xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_OFFLINE_IP"));
		xmlbuf.append("</details_info>\n");
		xmlbuf.append("</Response>");

		return xmlbuf.length();
	}

	if ( colnum != 3)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>");
		return xmlbuf.length();
	}          


	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
			"<result>1</result>\n"
			"<page>%d</page>\n"
			"<details_info>\n", 1);

	xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_OFFLINE_IP"));

	for ( int tRowIndex = 0; tRowIndex < rownum; ++tRowIndex)
	{          
		struct in_addr addr;
		addr.s_addr = strtoul(pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(), (char**)NULL, 10);
		xmlbuf.append("<info>%s,%s,%s</info>\n", 
					inet_ntoa(addr),
					pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(), 
					pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str());
	}

	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");

	return xmlbuf.length();	
}

int FlashGmCmdUserInfoByIPResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CGMMsgUserInfoByIPResult* pproto = (CGMMsgUserInfoByIPResult*) tpMessage->msgpara();
	
	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
				"<result>1</result>\n"
				"<page>1</page>\n"
				"<details_info>\n"); 

	if ( pproto->zeroip())
	{
		xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_ONLINE_IP"));
	}else
	{
		xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_ONLINE_PLAYER"));
	}

	for( int i = 0; i < pproto->ipinfo_size(); ++i)
	{

		const IPInfo& ipinfo = pproto->ipinfo(i);

		struct in_addr addr;
		addr.s_addr = ipinfo.ip();
		if ( pproto->zeroip())
		{
			xmlbuf.append("<info>%s,", inet_ntoa(addr));
			xmlbuf.append("%d", ipinfo.role_num());
			u_long arr = htonl(ipinfo.ip());
			if (CGmServer::GetSingleton().GetProvince(arr))
			{
				unsigned short pp[16] = {0};
				int len  = CGmServer::GetSingleton().GetCityLen(arr);
				memcpy(pp,CGmServer::GetSingleton().GetProvince(arr),len*2+1);
				wchar_t pp2[16]={0};
				for (int i= 0;i< len;i++)
				{
					pp2[i] = pp[i];
				}
				char *pp1 =  new char[len*4+1];
				setlocale(LC_ALL,"zh_CN.GBK");
				wcstombs(pp1,pp2,len*4+1);
				xmlbuf.append(",%s",pp1);
				delete pp1;
			}
			else
			{
				LOG_ERROR("gm","ip city name is empty ,ip =%d",arr);
				//xmlbuf.append(",local_ip");
				if (CGmServer::GetSingleton().GetProvince(ipinfo.ip()))
				{
					unsigned short pp[16] = {0};
					int len  = CGmServer::GetSingleton().GetCityLen(ipinfo.ip());
					memcpy(pp,CGmServer::GetSingleton().GetProvince(ipinfo.ip()),len*2+1);
					wchar_t pp2[16]={0};
					for (int i= 0;i< len;i++)
					{
						pp2[i] = pp[i];
					}
					char *pp1 =  new char[len*4+1];
					setlocale(LC_ALL,"zh_CN.GBK");
					wcstombs(pp1,pp2,len*4+1);
					xmlbuf.append(",%s",pp1);
					delete pp1;
				}
				else
				{
					LOG_ERROR("gm","ip city name2 is empty ,ip =%d,ip_name = %s",arr,inet_ntoa(addr));
					xmlbuf.append(",Empty");
				}
				
			}
			
			xmlbuf.append("</info>\n");
		}else
		{
			for ( int j = 0; j< ipinfo.player_info_size(); ++j)
			{
				// "IP,账号,角色,等级,职业,登陆时间,阵营,地图,MAC"
				xmlbuf.append("<info>%s,%d,%s(%d),%d,%s,%s,%s,%s,%s,%d</info>\n", 
						inet_ntoa(addr), 
						ipinfo.player_info(j).account_id(),
						ipinfo.player_info(j).role_name().c_str(), 
						ipinfo.player_info(j).role_id(),
						ipinfo.player_info(j).role_level(),
						LABEL("EMetier", ipinfo.player_info(j).metier_id()),
						FromUnixTime(ipinfo.player_info(j).login_time()),
						LABEL("ELineID", ipinfo.player_info(j).line_id()),
						LABEL("EMapName", ipinfo.player_info(j).map_id()),
						ipinfo.player_info(j).mac().c_str(),
						ipinfo.player_info(j).game_id()
						);
			}
		}

	}

	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");
	return xmlbuf.length();
}

int FlashGmCmdOperationDetailResult( CMessage* tpMessage, CMaxString& xmlbuf)
{
		
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
				"<result>1</result>\n"
				"<count>%d</count>\n", rownum);

	/*
	 *  0                       1        2        3         4         5          6        7          8          9
	 *  FROM_UNIXTIME(op_time), line_id, map_id,  item_id,  item_num, item_guid, op_type, op_param1, op_param2, op_param3
	 *
	*/

	
	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		int nOpType = atoi(pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str());
		int nMapID = atoi(pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str());

		const char* szAction = LABEL("EItemAction", nOpType);
		const char* szMap = LABEL("EMapName", nMapID);

		lk::CString32 sFrom("");
		lk::CString32 sTo("");

		switch (nOpType)
		{
			case EM_GET_BY_RECV_MAIL:
			case EM_GET_BY_STALL_BUY:
			case EM_GET_BY_EXCHANGE:
				{
					sFrom.append("RoleID:%s", pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
					break;
				}

			case EM_GET_BY_ACCEPT_TASK:
			case EM_GET_BY_END_TASK:
			case EM_GET_BY_RUN_TASK:
			case EM_GET_BY_TASKITEM:
				{
					int nTaskID = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
					sFrom.append("%s", TaskID2Name(nTaskID));
					break;
				}

			case EM_GET_BY_ERATING:
				{
					sFrom.append("ActivityID:%s", pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
					break;
				}

			case EM_GET_BY_PICKITEM:
				{
					sFrom.append("X:%s Y:%s", 
							pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str(),
							pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str());
					break;
				}

			case EM_GET_BY_NPC_SHOP:
				{
					int nFromNpcID = atoi(pSqlResult->fieldvalue(9 + tRowIndex * colnum).c_str());
					CTemplateOgre *pItemTpl = (CTemplateOgre *) CDataStatic::SearchTpl(nFromNpcID);

					sFrom.append("%s", pItemTpl ? TOKEN2STRING(pItemTpl->mName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"));
					break;
				}

			case EM_GET_BY_DEFAULT:
				{
					break;
				}
			case EM_GET_BY_REPETION:
				{
					sFrom.append("InstanceID:%s", pSqlResult->fieldvalue(9 + tRowIndex * colnum).c_str());
					break;
				}
			case EM_GET_BY_COLLECT:
			case EM_GET_BY_GMINSERT:
			case EM_GET_BY_ROLLBACK:
				{
					sFrom.append("X:%s Y:%s", 
							pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str(),
							pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str());
					break;
				}

			case EM_GET_BY_DIRECTINS:
				{
					int nFromItemID = atoi(pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str());
					CTplItem *pItemTpl = (CTplItem *) CDataStatic::SearchTpl(nFromItemID);

					sFrom.append("%s", pItemTpl ? TOKEN2STRING(pItemTpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"));
					break;
				}

			case EM_GET_BY_COMPOSE:
			case EM_GET_BY_CHANGE:
			case EM_GET_BY_PRODUCE:
			case EM_GET_BY_UNBOUND:
			case EM_GET_BY_DECOMPOUND:
			case EM_GET_BY_INVESTIGATION:
				{
					int nFromItemID = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
					CTplItem *pItemTpl = (CTplItem *) CDataStatic::SearchTpl(nFromItemID);

					sFrom.append("%s", pItemTpl ? TOKEN2STRING(pItemTpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"));
					break;
				}

			case EM_GET_BY_GUAJI_SHOP:
			case EM_GET_BY_BUY_BACK:
			case EM_GET_BY_IBSTORE:
				{
					int nMoneyType = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
					int nMoneyValue = atoi(pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str());

					sFrom.append("%s %d", LABEL("EMoneyUnit", nMoneyType), nMoneyValue);
					break;
				}

			case EM_LOST_BY_SEND_MAIL:
			case EM_LOST_BY_STALL_SELL:
			case EM_LOST_BY_EXCHANGE:
				{
					sTo.append("RoleID:%s", pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
					break;
				}

			case EM_LOST_BY_ACCEPT_TASK:
			case EM_LOST_BY_END_TASK:
			case EM_LOST_BY_ABORTTASK:
				{
					int nTaskID = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());

					lk::CString32 sTaskTplName("");
					sTaskTplName.append("#td_%d_name", nTaskID);

					sTo.append("%s", TOKEN2STRING(sTaskTplName.c_str())); 
					break;
				}
			case EM_LOST_BY_ERATING:
				{
					sTo.append("ActivityID:%s", pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
					break;
				}
			case EM_LOST_BY_THROW_ITEM:
			case EM_LOST_BY_USE:
			case EM_LOST_BY_DESTROY:
				{
					sFrom.append("X:%s Y:%s", 
							pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str(),
							pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str());
					break;
				}

			case EM_LOST_BY_PRODUCE:
			case EM_LOST_BY_OPEN_SLOT:
			case EM_LOST_BY_COMPOSE:
			case EM_LOST_BY_EAT:
			case EM_LOST_BY_INSERT_SLOT:
			case EM_LOST_BY_CHANGE:
			case EM_LOST_BY_JUDGE:
			case EM_LOST_BY_BOUND:
				{
					int nFromItemID = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
					CTplItem *pItemTpl = (CTplItem *) CDataStatic::SearchTpl(nFromItemID);

					sTo.append("%s", pItemTpl ? TOKEN2STRING(pItemTpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"));

					break;
				}

			case EM_LOST_BY_UPGRADE:
				{
					int nFromItemID = atoi(pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str());
					CTplItem *pItemTpl = (CTplItem *) CDataStatic::SearchTpl(nFromItemID);

					sTo.append("%s", pItemTpl ? TOKEN2STRING(pItemTpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"));

					break;
				}

			case EM_LOST_BY_DECOMPOUND:
				{
					break;
				}

			case EM_LOST_BY_SELL2NPC:
				{
					int nFromNpcID = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
					CTemplateOgre *pItemTpl = (CTemplateOgre *) CDataStatic::SearchTpl(nFromNpcID);

					sTo.append("%s", pItemTpl ? TOKEN2STRING(pItemTpl->mName): LABEL("EGlobeDefine", "UNKNOWN_NAME"));

					break;
				}
		}

	
		int nItemID = atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str());
		CTplItem *pItemTpl = (CTplItem *) CDataStatic::SearchTpl(nItemID);

		xmlbuf.append("<info>\n"
				"<from>%s</from>\n"
				"<to>%s</to>\n"
				"<action>%s</action>\n"
				"<time>%s</time>\n"
				"<item_id>%d:%s</item_id>\n"
				"<item_name>%s</item_name>\n"
				"<item_num>%s</item_num>\n"
				"<location>%s:%s</location>\n"
				"</info>\n", 
				sFrom.c_str(),
				sTo.c_str(),
				szAction,					
				pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
				nItemID,
				pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(),
				pItemTpl ? TOKEN2STRING(pItemTpl->mItemName) : LABEL("EGlobeDefine", "UNKNOWN_NAME"),
				pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str(),
				pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(),
				szMap ? szMap : LABEL("EGlobeDefine", "UNKNOWN_NAME")
				);

	}

	xmlbuf.append("</Response>");
	return xmlbuf.length();
}

int FlashGmCmdRoleItemOperateResult( CMessage* tpMessage, CMaxString& xmlbuf )
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( colnum != 11 )
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	// 取得session
	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	static const proto_func_t funcs[] = 
	{
		{"ETaskName(",  TaskID2Name },
		{"EItemName(",  ItemID2Name },
		{"ENpcName(",   NpcID2Name  },
		{"FromUnixTime(",  FromUnixTime},
		{"EMoneyType(", MoneyType2Name},
		{"EMapName(", 	MapID2Name},
		{"FormatMoney(", FormatMoney},
		{NULL, NULL}
	};

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
				"<result>1</result>\n"
				"<count>%d</count>\n" 
				"<page>%d</page>\n", rownum, pSession->GetParam3());

	replace_t repl;
	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		int nItemID = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
		int nOpType = atoi(pSqlResult->fieldvalue(9 + tRowIndex * colnum).c_str());

		repl.clear();
		repl.push_back(replace_t::value_type("$OP_PARAM1", pSqlResult->fieldvalue(4 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM2", pSqlResult->fieldvalue(5 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM3", pSqlResult->fieldvalue(6 + tRowIndex * colnum)));

		// 取得描述的字符串
		C2KString desc(LABEL("EItemAction", nOpType));
		DescItemAction(desc,  repl, funcs);

		xmlbuf.append(
				"<details_info>\n"
				"<info name=\"time\" text=\"%s\">%s</info>\n" 
				"<info name=\"map_id\" text=\"%s\">%s</info>\n"
				"<info name=\"op_type\" text=\"%s\">%s(%d)</info>\n"
				"<info name=\"item_id\" text=\"%s\">%s(%d)</info>\n" 
				"<info name=\"item_num\" text=\"%s\">%s</info>\n"
				"<info name=\"item_total\" text=\"%s\">%s</info>\n"
				"<info name=\"item_guid\" text=\"%s\">%s</info>\n"
				"<info name=\"op_param1\" text=\"%s\">%s</info>\n"
				"</details_info>\n",
				LABEL("EOpItemDef", 0),	
				pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),

				LABEL("EOpItemDef", 1),	
				LABEL("EMapName", atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str())),

				LABEL("EOpItemDef", 2),
				nOpType < 50 ? LABEL("EGlobeDefine", "EM_GET") : LABEL("EGlobeDefine", "EM_LOST"), nOpType,

				LABEL("EOpItemDef", 3),	
				ItemID2Name(nItemID), nItemID,

				LABEL("EOpItemDef", 4),	
				pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(),

				LABEL("EOpItemDef", 5),	
				pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(),

				LABEL("EOpItemDef", 7), 
				pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str(),

				LABEL("EOpItemDef", 6),
				desc.c_str()
		);
	}
		
	xmlbuf.append("</Response>");
	return xmlbuf.length();
}

int FlashGmCmdRoleMoneyOperateResult( CMessage* tpMessage, CMaxString& xmlbuf )
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	//共9个字段
	if ( colnum != 9 )
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	// 取得session
	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
				"<result>1</result>\n"
				"<count>%d</count>\n" 
				"<page>%d</page>\n", rownum, pSession->GetParam3());

	static const proto_func_t funcs[] = 
	{
		{"ETaskName(",  TaskID2Name },
		{"EItemName(",  ItemID2Name },
		{"ENpcName(",   NpcID2Name  },
		{"FromUnixTime(",  FromUnixTime},
		{"EMoneyType(", MoneyType2Name},
		{"EMapName(", 	MapID2Name},
		{"FormatMoney(", FormatMoney},
		{NULL, NULL}
	};

	replace_t repl;
	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		int nOpType = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());

		repl.clear();
		repl.push_back(replace_t::value_type("$OP_PARAM1", pSqlResult->fieldvalue(4 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM2", pSqlResult->fieldvalue(5 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM3", pSqlResult->fieldvalue(6 + tRowIndex * colnum)));

		// 取得描述的字符串
		C2KString desc(LABEL("EMoneyAction", nOpType));
		DescItemAction(desc, repl, funcs);

		xmlbuf.append(
				"<details_info>\n"
				"<info name=\"time\" text=\"%s\">%s</info>\n" 
				"<info name=\"map_id\" text=\"%s\">%s</info>\n"
				"<info name=\"op_type\" text=\"%s\">%s(%d)</info>\n"
				"<info name=\"item_num\" text=\"%s\">%.4f</info>\n"
				"<info name=\"item_total\" text=\"%s\">%.4f</info>\n"
				"<info name=\"op_param1\" text=\"%s\">%s</info>\n"
				"</details_info>\n",
				LABEL("EOpMoneyDef", 0),	
				pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
				LABEL("EOpMoneyDef", 1),	
				LABEL("EMapName", atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str())),
				LABEL("EOpMoneyDef", 2), 
				nOpType < 50 ? LABEL("EGlobeDefine", "EM_GET") : LABEL("EGlobeDefine", "EM_LOST"), nOpType, 
				LABEL("EOpMoneyDef", 3),	
				atoi(pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str())/10000.0,
				LABEL("EOpMoneyDef", 4),	
				atoi(pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str())/10000.0,
				LABEL("EOpMoneyDef", 5),	
				desc.c_str()
					);
	}

	xmlbuf.append("</Response>");
	return xmlbuf.length();
}

int FlashGmCmdRoleExpOperateResult( CMessage* tpMessage, CMaxString& xmlbuf )
{
    LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	//共9个字段
	if ( colnum != 9 )
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	// 取得session
	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
				"<result>1</result>\n"
				"<count>%d</count>\n" 
				"<page>%d</page>\n", rownum, pSession->GetParam3());

	static const proto_func_t funcs[] = 
	{
		{"ETaskName(",  TaskID2Name },
		{"EItemName(",  ItemID2Name },
		{"ENpcName(",   NpcID2Name  },
		{"FromUnixTime(",  FromUnixTime},
		{"EMoneyType(", MoneyType2Name},
		{"EMapName(", 	MapID2Name},
		{"FormatMoney(", FormatMoney},
		{NULL, NULL}
	};

	replace_t repl;
	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		int nOpType = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());

		repl.clear();
		repl.push_back(replace_t::value_type("$OP_PARAM1", pSqlResult->fieldvalue(4 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM2", pSqlResult->fieldvalue(5 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM3", pSqlResult->fieldvalue(6 + tRowIndex * colnum)));

		C2KString desc(LABEL("EExpAction", nOpType));
		DescItemAction(desc, repl, funcs);

		xmlbuf.append(
				"<details_info>\n"
				"<info name=\"time\" text=\"%s\">%s</info>\n" 
				"<info name=\"map_id\" text=\"%s\">%s</info>\n"
				"<info name=\"num\" text=\"%s\">%s</info>\n"
				"<info name=\"cur_exp\" text=\"%s\">%s</info>\n"
				"<info name=\"op_param1\" text=\"%s\">%s</info>\n"
				"</details_info>\n",
				LABEL("EOpExpDef", 0),	
				pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
				LABEL("EOpExpDef", 1),	
				LABEL("EMapName", atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str())),
				LABEL("EOpExpDef", 2),	
				pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(),
				LABEL("EOpExpDef", 3),	
				pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(),
				LABEL("EOpExpDef", 4),	
				desc.c_str()
				);
	}

	xmlbuf.append("</Response>");
	return xmlbuf.length();
}

int FlashGmCmdItemUpgradeResult( CMessage* tpMessage, CMaxString& xmlbuf )
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	//共9个字段
	if ( colnum != 9 )
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	// 取得session
	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<Response>\n"
		"<result>1</result>\n"
		"<count>%d</count>\n" 
		"<page>%d</page>\n", rownum, pSession->GetParam3());

	static const proto_func_t funcs[] = 
	{
		{"EItemName(",    ItemID2Name },
		{"EEquipResist(", ID2EquipResist},
		{"EEquipAttack(", ID2EquipAttack},
		{"EEquipAttr(",   ID2EquipAttr},
		{"EBindStatus(",  ID2BindStatus},
		{"EJudgeResult(", ID2JudgeResult},
		{"EElemLevel(",	  ID2ElemLevel},
		{NULL, NULL}
	};

	replace_t repl;
	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		int nItemID = atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());

		unsigned int nWhiteProperty = 0;
		int Attr[8] = { 0 };

		CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(nItemID);
		if ( ptpl != NULL)	
		{
			CTemplateEquipment* pEquipTpl = (CTemplateEquipment*) ptpl;
			for( nWhiteProperty = 0; nWhiteProperty < ARRAY_CNT(pEquipTpl->mUpgProperty); ++ nWhiteProperty)
			{
				if (pEquipTpl->mUpgProperty[nWhiteProperty] == 0) break;
				if ( nWhiteProperty < ARRAY_CNT(Attr)) 
				{
					Attr[nWhiteProperty] = pEquipTpl->mUpgProperty[nWhiteProperty];
				}
			}
		}

		// clear
		repl.clear();

		// upgrade_time, upgrade_type, result, prev_level, curr_level, consume_item1, consume_item2, item_id, item_guid 
		int nOpType = atoi(pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str());
		if ( nOpType == 9) //鉴定白色属性
		{
			char szPrev[24] = {0};
			char szCurr[24] = {0};

			sprintf(szPrev, "%d%06d", nWhiteProperty, atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str()));
			sprintf(szCurr, "%d%06d", nWhiteProperty, atoi(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str()));

			repl.push_back(replace_t::value_type("$PREV", szPrev));
			repl.push_back(replace_t::value_type("$CURR", szCurr));

			if ( Attr[0] != 0)
			{
				repl.push_back(replace_t::value_type("$OP_PARAM1", LABEL("EEquipFixAttr", Attr[0])));
			}else
			{
				repl.push_back(replace_t::value_type("$OP_PARAM1", ""));
			}

			if ( Attr[1] != 0)
			{
				repl.push_back(replace_t::value_type("$OP_PARAM2", LABEL("EEquipFixAttr", Attr[1])));
			}else
			{
				repl.push_back(replace_t::value_type("$OP_PARAM2", ""));
			}
		}else
		{
			repl.push_back(replace_t::value_type("$PREV", pSqlResult->fieldvalue(3 + tRowIndex * colnum)));
			repl.push_back(replace_t::value_type("$CURR", pSqlResult->fieldvalue(4 + tRowIndex * colnum)));
			repl.push_back(replace_t::value_type("$OP_PARAM1", pSqlResult->fieldvalue(5 + tRowIndex * colnum)));
			repl.push_back(replace_t::value_type("$OP_PARAM2", pSqlResult->fieldvalue(6 + tRowIndex * colnum)));
		}

		repl.push_back(replace_t::value_type("$RESULT", pSqlResult->fieldvalue(2 + tRowIndex * colnum)));

		C2KString desc(LABEL("EItemUpgradeType", nOpType));
		DescItemAction(desc, repl, funcs);
		desc.replace(";#t_tpl_0_1", "");


		xmlbuf.append(
			"<details_info>\n"
			"<info name=\"time\" text=\"%s\">%s</info>\n" 
			"<info name=\"item_id\" text=\"%s\">%s(%s)</info>\n" 
			"<info name=\"item_guid\" text=\"%s\">%s</info>\n" 
			"<info name=\"type\" text=\"%s\">%s</info>\n"
			"<info name=\"result\" text=\"%s\">%s</info>\n"
			"</details_info>\n",
			
			LABEL("EItemUpDef", 0),	
			pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),

			LABEL("EItemUpDef", 1),	
			ItemID2Name(nItemID),
			pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str(),

			LABEL("EItemUpDef", 2),	
			pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str(),
			
			LABEL("EItemUpDef", 3),	
			ID2BindStatus(atoi(pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str())),
			
			LABEL("EItemUpDef", 4),	
			desc.c_str()
		);
	}

	xmlbuf.append("</Response>");
	return xmlbuf.length();
}

// 简单描述装备扼要信息
int EquipSimple(const PBItemObject& pbItem, const CTplItem* ptplItem, C2KString& desc)
{
	if (ptplItem == NULL) return 0;

	if ( (ptplItem->mItemType != ITEM_WEAPON) &&  (ptplItem->mItemType != ITEM_ARMOR)
	&&  (ptplItem->mItemType != ITEM_FASHION) &&  (ptplItem->mItemType != ITEM_MAGICWEAPON))
	{
		// 非装备类道具
		return 0;
	}

	CTemplateEquipment* ptpl = (CTemplateEquipment*) ptplItem;

	lk::CString32 szColor("");
	lk::CString32 szBind("");
	lk::CString32 szRequire("");
	lk::CString32 szLevel("");

	szColor.append("%s", LABEL("EColor", ptpl->mShowColour));
	szBind.append("%s",  LABEL("EBindStatus", pbItem.bindstatus()));

	replace_t repl;
	repl.clear();

	repl.push_back(replace_t::value_type("$COLOR", szColor.c_str()));
	repl.push_back(replace_t::value_type("$BIND", szBind.c_str()));
	if ( pbItem.has_itemcontent() )
	{
		const PBItemContent& pbcon = pbItem.itemcontent();
		//if ( pbcon.has_itemequip())
		if (  ptpl->mItemType != ITEM_MAGICWEAPON )
		{
			// $COLOR $REQUIRE级 +$LEVEL $BIND 印($MARK) 符($RUNNE) 镶($JEWEL)
			desc = LABEL("EEquipDescribe", 0);

			lk::CString32 szRunne("");
			lk::CString128 szJewel("");
			lk::CString32 szMark("");
			lk::CString32 szMagic("");

			const PBItemObjEquip& pbEquip = pbcon.itemequip();
			szLevel.append("%d", pbEquip.level());
			szRequire.append("%d", ptpl->mLevelRequire);

			for(size_t i = 0; i < pbEquip.slotnumber(); ++i)
			{
				if ( i != 0)
				{
					szJewel.append("-%d", pbEquip.slotvalue(i));
				}else
				{
					szJewel.append("%s(%d", 
							LABEL("EEquipDescribe", 15), pbEquip.slotvalue(i));
				}
			}

			if ( pbEquip.slotnumber() > 0 )
			{
				szJewel.append(")");
			}

			if ( pbEquip.extratype() > 0 )
			{
				szMark.append("%s(%d)", LABEL("EEquipDescribe", 13), pbEquip.extratype());
			}

			if ( pbEquip.slotspell() )
			{
				szRunne.append("%s(%d)", LABEL("EEquipDescribe", 14), pbEquip.slotspell());
			}

			int nMagicStoneID = pbEquip.magicstoneid();
			int nMagicStoneNum = pbEquip.magicstonenum();
			if ( nMagicStoneNum > 0)
			{
				szMagic.append("%s(%d*%d)", LABEL("EEquipDescribe", 16), nMagicStoneID, nMagicStoneNum);
			}

			repl.push_back(replace_t::value_type("$JEWEL", szJewel.c_str()));
			repl.push_back(replace_t::value_type("$REQUIRE", szRequire.c_str()));
			repl.push_back(replace_t::value_type("$LEVEL", szLevel.c_str()));
			repl.push_back(replace_t::value_type("$MARK", szMark.c_str()));
			repl.push_back(replace_t::value_type("$RUNNE", szRunne.c_str()));
			repl.push_back(replace_t::value_type("$MAGIC", szMagic.c_str()));

		}else // if ( pbcon.has_itemmw())
		{
			desc = LABEL("EMWDescribe", 0);

			const PBItemObjMW& pbMW = pbcon.itemmw();
			szRequire.append("%d", pbMW.mwlevel());

			const PBItemObjEquip& pbEquip = pbcon.itemequip();
			szLevel.append("%d", pbEquip.level());

			repl.push_back(replace_t::value_type("$REQUIRE", szRequire.c_str()));
			repl.push_back(replace_t::value_type("$LEVEL", szLevel.c_str()));
		}
	}

	DescItemAction(desc, repl, NULL);
	
	return desc.length();
}

// 描述装备详细信息
int EquipDetail(const PBItemObject& pbItem, const CTplItem* ptplItem, C2KString& desc)
{
	if (ptplItem == NULL) return 0;

	if (ptplItem->mItemType != ITEM_WEAPON &&  ptplItem->mItemType != ITEM_ARMOR
	&&  ptplItem->mItemType != ITEM_FASHION &&  ptplItem->mItemType != ITEM_MAGICWEAPON) 
	{
		// 非装备类道具
		return 0;
	}

	CTemplateEquipment *ptpl = (CTemplateEquipment*) ptplItem;


	replace_t repl;
	// $COLOR $REQUIRE级 +$LEVEL $BIND 印($MARK) 符($RUNNE) 镶($JEWEL)
	if ( pbItem.has_itemcontent() )
	{
		const PBItemContent& pbcon = pbItem.itemcontent();
		//if ( pbcon.has_itemequip())
		if ( ptpl->mItemType != ITEM_MAGICWEAPON )
		{
			const PBItemObjEquip& pbEquip = pbcon.itemequip();

			// 白色属性
			lk::string<256> szWhiteDesc("");
			for( size_t i = 0; i < ARRAY_CNT(ptpl->mUpgProperty); ++ i)
			{
				int nAttrID = ptpl->mUpgProperty[i];
				if ( nAttrID == 0) break;

				szWhiteDesc.append(LABEL("EEquipDescribe", 1));
				repl.clear();
				repl.push_back(replace_t::value_type("$TPL_ATTR", LABEL("EEquipFixAttr", ptpl->mUpgProperty[i])));
				repl.push_back(replace_t::value_type("$JUDGE", LABEL("EJudgeResult", pbEquip.judge(i)/100)));

				DescItemAction(szWhiteDesc, repl, NULL);
				if ( pbItem.bindstatus() > 1 ) 
				{
					szWhiteDesc.append("%s", LABEL("EEquipDescribe", 2));
				}

				szWhiteDesc.append("<br/>");
			}

			// 精通
			lk::string<256> szElemAttack("");
			if ( pbEquip.has_resatt() && pbEquip.resatt() != 0)
			{
				szElemAttack.append(LABEL("EEquipDescribe", 3));
				repl.clear();	
				repl.push_back(replace_t::value_type("$ELEM_ATTACK", ID2EquipAttack(pbEquip.resatt())));
				repl.push_back(replace_t::value_type("$ELEM_LEVEL", ID2ElemLevel(pbEquip.attlevel())));
				DescItemAction(szElemAttack, repl, NULL);
			}
			
			// 抗性
			lk::string<256> szElemResist("");
			if ( pbEquip.has_resdef() && pbEquip.resdef() != 0)
			{
				szElemResist.append(LABEL("EEquipDescribe", 4));
				repl.clear();	
				repl.push_back(replace_t::value_type("$ELEM_RESIST", ID2EquipResist(pbEquip.resdef())));
				repl.push_back(replace_t::value_type("$ELEM_LEVEL", ID2ElemLevel(pbEquip.deflevel())));
				DescItemAction(szElemResist, repl, NULL);
			}

			// 蓝色附加属性
			lk::string<256> szBlueAttr("");
			lk::string<256> szGreenAttr("");
			for(int i = 0; i < pbEquip.property_size(); ++i)
			{
				// 蓝色附加属性
				int nAttrID = pbEquip.property(i);
				if ( nAttrID == 0) break;

				CTemplateProperty *pAttrTpl = (CTemplateProperty *) CDataStatic::SearchTpl(nAttrID);
				if ( pAttrTpl == NULL || pAttrTpl->mArea == 1) 
				{
					szBlueAttr.append(LABEL("EEquipDescribe", 5));
					repl.clear();	
					repl.push_back(replace_t::value_type("$VALUE", ID2EquipAttr(nAttrID)));
					DescItemAction(szBlueAttr, repl, NULL);
				}else
				{
					// 绿色套装附加属性
					szGreenAttr.append(LABEL("EEquipDescribe", 7));
					repl.clear();	
					repl.push_back(replace_t::value_type("$VALUE", ID2EquipAttr(nAttrID)));
					DescItemAction(szGreenAttr, repl, NULL);
				}
			}

			// 孔上镶嵌宝石及属性
			lk::string<256> szJewelAttr("");
			for(size_t i = 0; i < pbEquip.slotnumber(); ++i)
			{
				int nJewelID = pbEquip.slotvalue(i);
				if ( nJewelID != 0 )
				{
					szJewelAttr.append(LABEL("EEquipDescribe", 6));
					CTplItemJewel* pJewelTpl = (CTplItemJewel *) CDataStatic::SearchTpl(nJewelID);
					if ( pJewelTpl != NULL)
					{
						repl.clear();
						repl.push_back(replace_t::value_type("$VALUE", ID2EquipAttr(pJewelTpl->mProperty)));
						DescItemAction(szJewelAttr, repl, NULL);
					}else
					{
						repl.clear();
						repl.push_back(replace_t::value_type("$VALUE", ItemID2Name(nJewelID)));
						DescItemAction(szJewelAttr, repl, NULL);
					}
				}else	
				{
					szJewelAttr.append("%s", LABEL("EEquipDescribe", 11));
				}
			}

			// 点金
			int nMagicStoneID = pbEquip.magicstoneid();
			int nMagicStoneNum = pbEquip.magicstonenum();

			lk::string<256> szMagicAttr("");
			if ( nMagicStoneNum != 0)
			{
				szMagicAttr = LABEL("EEquipDescribe", 8);
				CTplItemMagicStone* pMagicStoneTpl = (CTplItemMagicStone *) CDataStatic::SearchTpl(nMagicStoneID);

				repl.clear();
				if ( pMagicStoneTpl) 
				{
					repl.push_back(replace_t::value_type("$VALUE", 
								ID2EquipAttr(pMagicStoneTpl->mAttribute[nMagicStoneNum-1])));
				} else
				{
					repl.push_back(replace_t::value_type("$VALUE", ItemID2Name(nMagicStoneID)));
				}
				repl.push_back(replace_t::value_type("$DIAN_LEVEL", LABEL("EMagicLevel", nMagicStoneNum)));
				DescItemAction(szMagicAttr, repl, NULL);
			}

			// 印鉴
			lk::string<256> szMarkAttr("");
			if ( pbEquip.extratype() > 0)
			{
				szMarkAttr.append(LABEL("EEquipDescribe", 9));
				repl.clear();
				repl.push_back(replace_t::value_type("$VALUE", LABEL("EMarkLabel", pbEquip.extratype())));
				DescItemAction(szMarkAttr, repl, NULL);
			}

			// 绑定
			lk::string<256> szBindAttr("");
			szBindAttr.append(LABEL("EEquipDescribe", 10));
			repl.clear();
			repl.push_back(replace_t::value_type("$VALUE", ID2BindStatus(pbItem.bindstatus())));
			DescItemAction(szBindAttr, repl, NULL);

			// 星
			lk::string<256> szShowStar("");
			if ( pbEquip.level() > 0)
			{
				char szLevel[32] = {0};
				sprintf(szLevel, "%d", pbEquip.level());

				szBindAttr.append(LABEL("EEquipDescribe", 12));
				repl.clear();
				repl.push_back(replace_t::value_type("$LEVEL", szLevel));
				DescItemAction(szBindAttr, repl, NULL);
			}

			desc.append("%s%s%s%s%s%s%s%s%s%s",
					szWhiteDesc.c_str(), szElemAttack.c_str(), szElemResist.c_str(), 
					szBlueAttr.c_str(), szJewelAttr.c_str(), szGreenAttr.c_str(), 
					szMagicAttr.c_str(), szMarkAttr.c_str(), szBindAttr.c_str(), 
					szShowStar.c_str());
		}else // if ( pbcon.has_itemmw())
		{
			const PBItemObjMW& pbMW = pbcon.itemmw();

			lk::string<256> szValue("");
			char szNumber[32] = {0};

			// 法宝等级
			repl.clear();

			sprintf(szNumber, "%d", pbMW.mwlevel());
			repl.push_back(replace_t::value_type("$LEVEL", szNumber));

			szValue.append(LABEL("EMWDescribe", 1));
			DescItemAction(szValue, repl, NULL);

			desc.append(szValue.c_str());
			// 法宝经验
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%llu", pbMW.exp());
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 2));
			DescItemAction(szValue, repl, NULL);

			desc.append(szValue.c_str());
			// 法力值
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.mp());
			repl.push_back(replace_t::value_type("$MP", szNumber));

			sprintf(szNumber, "%d", pbMW.maxmp());
			repl.push_back(replace_t::value_type("$MAXMP", szNumber));

			szValue.append(LABEL("EMWDescribe", 3));
			DescItemAction(szValue, repl, NULL);

			desc.append(szValue.c_str());
			// 灵气值
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.hp());
			repl.push_back(replace_t::value_type("$HP", szNumber));

			sprintf(szNumber, "%d", pbMW.maxhp());
			repl.push_back(replace_t::value_type("$MAXHP", szNumber));

			szValue.append(LABEL("EMWDescribe", 4));
			DescItemAction(szValue, repl, NULL);

			desc.append(szValue.c_str());
			// 锐力
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.curkeenpoints());
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 5));
			DescItemAction(szValue, repl, NULL);
			
			desc.append(szValue.c_str());
			// 定力
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.curcalmpoints());
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 6));
			DescItemAction(szValue, repl, NULL);
			
			desc.append(szValue.c_str());
			// 法力
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.curmagicpoints());
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 7));
			DescItemAction(szValue, repl, NULL);

			desc.append(szValue.c_str());
			// 灵力
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.curbierpoints());
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 8));
			DescItemAction(szValue, repl, NULL);
			
			desc.append(szValue.c_str());
			// 仙力
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.curimmortalpoints());
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 9));
			DescItemAction(szValue, repl, NULL);

			desc.append(szValue.c_str());
			// 潜能
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.leftpoints());
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 10));
			DescItemAction(szValue, repl, NULL);

			desc.append(szValue.c_str());
			// 修炼值
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.probase());
			repl.push_back(replace_t::value_type("$PROBASE", szNumber));

			sprintf(szNumber, "%d", pbMW.proup() + pbMW.probase());
			repl.push_back(replace_t::value_type("$PROUP", szNumber));

			szValue.append(LABEL("EMWDescribe", 11));
			DescItemAction(szValue, repl, NULL);
			
			desc.append(szValue.c_str());
			// 修炼强度
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d%s", pbMW.proeff()/100, "%");
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 12));
			DescItemAction(szValue, repl, NULL);

			desc.append(szValue.c_str());
			// 法宝灵性
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d%s", pbMW.prospir()/100, "%");
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 13));
			DescItemAction(szValue, repl, NULL);
			
			desc.append(szValue.c_str());
			// 技能槽开启进度
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.processval());
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 14));
			DescItemAction(szValue, repl, NULL);
			
			desc.append(szValue.c_str());
			// 法宝技能点
			repl.clear();
			szValue.clear();

			sprintf(szNumber, "%d", pbMW.leftskillpoints());
			repl.push_back(replace_t::value_type("$VALUE", szNumber));

			szValue.append(LABEL("EMWDescribe", 15));
			DescItemAction(szValue, repl, NULL);
			
			desc.append(szValue.c_str());
			// 法宝技能
			for ( int i = 0; i < pbMW.skillbox_size(); ++ i)
			{
				int nSkillID = pbMW.skillbox(i).skills();
				int nSkillLevel = pbMW.skillbox(i).skillinitlevel() + pbMW.skillbox(i).addedskillpoints();

				repl.clear();
				szValue.clear();

				CTemplateSkill *pTplSkill = (CTemplateSkill *) CDataStatic::SearchSkill(nSkillID, nSkillLevel);
				repl.push_back(replace_t::value_type("$SKILL_NAME",  
							pTplSkill != NULL ? TOKEN2STRING(pTplSkill->mName) : LABEL("EGlobeDefine", "UNKNOWN_NAME")));

				sprintf(szNumber, "%d", nSkillLevel);
				repl.push_back(replace_t::value_type("$SKILL_LEVEL", szNumber));

				szValue.append(LABEL("EMWDescribe", 16));
				DescItemAction(szValue, repl, NULL);

				desc.append(szValue.c_str());
			}
		}

	}

	return desc.length();
}

int FlashGmCmdRecoverRoleEquipResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();
	if ( pSqlResult == NULL || pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	// 查询装备
	if ( pSession->GetParam4() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
				"<Response>\n"
				"<result>1</result>\n"
				"<page>%d</page>\n"
				"<details_info>\n", pSession->GetParam3());

		for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
		{
			// TITLE
			if (tRowIndex == 0)
			{
				xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_RECOVERY_EQUIP"));
			}

			PBItemObject pbItem;
			if ( pbItem.ParseFromArray(pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvaluelen(2 + tRowIndex * colnum)))
			{
				CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(pbItem.itemid());

				C2KString desc("");
				EquipSimple(pbItem, ptpl, desc);

				xmlbuf.append("<info>%u,%s,%s,%s{%s}</info>\n", 
						//(pSession->GetParam3() - 1) * 60 + tRowIndex,
						pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(),
						ItemID2Name(pbItem.itemid()),
						desc.c_str());
			}
		}

		xmlbuf.append("</details_info>\n");
		xmlbuf.append("</Response>");
	}else
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	return xmlbuf.length();
}

// 2010-02-03  重设安全密码
int FlashGmCmdResetRolePassword( CMessage* tpMessage, CMaxString& xmlbuf)
{	
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageResetRolePasswordResponse* pResetPassword = (CMessageResetRolePasswordResponse*) tpMessage->msgpara();

	xmlbuf.append(
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>%d</result>\n</Response>",
		pResetPassword->errcode()
		); 

	return xmlbuf.length();	
}

// DescItemAction
template <int max_width>
lk::string<max_width>& DescItemAction(lk::string<max_width>& desc, const replace_t& repl, const proto_func_t*  pfunc)
{
	// 替换
	replace_t::const_iterator iter = repl.begin();
	for (; iter != repl.end(); ++iter)
	{
		desc.replace(iter->first.c_str(), iter->second.c_str());
	}

	if ( pfunc == NULL) return desc;

	// 函数解释
	for (; pfunc->name != NULL && pfunc->method != NULL;  ++pfunc)
	{
		// 如果函数名称不匹配，继续寻找函数
		char* pstart = NULL;
		while((pstart = strstr(desc.c_str(), pfunc->name)) != NULL)
		{
			// 如果找不到函数的又括弧，则返回
			char * pend = strchr(pstart + strlen(pfunc->name), ')');
			if ( pend == NULL) return desc;

			// 如果函数内的参数不是数字，则返回
			char *endptr = NULL;
			int id = (int) strtoul(pstart  + strlen(pfunc->name), &endptr, 0);
			if ( endptr != pend) return desc;

			// 执行函数, 并进行替换
			const char* szValue = pfunc->method(id);

			// 取得要替换的key, 如果超过了它的长度则返回
			char key[64] = {0};	
			int len = pend - pstart + 1;
			if ( len < 1 || len > (int) (sizeof(key) - 1)) return desc;
			memcpy(key, pstart, pend - pstart + 1);

			// 如果替换失败则返回
			if (desc.replace(key,  szValue) == false)  return desc;
		}
	}

	return desc;
}

int FlashGmCmdCorpsTotalResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( colnum < 1)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<Response>\n"
		"<result>1</result>\n"
		"<page>%d</page>\n"
		"<details_info>\n", pSession->GetParam3());

	C2KString Escaped4Xml("");
	for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
	{
		Escaped4Xml.clear();
		int nType = atoi(pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str());
		switch (nType)
		{
			case 0:
			case 1:
			{
				// TITLE
				if (tRowIndex == 0)
				{
					xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_FAMILY_LIST"));
				}

				// check
				if(colnum != 7) continue;

				escape4xml( pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(), Escaped4Xml);
				xmlbuf.append("<info><![CDATA[%s,%s,%s,%s,%s,%d,%s]]></info>\n", 
						pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(),
						Escaped4Xml.c_str(),
						pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str(),
						FromUnixTime(atoi(pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str())),
						0,
						LABEL("ECamp", atoi(pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str()))
						);

				break;
			}

			case 2:
			{
				// 写TITLE
				if (tRowIndex == 0)
				{
					xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_UNION_LIST"));
				}

				// 判断结果集
				if(colnum != 14) continue;

				// 计算军团数量
				int nMember = 1;
				for ( int i = 4; i < 13; ++i)
				{
					if ( atoi(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str()) != 0) nMember ++;	
				}

				escape4xml( pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(), Escaped4Xml);
				xmlbuf.append("<info><![CDATA[%s,%s,%d,%s,%s]]></info>\n", 
						pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(),
						Escaped4Xml.c_str(),
						nMember,
						pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str(),
						LABEL("ECamp", atoi(pSqlResult->fieldvalue(13 + tRowIndex * colnum).c_str()))
						);

				break;
			}

			case 3:
			{
				//TITLE
				if (tRowIndex == 0)
				{
					xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_COUNTRY_LIST"));
				}

				// CHECK
				// type, country_id, create_role_name, npc_id, map_id, owner_id, occupy_time, welfare_time, corps_name
				if(colnum != 9) continue;

				int nCampID = atoi(pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str());
				int nMapID = atoi(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str());

				int nCountryList[] = {62, 68, 69, 70, 45};
				unsigned int nCountryID = 0; 
				for (; nCountryID < sizeof(nCountryList) / sizeof(int); ++nCountryID)
				{
					if (nCountryList[nCountryID] == nMapID)
					{
						break;
					}
				}

				// CONTENT
				xmlbuf.append("<info><![CDATA[%d.%d,%s,%s,%s,%s,%s]]></info>\n", 
						nCampID, nCountryID,
						LABEL("ECamp", nCampID),
						LABEL("EMapName", nMapID),
						pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(),
						FromUnixTime(atoi(pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str())),
						pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str()
						);

				break;
			}

			default:
			{
				break;
			}
		}
	}

	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");

	return xmlbuf.length();	
}

int FlashGmCmdCorpsDetailResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	size_t colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( rownum != 1 || colnum < 1)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>1</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<Response>\n"
		"<result>1</result>\n"
		"<details_info>\n");
			
	xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_COMMON"));
	int nType = atoi(pSqlResult->fieldvalue(0).c_str());
	switch (nType)
	{
		// Family
		case 1:
		{
			Tokens token = StrSplit( CGmServer::GetSingleton().UmsFamilyFields(), ",");
			for ( size_t i = 1; i < colnum && i <= token.size(); ++i)
			{
				if(strcmp(token[i-1].c_str(), "family_shopinfo") == 0) 
				{
					PBLeagueShopsListInfo pbShop;
					if( pbShop.ParseFromArray(pSqlResult->fieldvalue(i).c_str(), pSqlResult->fieldvalue(i).length()))
					{
						protobuf2xml(pbShop, xmlbuf, Dformat);
					}
				}else
				{
					xmlbuf.append("<info>");
					Cformat(xmlbuf, "UMS_FAMILY", token[i-1].c_str(), pSqlResult->fieldvalue(i).c_str());
					xmlbuf.append("</info>\n");
				}
			}

			break;
		}

		// Corps
		case 0:
		{
			Tokens token = StrSplit( CGmServer::GetSingleton().UmsCorpsFields(), ",");
			for ( size_t i = 1; i < colnum && i <= token.size(); ++i)
			{
				if(strcmp(token[i-1].c_str(), "family_info") == 0) 
				{
					PBCorpsFamilyInfoList pbFamily;
					if( pbFamily.ParseFromArray(pSqlResult->fieldvalue(i).c_str(), pSqlResult->fieldvalue(i).length()))
					{
						protobuf2xml(pbFamily, xmlbuf, Dformat);
					}
				}else if(strcmp(token[i-1].c_str(), "corps_shopinfo") == 0) 
				{
					PBLeagueShopsListInfo pbShop;
					if( pbShop.ParseFromArray(pSqlResult->fieldvalue(i).c_str(), pSqlResult->fieldvalue(i).length()))
					{
						protobuf2xml(pbShop, xmlbuf, Dformat);
					}
				}
				else
				{
					xmlbuf.append("<info>");
					Cformat(xmlbuf, "UMS_CORPS", token[i-1].c_str(), pSqlResult->fieldvalue(i).c_str());
					xmlbuf.append("</info>\n");
				}
			}

			break;
		}

		// Union
		case 2:
		{
			Tokens token = StrSplit( CGmServer::GetSingleton().UmsUnionFields(), ",");
			for ( size_t i = 1; i < colnum && i <= token.size(); ++i)
			{
				xmlbuf.append("<info>");
				Cformat(xmlbuf, "UMS_UNION", token[i-1].c_str(), pSqlResult->fieldvalue(i).c_str());
				xmlbuf.append("</info>\n");
			}

			break;
		}

		// World
		case 3:
		{
			Tokens token = StrSplit( CGmServer::GetSingleton().UmsWorldFields(), ",");
			for ( size_t i = 1; i < colnum && i <= token.size(); ++i)
			{
				if(strncmp(token[i-1].c_str(), "official", strlen("official")) == 0) 
				{
					PBOfficialList pbOffficial;
					if( pbOffficial.ParseFromArray(pSqlResult->fieldvalue(i).c_str(), pSqlResult->fieldvalue(i).length()))
					{
						protobuf2xml(pbOffficial, xmlbuf, Dformat);
					}
				}else
				{
					xmlbuf.append("<info>");
					Cformat(xmlbuf, "UMS_WORLD", token[i-1].c_str(), pSqlResult->fieldvalue(i).c_str());
					xmlbuf.append("</info>\n");
				}
			}

			break;
		}
	}

	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");

	return xmlbuf.length();	
}


int FlashGmCmdFamilyLogResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	size_t colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( rownum == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
			"<result>1</result>\n"
			"<details_info>\n");
		xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_FAMILY_LOG"));
		xmlbuf.append("</details_info>\n");
		xmlbuf.append("</Response>");

		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<Response>\n"
		"<result>1</result>\n"
		"<details_info>\n");

	xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_FAMILY_LOG"));
	replace_t repl;
	for ( int tRowIndex = 0; tRowIndex < rownum; ++tRowIndex)
	{
		repl.clear();
		repl.push_back(replace_t::value_type("$OP_PARAM1", pSqlResult->fieldvalue(5 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM2", pSqlResult->fieldvalue(6 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM3", pSqlResult->fieldvalue(7 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OPER", pSqlResult->fieldvalue(1 + tRowIndex * colnum)));

		int nOpType = atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str());
		int nAttrID = atoi(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str());

		// 取得描述的字符串
		C2KString desc(LABEL("EFamilyAction", nOpType*10 + nAttrID));
		DescItemAction(desc, repl, NULL);

		xmlbuf.append("<info>%s,%s</info>\n", 
				pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(), desc.c_str());
	}

	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");

	return xmlbuf.length();	
}

int FlashGmCmdCorpsLogResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	size_t colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( colnum < 8)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<Response>\n"
		"<result>1</result>\n"
		"<details_info>\n");

	xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_FAMILY_LOG"));
	replace_t repl;
	for ( int tRowIndex = 0; tRowIndex < rownum; ++tRowIndex)
	{
		repl.clear();
		repl.push_back(replace_t::value_type("$OP_PARAM1", pSqlResult->fieldvalue(5 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM2", pSqlResult->fieldvalue(6 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OP_PARAM3", pSqlResult->fieldvalue(7 + tRowIndex * colnum)));
		repl.push_back(replace_t::value_type("$OPER", pSqlResult->fieldvalue(1 + tRowIndex * colnum)));

		int nOpType = atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str());
		int nAttrID = atoi(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str());

		// 取得描述的字符串
		C2KString desc(LABEL("ECorpsAction", nOpType*10 + nAttrID));
		DescItemAction(desc, repl, NULL);

		xmlbuf.append("<info>%s,%s</info>\n", 
				pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(), desc.c_str());
	}

	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");

	return xmlbuf.length();	
}

int FlashGmCmdQueryTaskResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	size_t colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( rownum == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
				"<Response>\n"
				"<result>1</result>\n"
				"<details_info>\n");
		xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_ROLE_TASK"));
		xmlbuf.append("</details_info>\n");
		xmlbuf.append("</Response>");

		return xmlbuf.length();
	}

	if ( colnum < 8)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>");
		return xmlbuf.length();
	}          

	// 取得session
	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
			"<result>1</result>\n"
			"<page>%d</page>\n"
			"<details_info>\n", pSession->GetParam3());

	xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_ROLE_TASK"));

	for ( int tRowIndex = 0; tRowIndex < rownum; ++tRowIndex)
	{          
		//  FROM_UNIXTIME(op_time), line_id, repetion_id, map_id, metier_id, role_level, task_id, op_type 
		//  时间,ServerID,副本ID,地图,等级,操作,任务ID,任务名称
		xmlbuf.append("<info>%s,%s,%s,%s,%s,%s,%s,%s</info>\n",
				pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(), 
				LABEL("ELineID", atoi(pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str())), 
				pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(), 
				LABEL("EMapName", atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str())), 
				pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(), 
				LABEL("ETaskAction", atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str())),
				pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str(), 
				TaskID2Name(atoi(pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str())));
	}

	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");

	return xmlbuf.length();	
}

int FlashGmCmdQueryDeathResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	size_t colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( rownum == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
				"<Response>\n"
				"<result>1</result>\n"
				"<details_info>\n");
		xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_ROLE_DEATH"));
		xmlbuf.append("</details_info>\n");
		xmlbuf.append("</Response>");

		return xmlbuf.length();
	}

	if ( colnum < 10)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>");
		return xmlbuf.length();
	}          

	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
			"<result>1</result>\n"
			"<page>%d</page>\n"
			"<details_info>\n", pSession->GetParam3());

	xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_ROLE_DEATH"));

	for ( int tRowIndex = 0; tRowIndex < rownum; ++tRowIndex)
	{          
		int nEntityType = atoi(pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str()); 
		if (nEntityType != 0)
		{
			for ( unsigned int i=1; i <= sizeof(int)*8; ++i)
			{
				if ((nEntityType >> i) == 0)
				{
					nEntityType = i;
					break;
				}
			}
		}

		switch(nEntityType)
		{
			case 28:
			case 29:
			case 30:
			case 31:
			{
				int m = strtoul(pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str(), (char**)NULL, 10); 
				// int nLineID = (m >> 16);
				int nMapID = (m & 0x0000FFFF);

				unsigned int n = strtoul(pSqlResult->fieldvalue(9 + tRowIndex * colnum).c_str(), (char**)NULL, 10); 
				int nX = n >> 16;  
				int nY = n & 0x0000FFFF;  

				xmlbuf.append("<info>%s,%s,%s,%s,%s,%s,%s,%s,(%d %d)</info>\n",
					pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(), 
					LABEL("ELineID", atoi(pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str())),
					pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(), 
					LABEL("EMapName", atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str())), 
					pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(), 
					LABEL("EMetier", atoi(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str())), 
					LABEL("EEntityType", nEntityType),
					LABEL("EMapName", nMapID), nX, nY); 

				break;
			}

			case 27:
			{
				xmlbuf.append("<info>%s,%s,%s,%s,%s,%s,%s,%s,%s</info>\n",
					pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(), 
					LABEL("ELineID", atoi(pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str())),
					pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(), 
					LABEL("EMapName", atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str())), 
					pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(), 
					LABEL("EMetier", atoi(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str())), 
					LABEL("EEntityType", nEntityType),
					pSqlResult->fieldvalue(9 + tRowIndex * colnum).c_str(), 
					pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());

				break;
			}

			default:
			{
				xmlbuf.append("<info>%s,%s,%s,%s,%s,%s,%s,%s,%s</info>\n",
					pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(), 
					LABEL("ELineID", atoi(pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str())),
					pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(), 
					LABEL("EMapName", atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str())), 
					pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(), 
					LABEL("EMetier", atoi(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str())), 
					LABEL("EEntityType", nEntityType),
					pSqlResult->fieldvalue(9 + tRowIndex * colnum).c_str(), 
					nEntityType == 1 ?
						pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str() :
						NpcID2Name(atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str()))); 
			}
		}
	}

	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");

	return xmlbuf.length();	
}

int FlashGmCmdRoleLevelInfoResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	size_t colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<Response>\n"
		"<result>1</result>\n"
		"<count>%d</count>\n", rownum);

	int nLastUpTime = 0;
	for(int tRowIndex = 0; tRowIndex < rownum ; ++tRowIndex)
	{
		int nUpTime = atoi(pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str());
		if ( tRowIndex == 0 ) nLastUpTime = nUpTime;

		int nDur = nUpTime - nLastUpTime;
		xmlbuf.append(
			"<level_info>\n"
				"<level>%s</level>\n"
				"<up_period>%d Hours %d Minutes %d Seconds</up_period>\n"
				"<up_time>%s</up_time>\n"
			"</level_info>\n",
			 pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
			 nDur/3600, nDur%3600/60, nDur%60,
			 FromUnixTime(nUpTime)
			);

		nLastUpTime = nUpTime;
	}

	xmlbuf.append("</Response>");
	return xmlbuf.length();	
}

int FlashGmCmdQueryStockResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int nSessonType = pSession->GetSessionType();
	const char* pTitle = ( nSessonType == em_dbsession_query_stock1 ? "TITLE_STOCK_LOG1" :
							( nSessonType == em_dbsession_query_stock2 ? "TITLE_STOCK_LOG2" :
					  			( nSessonType == em_dbsession_query_stock3 ? "TITLE_STOCK_LOG3" : NULL)));


	size_t colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	const bool bFieldIsOK = ( nSessonType == em_dbsession_query_stock1 ?   colnum == 11 :
								( nSessonType == em_dbsession_query_stock2 ?  colnum == 8 :
					  				( nSessonType == em_dbsession_query_stock3 ?  colnum == 8 : false)));

	if ( ! bFieldIsOK )
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	if ( rownum == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
				"<Response>\n"
				"<result>1</result>\n"
				"<details_info>\n");
		xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", pTitle));
		xmlbuf.append("</details_info>\n");
		xmlbuf.append("</Response>");

		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
			"<result>1</result>\n"
			"<page>%d</page>\n"
			"<details_info>\n", pSession->GetParam3());

	xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", pTitle));

	for ( int tRowIndex = 0; tRowIndex < rownum; ++tRowIndex)
	{          
		switch ( nSessonType )
		{
			case em_dbsession_query_stock1:
			{
				xmlbuf.append("<info>%s,%s(%s),%s,%s,%s,%s,%s,%s,%s,%s</info>\n",
						pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(),
						LABEL("EStockTrade", atoi(pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str())),
						LABEL("EStockStatus", atoi(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str())),
						pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(9 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(10 + tRowIndex * colnum).c_str());
				break;
			}

			case em_dbsession_query_stock2:
			{
				xmlbuf.append("<info>%s,%s,%s,%s,%s,%s,%s,%s</info>\n",
						pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
				break;
			}

			case em_dbsession_query_stock3:
			{
				xmlbuf.append("<info>%s,%s,%s,%s,%s,%s,%s,%s</info>\n",
						pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str(),
						pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str());
				break;
			}
		}
	}


	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");

	return xmlbuf.length();	

}

int FlashGmCmdQueryLoginResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	size_t colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	if ( rownum == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
				"<Response>\n"
				"<result>1</result>\n"
				"<details_info>\n");
		xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_ROLE_LOGOUT"));
		xmlbuf.append("</details_info>\n");
		xmlbuf.append("</Response>");

		return xmlbuf.length();
	}

	if ( colnum < 12)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>");
		return xmlbuf.length();
	}          

	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
			"<result>1</result>\n"
			"<page>%d</page>\n"
			"<details_info>\n", pSession->GetParam3());

	xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", "TITLE_ROLE_LOGOUT"));

	for ( int tRowIndex = 0; tRowIndex < rownum; ++tRowIndex)
	{          
		unsigned int nIP = strtoul(pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str(), (char**)NULL, 10);
		const unsigned char* p = (const unsigned char*) &nIP;

		int nKickReason = atoi(pSqlResult->fieldvalue(10 + tRowIndex * colnum).c_str());
		int nKickCode = atoi(pSqlResult->fieldvalue(11 + tRowIndex * colnum).c_str());

		xmlbuf.append("<info>%s,%s,%s,%s,%d.%d.%d.%d,%s,%s,%s,%s,%s</info>\n",
				LABEL("ELineID", atoi(pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str())),
				pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(), 
				LABEL("EMapName", atoi(pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str())), 
				pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str(), 
				*p, *(p+1), *(p+2), *(p+3),
				pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str(), 
				pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str(), 
				LABEL("EMLeaveReason", atoi(pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str())), 
				LABEL("EMSaveResult", atoi(pSqlResult->fieldvalue(9 + tRowIndex * colnum).c_str())), 
				LABEL("EMKickReason", nKickCode != 0 ? abs(nKickCode) : abs(nKickReason)));
	}


	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");

	return xmlbuf.length();	
}


int FlashGmCmdQueryYBStallResult(CMessage* tpMessage, CMaxString& xmlbuf)
{
	LK_ASSERT(tpMessage != NULL, return -1);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return -1);

	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	if ( pSqlResult->resultcode() == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int nSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nSessionID);
	if ( pSession == NULL)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	int nSessonType = pSession->GetSessionType();
	const char* pTitle = "TITLE_YB_STALL";


	size_t colnum = pSqlResult->colcount();
	int rownum = pSqlResult->rowcount();

	const bool bFieldIsOK = (  colnum == 10 );

	if ( ! bFieldIsOK )
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Response>\n<result>0</result>\n</Response>"); 
		return xmlbuf.length();
	}

	if ( rownum == 0)
	{
		xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
				"<Response>\n"
				"<result>1</result>\n"
				"<details_info>\n");
		xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", pTitle));
		xmlbuf.append("</details_info>\n");
		xmlbuf.append("</Response>");

		return xmlbuf.length();
	}

	xmlbuf.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			"<Response>\n"
			"<result>1</result>\n"
			"<page>%d</page>\n"
			"<details_info>\n", pSession->GetParam3());

	xmlbuf.append("<info>%s</info>\n", LABEL("ETitle", pTitle));

	for ( int tRowIndex = 0; tRowIndex < rownum; ++tRowIndex)
	{          
		xmlbuf.append("<info>%s,%s,%s,%s,%s,%s,%s,%s,%s,%s</info>\n",
				pSqlResult->fieldvalue(0 + tRowIndex * colnum).c_str(),
				pSqlResult->fieldvalue(1 + tRowIndex * colnum).c_str(),
				pSqlResult->fieldvalue(2 + tRowIndex * colnum).c_str(),
				pSqlResult->fieldvalue(3 + tRowIndex * colnum).c_str(),
				pSqlResult->fieldvalue(4 + tRowIndex * colnum).c_str(),
				ItemID2Name( atoi(pSqlResult->fieldvalue(5 + tRowIndex * colnum).c_str())),
				pSqlResult->fieldvalue(6 + tRowIndex * colnum).c_str(),
				pSqlResult->fieldvalue(7 + tRowIndex * colnum).c_str(),
				pSqlResult->fieldvalue(8 + tRowIndex * colnum).c_str(),
				pSqlResult->fieldvalue(9 + tRowIndex * colnum).c_str());

	}


	xmlbuf.append("</details_info>\n");
	xmlbuf.append("</Response>");

	return xmlbuf.length();	

}



