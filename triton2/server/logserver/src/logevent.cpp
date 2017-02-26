#include "logevent.h"
#include "logctrl.h"
#include "charset.h"

#include "google/protobuf/text_format.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/unknown_field_set.h"
#include "google/protobuf/io/tokenizer.h"
#include "google/protobuf/stubs/strutil.h"
#include "gmmessage_pb.hxx.pb.h"

#define TOKEN2STRING(a) CLogCtrl::GetSingleton().Token2String((a))

#define EVENT_BLOCK_SIZE 100
#define EVENT_DROPITEM_SIZE 20

bool IsValuable(const PBItemObject &pbItem);
extern "C"
{
	unsigned long crc32(unsigned long crc, const unsigned char *buf, unsigned int len);
}

CLogEvent::CLogEvent(const char* szTableName, const int nSplit)
{
	memcpy(m_szTableName, szTableName, sizeof(m_szTableName));
	m_szTableName[sizeof(m_szTableName) -1 ] = 0;

	// 分表
	m_nSplit = nSplit;
	m_pInsBuf = new CMaxString[nSplit];

	for (int i = 0; i < nSplit; ++ i)
	{
		m_pInsBuf[i].clear();
	}

	ClearStatInfo();
}

CLogEvent::~CLogEvent()
{
	delete [] m_pInsBuf;
}

EMAppendResult CLogEvent::AppendStatInfoMessage( int nIndex, CMessage * pMessage)
{
	if ( pMessage == NULL || pMessage->msgpara() == 0 || nIndex < 0 ) return em_append_fail;

	CMaxString& refbuf = m_pInsBuf[nIndex%m_nSplit];	
	CMessageServerStatInfoNotify* message = (CMessageServerStatInfoNotify*) pMessage->msgpara();			
	
	time_t nSeconds= time(0);
	struct tm tm;
	struct tm * ptm = localtime_r(&nSeconds, &tm);
	char szDate[12] = {0};
	strftime(szDate, sizeof(szDate), "%Y%m%d", ptm);

	for ( int i=0; i < message->statinfo_size(); ++i)
	{
		int nServerID = message->serverid();
		const PBCatalog& pbCatalog = message->statinfo(i);
		for (int j=0; j < pbCatalog.attr_size(); ++j) 
		{
			if (refbuf.empty())
			{
				mLogStat.m_uiMsgID = pMessage->msghead().messageid();
				refbuf.append("INSERT INTO %s_%s_%02d VALUES ( NULL, ", szDate, m_szTableName, nIndex % m_nSplit + 1);
			} else
			{
				refbuf.append(", ( NULL, ");
			}

			refbuf.append("%d,%d,%d,%d,%lld", nSeconds,
					nServerID, pbCatalog.id(), pbCatalog.attr(j).id(), pbCatalog.attr(j).value()); 

			refbuf.append(")");
		}
	}

	if( (++ mLogStat.m_uiRecvNum) >= 1)
	{
		return em_append_full;
	}
	
	return em_append_ok;
}

EMAppendResult CLogEvent::AppendItemGhostMessage( int nIndex, CMessage * pMessage)
{
	if ( nIndex < 0 ) return em_append_fail;

	CMaxString& refbuf = m_pInsBuf[nIndex%m_nSplit];	

	time_t nSeconds= time(0);
	//CMessageLogValuableEquip* message = (CMessageLogValuableEquip*) pMessage->msgpara();
	CMessageSavePlayerItemInfoRequest* message = (CMessageSavePlayerItemInfoRequest*) pMessage->msgpara();
	for (int j=0; j < message->itembox_size(); ++j)
	{
		const PBItemBox& pbBox = message->itembox(j);
		int nPackType = message->packageidx(j);
		for ( int i = 0; i < pbBox.itemobjects_size(); ++i)
		{
			if (! IsValuable(pbBox.itemobjects(i))) continue;

			// 约定5: 表的第一个字段必须是自增字段
			if (refbuf.empty())
			{
				mLogStat.m_uiMsgID = pMessage->msghead().messageid();

				struct tm tm;
				struct tm * ptm = localtime_r(&nSeconds, &tm);

				char szDate[12];

				strftime(szDate, sizeof(szDate), "%Y%m%d", ptm);
				refbuf.append("REPLACE INTO %s_%s_%02d VALUES ( ", szDate, m_szTableName, nIndex % m_nSplit + 1);

			} else
			{
				refbuf.append(", ( ");
			}

			const int const_package_size = 512;
			static char szProto[const_package_size] = {0};
			static char szEscProto[const_package_size] = {0};

			bool OK = pbBox.itemobjects(i).SerializeToArray(szProto, sizeof(szProto)) 
				&& escape_string_for_mysql( szEscProto, sizeof(szEscProto)-1, szProto, pbBox.itemobjects(i).ByteSize());

			unsigned int nCrc32 = crc32(0, (unsigned char*) szProto, pbBox.itemobjects(i).ByteSize());

			refbuf.append("%u, %d,%d,%lld,'%s'", OK ? nCrc32 : 0,
					message->charid(), nSeconds, pbBox.itemobjects(i).objguid(), OK ? szEscProto : NULL);

			refbuf.append(")");

			mLogStat.m_uiRecvNum ++;
		}
	}

	if( refbuf.length() > 0 )
	{
		return em_append_full;
	}
	else
	{
		return em_append_ok;
	}
}

bool IsValuable(const PBItemObject &pbItem)
{
	// 首先得是装备
	CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(pbItem.itemid());
	if (ptpl == NULL) return false;

	// if (!ptpl->IsEquipable()) return false;
	if (ptpl->mItemType != ITEM_WEAPON &&  ptpl->mItemType != ITEM_ARMOR
			&&  ptpl->mItemType != ITEM_FASHION &&  ptpl->mItemType != ITEM_MAGICWEAPON)
	{
		return false;
	}

	// 如果是手工绑定状态直接返回ture
	if ( pbItem.bindstatus() > 1) return true;

	// 如果是金色的或者紫色的装备
	CTemplateEquipment* pTplEquip =(CTemplateEquipment *) ptpl;

	// 非紫金则返回false
	if ( pTplEquip && pTplEquip->mShowColour < 2 )
	{
		// if ( pbEquip.level() >  5) return true; // 如果星级超过5星
		return false;
	}

	if (! pbItem.has_itemcontent() ) return false;
	const PBItemContent& pbcon = pbItem.itemcontent();

	if ( pbcon.has_itemmw())
	{
		const PBItemObjMW& pbMw = pbcon.itemmw();
		return true;
	}else
	if ( pbcon.has_itemequip())
	{
		if (pTplEquip && pTplEquip->mLevelRequire < 45 ) return false;

		const PBItemObjEquip& pbEquip = pbcon.itemequip();

		if ( pbEquip.slotnumber() > 0)  return true;	// 如果开槽了
		if ( pbEquip.extratype() > 0) return true;		// 如果有印鉴
		if ( pbEquip.slotspell() > 0) return true;		// 如果有符文
		if ( pbEquip.level() >  4) return true; 		// 如果等级>0级
	}else
	{
		return false;
	}
	
	return true;
}

EMAppendResult CLogEvent::AppendDropItemMessage( int nIndex, CMessage * pMessage)
{
	if ( nIndex < 0 ) return em_append_fail;

	CMaxString& refbuf = m_pInsBuf[nIndex%m_nSplit];	

	CMessageLogOrgeDropItem* message = (CMessageLogOrgeDropItem*) pMessage->msgpara();
	for (int i=0; i < message->dropitem_size(); ++i)
	{
		if ( CLogCtrl::GetSingleton().IsFilterDrop(message->dropitem(i).itemid())) continue;

		// 约定5: 表的第一个字段必须是自增字段
		if (refbuf.empty())
		{
			mLogStat.m_uiMsgID = pMessage->msghead().messageid();

			time_t nSeconds= time(0);
			struct tm tm;
			struct tm * ptm = localtime_r(&nSeconds, &tm);

			char szDate[12];

			strftime(szDate, sizeof(szDate), "%Y%m%d", ptm);
			refbuf.append("INSERT INTO %s_%s_%02d VALUES ( NULL, ", szDate, m_szTableName, nIndex % m_nSplit + 1);

		} else
		{
			refbuf.append(", ( NULL, ");
		}

		refbuf.append("%d,%d,%d,%d,%d,%lld,%d,%d,%d,%d", 
				message->npcid(), message->npcindex(), message->npclevel(),
				message->optime(), message->lineid(), message->repetionid(),
				message->mapid(), message->dropitem(i).itemid(), 
				message->dropitem(i).itemtype(), message->dropitem(i).itemlevel());

		refbuf.append(")");
	}


	mLogStat.m_uiRecvNum ++;
	if(( refbuf.length() > (refbuf.size() - 2048)) 
	|| (mLogStat.m_uiRecvNum % EVENT_DROPITEM_SIZE) == 0 )
	{
		return em_append_full;
	}
	else
	{
		return em_append_ok;
	}
}

EMAppendResult CLogEvent::AppendMessage(int nIndex, CMessage* pMessage)
{
	if ( nIndex < 0 ) return em_append_fail;

	CMaxString& refbuf = m_pInsBuf[nIndex%m_nSplit];	

	// 约定5: 表的第一个字段必须是自增字段
	if (refbuf.empty())
	{
		mLogStat.m_uiMsgID = pMessage->msghead().messageid();

		time_t nSeconds= time(0);
		struct tm tm;
		struct tm * ptm = localtime_r(&nSeconds, &tm);

		char szDate[12];

		strftime(szDate, sizeof(szDate), "%Y%m%d", ptm);
		refbuf.append("INSERT INTO %s_%s_%02d VALUES ( NULL, ", szDate, m_szTableName, nIndex % m_nSplit + 1);

	} else
	{
		refbuf.append(", ( NULL, ");
	}

	Message* message = (Message*) pMessage->msgpara();
	PrintRecord(*message, refbuf);

	refbuf.append( " ) ");

	// 假设每条记录转换为SQL语句, 不超过2048字节
	//if ( refbuf.length() > refbuf.size() - 2048) 
	//{
	//	return em_append_full;
	//}else
	//{
	//	return em_append_ok;
	//}

	mLogStat.m_uiRecvNum ++;

	// 聊天消息特殊处理
	if( pMessage->msghead().messageid() == ID_G2L_ROLE_CHAT )
	{
		if( mLogStat.m_uiRecvNum % 10  == 0 )
		{
			return em_append_full;
		}
	}
	else
	{
		// 按sql条数计算插入点,暂定100条，考虑作为配置
		// 一个线程可能操作多种日志，因此此处用%不会生效
		if(( refbuf.length() > (refbuf.size() - 2048)) 
			|| (mLogStat.m_uiRecvNum % EVENT_BLOCK_SIZE) == 0)
		{   
			return em_append_full;
		}
		else
		{
			return em_append_ok;
		}
	}

}

const char* CLogEvent::GetSql(int nIndex)
{
	if ( nIndex < 0 || m_pInsBuf == NULL) return NULL;

	return m_pInsBuf[nIndex%m_nSplit].c_str();
}

void CLogEvent::ClearSql(int nIndex)
{
	if ( nIndex < 0 ) return;

	m_pInsBuf[nIndex%m_nSplit].clear();
}

void CLogEvent::PrintRecord(const Message& message, CMaxString& generator)
{
	const Reflection* reflection = message.GetReflection();
	std::vector<const FieldDescriptor*> fields;
	reflection->ListFields(message, &fields);

	for (unsigned int i = 0; i < fields.size(); i++)
	{
		if ( i != 0 ) generator.append(", ");
		PrintField(message, reflection, fields[i], generator);
	}
}

void CLogEvent::PrintField(const Message& message, 
		const Reflection* reflection, const FieldDescriptor* field, CMaxString& generator)
{
	// 字段不能是重复的
	LK_ASSERT(!(field->is_repeated()) && reflection->HasField(message, field), return);

	// TODO: 如果没有这个字段，建议取默认值
	switch (field->cpp_type())
	{
		#define OUTPUT_FIELD(CPPTYPE, METHOD, TO_STRING)	\
		case FieldDescriptor::CPPTYPE_##CPPTYPE:	\
		{											\
			generator.append("%s", TO_STRING(reflection->Get##METHOD(message, field)).c_str()); \
			break;  \
		}           \

		OUTPUT_FIELD( INT32,  Int32, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD( INT64,  Int64, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD(UINT32, UInt32, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD(UINT64, UInt64, ::google::protobuf::SimpleItoa);
		OUTPUT_FIELD( FLOAT,  Float, ::google::protobuf::SimpleFtoa);
		OUTPUT_FIELD(DOUBLE, Double, ::google::protobuf::SimpleDtoa);
		#undef OUTPUT_FIELD

		case FieldDescriptor::CPPTYPE_STRING:
		{
			std::string scratch;
			const std::string& value = reflection->GetStringReference(message, field, &scratch);

			/*
			char src[256] = {0};
			char szToken[256] = {0};

			int len = value.length() > sizeof(src) - 1 ? sizeof(src) - 1 : value.length();

			strncpy(src, value.c_str(), len);
			src[len] = 0;

			strncpy(szToken, TOKEN2STRING(src), sizeof(szToken));
			szToken[sizeof(szToken)-1] = 0;

			*/

			char src[512] = {0};
			int  nlen = 0;
			char dst[512] = {0};
			char *pdst = dst;


			if ( value.length() < sizeof(src))
			{
				nlen = value.length();
				memcpy(src, value.c_str(), value.length());
			}

			EscapeDataString(&pdst, sizeof(dst)-1, src, nlen);

			//generator.append("'%s'", ::google::protobuf::CEscape(value).c_str());
			generator.append("'%s'", dst);
			break;
		}

		// 约定1: 篵ool 类型的值取 1 或者 0
		case FieldDescriptor::CPPTYPE_BOOL:
		{
			generator.append("%d", reflection->GetBool(message, field) ? 1 : 0 );
			break;
		}

		// 约定2: 枚举类型，取枚举值
		case FieldDescriptor::CPPTYPE_ENUM:
		{
			//TODO:
			generator.append("%d", reflection->GetEnum(message, field)->number());
			break;
		}

		// 约定3: 如果是消息类型，则展开消息
		case FieldDescriptor::CPPTYPE_MESSAGE:
		{
			PrintRecord(reflection->GetMessage(message, field), generator);
			break;
		}
	}
}



