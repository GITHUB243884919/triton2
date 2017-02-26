#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <list>

#include "base.h"
#include "lk_string.h"
#include "mysql.h"
#include "property_pb.hxx.pb.h"
#include "mailmessage_pb.hxx.pb.h"
#include "corpsinfo_pb.hxx.pb.h"
#include "tinyxml.h"
#include "charset.h"

#include "lk_proto.h"
#include "lk_hashmap.h"

struct single_role_t
{
	int account_id;
	int role_id;
	int camp_id;

	single_role_t(int first, int second, int third)
	{
		account_id = first;
		role_id = second;
		camp_id = third;
	};
};

typedef std::list < lk::CString128 > CTokenList;
typedef std::list< struct single_role_t >  role_info_t;
typedef lk::hash_map<lk::CString32, int, 64> CField2Table;
CField2Table mField2Table;

enum em_op_flag
{
	em_select,
	em_update,
	em_expand,
	em_mail,
};

enum em_op_source
{
	em_db,
	em_stream,
};

void fill_fielddefs(const char* buff, CTokenList& fielddefs);

int which_table (MYSQL& mysql, int nRoleID, int* pnCampID);


int field2xml(const char* szFieldName, CMaxString& xmlbuf, char* pbcode, unsigned int pblen);

int role2xml( MYSQL& mysql, int nTableID, int nRoleID, CTokenList& fielddefs, const char* xmlfile);

int xml2role( MYSQL& mysql, int nTableID, int nRoleID, CTokenList& fielddefs, const char* xmlfile);

int xml2field(MYSQL& mysql, int nTableID, int nRoleID, TiXmlElement* xmlelem);

bool connect_database(MYSQL& mysql, const char* connstr = "mysql_info");

int expand2db( MYSQL&mysql, MYSQL &logdb, int nTableID, int nRoleID, int nCampID, CTokenList& fielddefs, const char* szTable);

int expand4mail( MYSQL&mysql, int nCampID, int nRoleID, CMaxString& stmtins, const char* szTable);
int expand4pack( MYSQL&mysql, int nTableID, int nRoleID, CTokenList& fielddefs, CMaxString& stmtin, const char* szTable );
int expand4corps( MYSQL &mysql, CMaxString& stmtins, const char* szTable);

int all_roles( MYSQL&mysql, role_info_t& lstRoles, const char* szWhere);

//bool DecryptAscIICode(const char* szInput, int nInput, char* szOutput, int* pnOutput);

int stream2xml(int nRoleID, CTokenList& fielddefs, const char* xmlfile);

// xml to mailbox for specail guid
int xml2mail( MYSQL& mysql, int nTableID, int nRoleID, uint64_t guid, const char* xmlfile, int isbind);
int xml2attach(MYSQL& mysql, int nTableID, int nRoleID, TiXmlElement* xmlelem, uint64_t guid, int isbind);

int gItemType = 0;

// ***************************************************************
// Function:       
// Description:    
// Input:   -c column -r role_id -u[pdate] -f xml_file -[e]x[pand] -s[tream] [-g guid -b[ind]]
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************
int main(int argc, char* argv[])
{
	em_op_flag op = em_select;
	em_op_source op_source = em_db;

	int nRoleID = -1;
	const char* xmlfile = NULL;
	const char* szWhere = NULL;
	CTokenList fielddefs;
	role_info_t lstRoles;

	uint64_t guid = 1;

	int result = 0;
	int opterr = 0;
	int isbind = 0;

	while( (result = getopt(argc, argv, "sur:x:f:c:g:w:t:b")) != -1 )
	{
		switch(result)
		{
			// update flag 
			case 'u':
			{
				op = em_update;	
				break;
			}

			// role_id  
			case 'r':
			{
				nRoleID = atoi(optarg);
				break;
			}

			// file input or output xml file name.
			case 'f':
			{
				xmlfile = optarg;	
				break;
			}

			// column std::list, spliter is blankspace, tab or ','
			case 'c':
			{
				fill_fielddefs(optarg, fielddefs);
				break;
			}
				
			// expand colum to db
			case 'x':
			{
				op = em_expand;
				gItemType = atoi(optarg);	
				break;
			}

			// read from stream
			case 's':
			{
				op_source = em_stream;
				break;
			}

			// guid 
			case 'g':
			{
				op = em_mail;
				guid = strtoull(optarg, NULL, 0);
				break;
			}

			// bind or not
			case 'b':
			{
				isbind = 1;
				break;
			}

			// where statment
			case 'w':
			{
				szWhere = optarg;
				break;
			}

			default:
			{
				break;
			}
		}
	}



	mField2Table.initailize();

	mField2Table.insert(CField2Table::value_type("role_id",0));
	mField2Table.insert(CField2Table::value_type("role_hp",0));
	mField2Table.insert(CField2Table::value_type("role_mp",0));
	mField2Table.insert(CField2Table::value_type("role_ap",0));
	mField2Table.insert(CField2Table::value_type("role_basic",0));
	mField2Table.insert(CField2Table::value_type("role_skill",0));
	mField2Table.insert(CField2Table::value_type("role_buff",0));
	mField2Table.insert(CField2Table::value_type("role_cdgroup",0));
	mField2Table.insert(CField2Table::value_type("role_package",0));
	mField2Table.insert(CField2Table::value_type("role_equipment",0));
	mField2Table.insert(CField2Table::value_type("role_storage",0));
	mField2Table.insert(CField2Table::value_type("role_title",0));
	mField2Table.insert(CField2Table::value_type("role_activity",0));
	mField2Table.insert(CField2Table::value_type("role_task_package",0));
	mField2Table.insert(CField2Table::value_type("role_repetion",0));
	mField2Table.insert(CField2Table::value_type("role_pvp",0));

	if ( op != em_expand )
	{	
		if ( xmlfile == NULL || nRoleID == -1)
		{
			printf("usage: %s -r role_id -f filename [ -u ] [ -c column1,column2 ] [ -s ]\n", argv[0]);
			printf("example: \n{\n"
				   "  1. EXPORT role_package and role_storage of charactor id 10001 to file 10001.xml\n"
				   "	%s -r 10001 -f./10001.xml -c\"role_package,role_storage\"\n\n"
				   "  2. IMPORT role_package and role_storage of charactor id 10001 from file 10001.xml\n"
				   "	%s -r 10001 -f./10001.xml -c\"role_package,role_storage\" -u \n\n"
				   "	NOTE: The file ./10001.xml content <role_detail role_id=\"10001\"> must be as same as you want\n\n"
				   "  3. EXPORT all characters' package, storage, equipment and mail attachment to LogDB.YYYYMMDD_STAT_ROLE_ITEM \n"
				   "	%s [-r 10001] -x 0 -c\"role_equipment, role_package, role_storage\"\n\n"
				   "  4. EXPORT all coprs' battle_info to LogDB.LOG_CORPS_BATTLE_INFO \n"
				   "	%s -x 1 \n\n"
				   "  5. SEND guid 2756556788333290760 to character 10001 by mail attachent from file ./10001.xml\n"
				   "	%s -r 10001 -f ./10001.xml -g\"2756556788333290760\" [-b] \n\n"
				   "	NOTE: -b means if it need be binded to the charactor\n\n"
				   "  6. EXPORT all characters' package, storage, equipment and mail attachment to LogDB.YYYYMMDD_STAT_ROLE_EQUIP \n"
				   "	%s [-r 10001] -x 2 -w \"account_id < 10000\" -c\"role_equipment, role_package, role_storage\"\n\n"
				   "  7. EXPORT character 10001 some field from mysql dumpfile \n"
				   "	cat mysqldump.file | %s -r 10001 -f./10001.xml -c\"role_package,role_storage\" -s \n\n"
				   "	NOTE: the blob field must be dump as --hex-blob\n}\n",
				argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);


			return -1;
		}
	}

	MYSQL mysql;
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");

	int nTableID = -1;
	int nCampID = 0;
	if ( op_source == em_db)
	{
		if(!connect_database(mysql))
		{
			printf("connect database error, pls check ../config/dbserver.xml \n");
			return -1;
		}

		nTableID = which_table(mysql, nRoleID, &nCampID);
		if ( nTableID < 0 )
		{
			printf("Can not find the role_id %d\n", nRoleID);
			mysql_close(&mysql);
			return -1;
		}
	}

	if ( op == em_select )
	{
		if ( op_source == em_db)
		{
			role2xml(mysql, nTableID, nRoleID, fielddefs, xmlfile);
		}else
		{
			stream2xml(nRoleID, fielddefs, xmlfile);
		}
	} else if ( op == em_expand)
	{

		MYSQL logdb;
		mysql_init(&logdb);
		mysql_options(&logdb, MYSQL_SET_CHARSET_NAME, "utf8");

		if(!connect_database(logdb, "logdb_info"))
		{
			printf("connect log database error, pls check ../config/dbserver.xml ");
			mysql_close(&mysql);
			return -1;
		}

		// 创建目标表
		char stmtcreate[512];
		char szTable[64];
		if ( gItemType == 1)
		{
			const char* stmtfmt =
				"CREATE TABLE IF NOT EXISTS %s ( log_date timestamp, "
				" npc_id int, npc_status int, war_status int, a_type int, bid_money int, "
				" des_corps_id int, map_id int, time int, corps_id int )";

			sprintf(szTable, "LOG_CORPS_BATTLE_INFO");
			sprintf(stmtcreate, stmtfmt, szTable);
		}else
		{ 
			time_t nSeconds= time(0);
			struct tm tm;
			struct tm * ptm = localtime_r(&nSeconds, &tm);

			char szDate[12];
			strftime(szDate, sizeof(szDate), "%Y%m%d", ptm);

			if ( gItemType == 0)
			{
				sprintf(szTable,  "%s_STAT_ROLE_ITEM", szDate); 
				const char* stmtfmt= 
					"CREATE TABLE IF NOT EXISTS %s(role_id int, where_id int, item_id int, item_num int)";

				sprintf(stmtcreate, stmtfmt, szTable);
			}else
			{
				sprintf(szTable,  "%s_STAT_ROLE_EQUIP", szDate); 
				const char* stmtfmt= 
					"CREATE TABLE IF NOT EXISTS %s( "
						"role_id int, "
						"where_id int, "
						"item_id int, "
						"level int, "
						"bind int, "
						"runne int, "
						"magic_id int, "
						"magic_level int, " 
						"property1 int, " 
						"judge1 int, " 
						"property2 int, " 
						"judge2 int, " 
						"property3 int, " 
						"judge3 int, " 
						"slot1 int, " 
						"slot2 int, " 
						"slot3 int, " 
						"slot4 int, " 
						"resatt int, " 
						"attlevel int, " 
						"resdef int, " 
						"deflevel int, " 
						"mark int )";

				sprintf(stmtcreate, stmtfmt, szTable);
			}
		}

		if ( mysql_query(&logdb, stmtcreate))
		{
			printf("execute %s error on logdb: %s\n", stmtcreate, mysql_error(&logdb));
			mysql_close(&logdb);
			mysql_close(&mysql);
			return -1;
		}


		if ( gItemType == 1)
		{
			CMaxString stmtins("");
			if (expand4corps( mysql, stmtins, szTable) != 0)
			{
				printf("execute %s error on logdb: %s\n", stmtcreate, mysql_error(&logdb));
				mysql_close(&logdb);
				mysql_close(&mysql);
				return -1;
			}

			if (stmtins.length() > 0 && mysql_query(&logdb, stmtins.c_str()))
			{
				printf("execute %s error on logdb: %s\n", stmtins.c_str(), mysql_error(&logdb));
				mysql_close(&logdb);
				mysql_close(&mysql);
				return -1;
			}

			mysql_close(&logdb);
			mysql_close(&mysql);
			return -1;
		}

		if (nRoleID != -1)
		{
			expand2db(mysql, logdb, nTableID, nRoleID, nCampID, fielddefs, szTable);

		}else if ( all_roles( mysql, lstRoles, szWhere) == 0)
		{
			printf("role number %d\n", lstRoles.size());

			role_info_t::const_iterator iter = lstRoles.begin();
			for( int i = 0; iter != lstRoles.end(); ++iter, ++i)
			{
				if ( i % 50 == 0) 
				{
					printf(".");
					fflush(stdout);
				}

				expand2db(mysql, logdb, iter->account_id % 16, iter->role_id, iter->camp_id, fielddefs, szTable);
			}
			printf("\n");
		}

		mysql_close(&logdb);
	} else if ( op == em_mail)
	{
		xml2mail(mysql, nCampID, nRoleID, guid, xmlfile, isbind);
	} else
	{
		xml2role(mysql, nTableID, nRoleID, fielddefs, xmlfile);
	}

	mysql_close(&mysql);

	return 0;
}

// ***************************************************************
// Function:       
// Description:  fetch role_id, account_id from UMS_ROLE to lstRoles
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           11/26/2009
// 
// ***************************************************************
int all_roles( MYSQL&mysql, role_info_t& lstRoles, const char* szWhere)
{
	const char* stmtfmt= "SELECT account_id, role_id, nationality_id from UMS_ROLE";	
	char stmtsql[4096] = {0};


	if ( szWhere != NULL)
	{
		sprintf(stmtsql, "%s WHERE %s", stmtfmt, szWhere);
	}else
	{
		sprintf(stmtsql, "%s", stmtfmt);
	}

	if (mysql_query(&mysql,stmtsql))
	{
		printf("mysql_query Error: %s", mysql_error(&mysql));
		return -1;
	}

	MYSQL_RES* presult = mysql_use_result(&mysql);
	if ( !presult )
	{
		printf("mysql_use_reuslt Error: %s", mysql_error(&mysql));
		return -1;
	}

	unsigned int  nfields = mysql_num_fields(presult);
	MYSQL_FIELD *fields = mysql_fetch_fields(presult);

	if ( !fields )
	{
		mysql_free_result(presult);
		printf("mysql_fetch_fields fetch fields struct error.\n");

		return -1;
	}

	// only fetch one row.
	MYSQL_ROW row ;
	while((row = mysql_fetch_row(presult)))
	{
		// fetch field length
		unsigned long *lengths = mysql_fetch_lengths(presult);
		//for (int i=0; i < nfields; ++i)
		lstRoles.push_back( role_info_t::value_type(atoi(row[0]), atoi(row[1]), atoi(row[2])));
	}
	
	if(!mysql_eof(presult))
	{
		mysql_free_result(presult);
		return -1;
	}

	mysql_free_result(presult);

	return 0;
}



// ***************************************************************
// Function:       
// Description:  expand pbitembox to table ums_role_item
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           11/26/2009
// 
// ***************************************************************
int expand2db( MYSQL&mysql, MYSQL& logdb, int nTableID, int nRoleID, int nCampID, CTokenList& fielddefs, const char* szTable)
{
	CMaxString stmtins("");
	if ( expand4pack(mysql, nTableID, nRoleID, fielddefs, stmtins, szTable) != 0 )
	{
		printf("expand4pack for role %d error\n", nRoleID);
	}

	if ( nCampID >=1 and nCampID <= 4 && expand4mail(mysql, nCampID, nRoleID, stmtins, szTable) != 0)
	{
		printf("expand4mail for role %d error\n", nRoleID);
	}

	if (stmtins.length() > 0 && mysql_query(&logdb, stmtins.c_str()))
	{
		printf("execute %s error on logdb: %s\n", stmtins.c_str(), mysql_error(&logdb));
		return -1;
	}

	// puts(stmtins.c_str());

	return 0;
}

int expand4pack( MYSQL&mysql, int nTableID, int nRoleID, CTokenList& fielddefs, CMaxString& stmtins, const char* szTable )
{
	int nBoxID = 0;	
	CMaxString stmtsql("");	

	if ( fielddefs.empty())
	{
		stmtsql.append("SELECT a.*, b.* FROM UMS_ROLE_DETAIL_%02d a, UMS_ROLE_DETAIL1_%02d b WHERE a.role_id = %d and a.role_id = b.role_id ", nTableID, nTableID, nRoleID);
	} else 
	{
		stmtsql.append("SELECT");
		CTokenList::const_iterator iter = fielddefs.begin();

		stmtsql.append(" %s", (iter++)->c_str());
		for(; iter != fielddefs.end(); ++iter)
		{
			stmtsql.append(", %s", iter->c_str() );
		}
		stmtsql.append(" FROM UMS_ROLE_DETAIL_%02d a, UMS_ROLE_DETAIL1_%02d b WHERE a.role_id =  %d and a.role_id = b.role_id", nTableID, nTableID, nRoleID);
	}

	if (mysql_query(&mysql,stmtsql.c_str()))
	{
		printf("mysql_query Error: %s\n", mysql_error(&mysql));
		return -1;
	}

	MYSQL_RES* presult = mysql_use_result(&mysql);
	if ( !presult )
	{
		printf("mysql_use_reuslt Error: %s\n", mysql_error(&mysql));
		return -1;
	}

	unsigned int  nfields = mysql_num_fields(presult);
	MYSQL_FIELD *fields = mysql_fetch_fields(presult);

	if ( nfields != fielddefs.size() && !fielddefs.empty()) 
	{
		mysql_free_result(presult);
		printf("mysql_num_fields fetch number(%d) does not match input(%d)\n", nfields, fielddefs.size());

		return -1;
	}

	if ( !fields )
	{
		mysql_free_result(presult);
		printf("mysql_fetch_fields fetch fields struct error.\n");

		return -1;
	}

	// only fetch one row.
	MYSQL_ROW row ;
	while((row = mysql_fetch_row(presult)))
	{
		// fetch field length
		unsigned long *lengths = mysql_fetch_lengths(presult);

		CTokenList::const_iterator iter =  fielddefs.begin();					
		for(int i = 0; iter != fielddefs.end(); ++iter, ++i)
		{
			const char* szFieldName = iter->c_str();
			if (strcmp(szFieldName, "role_storage") == 0)
			{
				nBoxID = 1;			
			} else if (strcmp(szFieldName, "role_package") == 0)
			{
				nBoxID = 2;
			} else if (strcmp(szFieldName, "role_equipment") == 0)
			{
				nBoxID = 0;
			}

			PBItemBox* pproto = new PBItemBox;
			if (!pproto || !pproto->ParseFromArray( row[i], lengths[i]))
			{
				printf("can't parse from array for role_id(%d) %s\n", nRoleID, szFieldName);
				continue;
			}
	
			for (int j = 0; j < pproto->itemobjects_size(); ++j)					
			{
				if ( gItemType == 0)
				{
					if (stmtins.length () == 0)
					{
						stmtins.append ("INSERT INTO %s VALUES ( %d, %d, %d, %d )", 
								szTable, nRoleID, nBoxID, pproto->itemobjects (j).itemid (),
								pproto->itemobjects (j).number ());
					}else
					{
						stmtins.append (",(%d, %d, %d, %d) ", nRoleID,
								nBoxID, pproto->itemobjects (j).itemid (),
								pproto->itemobjects (j).number ());
					}
				}else if (pproto->itemobjects(j).has_itemcontent())
				{
					const PBItemContent& pbcon = pproto->itemobjects(j).itemcontent(); 
					if ( pbcon.has_itemequip())
					{
						const PBItemObjEquip& pbEquip = pbcon.itemequip();

						if (stmtins.length () == 0)
						{
							stmtins.append ("INSERT INTO %s VALUES ( %d, %d, %d, ", szTable,
									nRoleID, nBoxID, pproto->itemobjects (j).itemid ());
						}else
						{
							stmtins.append (",(%d, %d, %d, ", nRoleID,
									nBoxID, pproto->itemobjects (j).itemid ());
						}

						// level int, bind int, runne int
						stmtins.append("%d, %d, %d, ", pbEquip.level(),  pproto->itemobjects(j).bindstatus(), pbEquip.slotspell());

						// magic_id int, magic_level int, 
						stmtins.append("%d, %d, ", pbEquip.magicstoneid(), pbEquip.magicstonenum());

						// property and judge
						stmtins.append("%d, %d, %d, %d, %d, %d, ", 
								pbEquip.property(0), pbEquip.judge(0), pbEquip.property(1), pbEquip.judge(1), pbEquip.property(2), pbEquip.judge(2));

						// slot
						stmtins.append("%d, %d, %d, %d, ",  pbEquip.slotvalue(0), pbEquip.slotvalue(1), pbEquip.slotvalue(1), pbEquip.slotvalue(2));

						// elem attack and resist
						stmtins.append("%d, %d, %d, %d, ",  pbEquip.resatt(), pbEquip.attlevel(), pbEquip.resdef(), pbEquip.deflevel());
					
						// mark
						stmtins.append("%d) ",  pbEquip.extratype());
					}
				}
			}
	
			delete pproto;
			pproto = NULL;
		}
	}

	if(!mysql_eof(presult))
	{
		mysql_free_result(presult);
		return -1;
	}

	mysql_free_result(presult);
	return 0;
}

int expand4mail( MYSQL&mysql, int nCampID, int nRoleID, CMaxString& stmtins, const char* szTable )
{
	CMaxString stmtmail("");
	stmtmail.append("SELECT item_attachment from UMS_ROLE_MAIL_%02d where recver_role_id = %d", nCampID, nRoleID);

	const int nBoxID = 3;
	if (mysql_query(&mysql, stmtmail.c_str()))
	{
		printf("mysql_query Error: %s\n", mysql_error(&mysql));
		return -1;
	}

	MYSQL_RES* presult = mysql_use_result(&mysql);
	if ( !presult )
	{
		printf("mysql_use_reuslt Error: %s\n", mysql_error(&mysql));
		return -1;
	}

	unsigned int  nfields = mysql_num_fields(presult);
	MYSQL_FIELD *fields = mysql_fetch_fields(presult);

	if ( nfields != 1 || !fields )
	{
		mysql_free_result(presult);
		printf("mysql_fetch_fields fetch fields struct error.\n");

		return -1;
	}

	// only fetch one row.
	MYSQL_ROW row ;
	while((row = mysql_fetch_row(presult)))
	{
		// fetch field length
		unsigned long *lengths = mysql_fetch_lengths(presult);

		MailItemBox proto; 
		if (!proto.ParseFromArray(row[0], lengths[0]))
		{
			printf("can't parse from array for role_id(%d) mail\n", nRoleID);
			continue;
		}
	
		for (int j = 0; j < proto.mailitem_size(); ++j)
		{
			if ( gItemType == 0)
			{
				if (stmtins.length () == 0)
				{
					stmtins.append ("INSERT INTO %s VALUES ( %d, %d, %d, %d )",
							szTable, nRoleID, nBoxID, proto.mailitem (j).itemid (),
							proto.mailitem (j).number ());
				}else
				{
					stmtins.append (",(%d, %d, %d, %d) ", nRoleID,
							nBoxID, proto.mailitem (j).itemid (),
							proto.mailitem (j).number ());
				}
			}else if (proto.mailitem(j).has_itemcontent())
			{
				const PBItemContent& pbcon = proto.mailitem(j).itemcontent(); 
				if ( pbcon.has_itemequip())
				{
					const PBItemObjEquip& pbEquip = pbcon.itemequip();

					if (stmtins.length () == 0)
					{
						stmtins.append ("INSERT INTO %s VALUES ( %d, %d, %d, ", szTable,
								nRoleID, nBoxID, proto.mailitem(j).itemid ());
					}else
					{
						stmtins.append (",(%d, %d, %d, ", nRoleID,
								nBoxID, proto.mailitem(j).itemid ());
					}

					// level int, bind int, runne int
					stmtins.append("%d, %d, %d, ", pbEquip.level(),  proto.mailitem(j).bindstatus(), pbEquip.slotspell());

					// magic_id int, magic_level int, 
					stmtins.append("%d, %d, ", pbEquip.magicstoneid(), pbEquip.magicstonenum());

					// property and judge
					stmtins.append("%d, %d, %d, %d, %d, %d, ", 
							pbEquip.property(0), pbEquip.judge(0), pbEquip.property(1), pbEquip.judge(1), pbEquip.property(2), pbEquip.judge(2));

					// slot
					stmtins.append("%d, %d, %d, %d, ",  pbEquip.slotvalue(0), pbEquip.slotvalue(1), pbEquip.slotvalue(1), pbEquip.slotvalue(2));

					// elem attack and resist
					stmtins.append("%d, %d, %d, %d, ",  pbEquip.resatt(), pbEquip.attlevel(), pbEquip.resdef(), pbEquip.deflevel());

					// mark
					stmtins.append("%d) ",  pbEquip.extratype());
				}
			}
		}
	}

	if(!mysql_eof(presult))
	{
		mysql_free_result(presult);
		return -1;
	}

	mysql_free_result(presult);
	return 0;
}


// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************
int xml2mail( MYSQL& mysql, int nTableID, int nRoleID, uint64_t guid, const char* xmlfile, int isbind)
{
	CTokenList fielddefs;
	fielddefs.push_back("role_equipment");
	fielddefs.push_back("role_package");
	fielddefs.push_back("role_storage");

	TiXmlDocument xmldoc;
	xmldoc.LoadFile( xmlfile );

	TiXmlElement* tpRoot= xmldoc.FirstChildElement();
	if (tpRoot == NULL)
	{
		printf("load role_detail from xmlfile(%s) failure.\n", xmlfile);
		return -1;
	}

	CTokenList::iterator iter = fielddefs.begin();
	for(; iter != fielddefs.end(); ++iter)
	{
		TiXmlElement* pFieldXml = tpRoot->FirstChildElement(iter->c_str());
		if (pFieldXml == NULL ) continue;

		if (xml2attach(mysql, nTableID, nRoleID, pFieldXml, guid, isbind) == 0)
		{
			printf("Congratulations, send item %lld to role(%d) successfully from xmlfile `%s.%s`\n", 
					guid, nRoleID,  xmlfile, iter->c_str());

			return 0;
		}
	}

	printf("Sorry, could not find item `%lld` from xmlfile `%s`, pls check it\n", guid, xmlfile);
	return -1;
}

int xml2attach(MYSQL& mysql, int nTableID, int nRoleID, TiXmlElement* xmlelem, uint64_t guid, int isbind) 
{
	PBItemBox* pproto = NULL;
	pproto = new PBItemBox;

	if ( !xmlelem ) return -1;

	if (pproto )
	{
		if (xml2protobuf(xmlelem, pproto, NULL) != 0)
		{
			printf("convert xml element [%s] to proto [%s] error.\n", 
					xmlelem->Value(), pproto->GetDescriptor()->name().c_str());

			delete pproto;
			return -1;
		}
	}

	MailItemBox mailbox;
	bool isguid = false;
	int nItemID = 0;

	for ( int i=0; i<pproto->itemobjects_size(); ++i )
	{
		if (pproto ->itemobjects(i).objguid() == guid)
		{
			PBItemObject * pObject = mailbox.add_mailitem();
			*pObject = pproto->itemobjects(i);
			if ( isbind) 
			{
				int nBindStatus = pObject->bindstatus(); 
				pObject->set_bindstatus(nBindStatus | 1);
			}

			nItemID = pObject->itemid();

			mailbox.add_itemindex(0);

			isguid = true;
			break;
		}
	}

	delete pproto;
	if (!isguid) return -1;


	// Compose SQL statement
	#define const_package_size 0x7FFF
	char stmtsql[const_package_size] = {0};	

	char szProto[const_package_size] = {0};
	char szEscProto[const_package_size] = {0};

	if (mailbox.SerializeToArray(szProto, sizeof(szProto)) != true)
	{
		printf("proto %s serialize failed for field %s, buffer size is %d.\n", 
					mailbox.GetDescriptor()->name().c_str(), xmlelem->Value(), const_package_size);

		return -1;
	}

	if (escape_string_for_mysql( szEscProto, sizeof(szEscProto)-1, szProto, mailbox.ByteSize()) == -1 )
	{
		printf("EscapeDataString from szProto to szEscProto for field(%s) overflow!\n", xmlelem->Value());
		return -1;
	}

	int now = time(0);

	sprintf(stmtsql, "INSERT INTO UMS_ROLE_MAIL_%02d (mail_id, mail_type, mail_status, sender_role_id, "
		"sender_role_name, recver_role_id, mail_title, mail_body, send_time, money_attachment, item_number, "
		"item_attachment, post_recver, expired_time, recver_role_name) "
		"VALUES( NULL, 0, 1, 0, 0, %d, '#t_tpl_%d_1', '', %d, 0, 1, '%s', 0, %d, 'SYSTEM') ", 
			nTableID, nRoleID, nItemID, now, szEscProto, now + 3600 * 24 * 30);

	// Execute SQL statement
	if (mysql_query(&mysql, stmtsql))
	{
		printf("MySQL ERROR(%s): [ %s ]\n", mysql_error(&mysql), stmtsql);
		return -1;
	}

	return 0;
}

int xml2role( MYSQL& mysql, int nTableID, int nRoleID, CTokenList& fielddefs, const char* xmlfile)
{
	
	TiXmlDocument xmldoc;
	xmldoc.LoadFile( xmlfile );

	TiXmlElement* tpRoot= xmldoc.FirstChildElement();
	if (tpRoot == NULL)
	{
		printf("load role_detail from xmlfile(%s) failure.", xmlfile);
		return -1;
	}

	const char* szRoleID = tpRoot->Attribute("role_id");
	if ( nRoleID != atoi(szRoleID))
	{
		printf("The role_id(%d) does not match role_id [%s] from xmlfile", nRoleID, szRoleID);
		return -1;
	}

	// 检查字段是否完整
	CTokenList::const_iterator iter =  fielddefs.begin();					
	for(; iter != fielddefs.end(); ++iter)
	{
		TiXmlElement* pFieldXml = tpRoot->FirstChildElement(iter->c_str());
		if (pFieldXml == NULL)
		{
			printf("The xmlfile %s has no field %s, pls check it.\n", xmlfile, iter->c_str());
			return -1;
		}
	}

	// update this field
	for( iter = fielddefs.begin(); iter != fielddefs.end(); ++iter)
	{
		TiXmlElement* pFieldXml = tpRoot->FirstChildElement(iter->c_str());
		if (xml2field(mysql, nTableID, nRoleID, pFieldXml) != 0)
		{
			printf("Sorry, update field `%s` from xmlfile `%s` failure, pls check it\n", iter->c_str(), xmlfile);
			continue;
		}else
		{
			printf("Congratulation, upload field `%s` from xmlfile `%s` successfully!\n", iter->c_str(), xmlfile);
		}
	}

	return 0;
}
// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************

int xml2field(MYSQL& mysql, int nTableID, int nRoleID, TiXmlElement* xmlelem) 
{
	const char* szFieldName = xmlelem->Value();

	Message* pproto = NULL;

	if (strcmp(szFieldName, "role_basic") == 0)
	{
		pproto = new PBPlayerBasicInfo;
	}else if (strcmp(szFieldName, "role_skill") == 0)
	{
		pproto = new PBSkillList;
	} else if (strcmp(szFieldName, "role_buff") == 0)
	{
		pproto = new PBBuffList;
	} else if (strcmp(szFieldName, "role_cdgroup") == 0)
	{
		pproto = new PBPlayerCDGroupInfo;
	} else if (strcmp(szFieldName, "role_package") == 0)
	{
		pproto = new PBItemBox;
	} else if (strcmp(szFieldName, "role_equipment") == 0)
	{
		pproto = new PBItemBox;
	} else if (strcmp(szFieldName, "role_storage") == 0)
	{
		pproto = new PBItemBox;
	} else if (strcmp(szFieldName, "role_pvp") == 0)
	{
		pproto = new PBPlayerPVPInfo;
	} else if (strcmp(szFieldName, "role_title") == 0)
	{
		pproto = new PBPlayerTitleInfo;
	} else if (strcmp(szFieldName, "role_activity") == 0)
	{
		pproto = new PBPlayerActivityInfo;
	} else if (strcmp(szFieldName, "role_talent") == 0)
	{
		pproto = new PBPlayerTalentInfo;
	} else if (strcmp(szFieldName, "role_mw") == 0)
	{
		pproto = new PBPlayerMWInfo;
	} else if (strcmp(szFieldName, "role_lifeskill") == 0)
	{
		pproto = new PBLifeSkillBox;
	} else if (strcmp(szFieldName, "role_statistic") == 0)
	{
		pproto = new PBPlayerStatisticInfo;
	} else if (strcmp(szFieldName, "role_task") == 0)
	{
		pproto = new PBTaskList;
	} else if (strcmp(szFieldName, "role_task_package") == 0)
	{
		pproto = new PBItemBox;
	} else if (strcmp(szFieldName, "role_monster_record") == 0)
	{
		pproto = new PBMonsterRecordInfo;
	} else if (strcmp(szFieldName, "role_tele_pos") == 0)
	{
		pproto = new PBTelePosInfo;
	} else if (strcmp(szFieldName, "role_wizard") == 0)
	{
		pproto = new PBWizard;
	} else if (strcmp(szFieldName, "role_friend") == 0)
	{
		pproto = new PBFriend;
	} else if (strcmp(szFieldName, "role_repetion") == 0)
	{
		pproto = new PBRepetion;
	} else if (strcmp(szFieldName, "role_client") == 0)
	{
		pproto = new PBPlayerClientInfo;
	} else if (strcmp(szFieldName, "role_charm") == 0)
	{
		pproto = new PBCharm;
	} else if (strcmp(szFieldName, "role_guide") == 0)
	{
		pproto = new PBGuideData;
	} else if (strcmp(szFieldName, "role_times") == 0)
	{
		pproto = new PBTimes;
	} else if (strcmp(szFieldName, "role_matrix") == 0)
	{
		pproto = new PBMatrixInfo;
	}else if (strcmp(szFieldName, "role_exptimeinfo"))
	{
		pproto = new PBExpTimeInfo;
	}else if (strcmp(szFieldName, "role_treasurebowl"))
	{
		pproto = new PBTreasureBowl;
	}else
	{
		printf("There is no message for the field %s, pls check it\n", szFieldName);	
		return -1;
	}

	if (pproto )
	{
		if (xml2protobuf(xmlelem, pproto, NULL) != 0)
		{
			printf("convert xml element [%s] to proto [%s] error.\n", 
					szFieldName, pproto->GetDescriptor()->name().c_str());

			delete pproto;
			return -1;
		}
	}

	/* for debug
	CMaxString xmlbuf;
	protobuf2xml(*pproto, xmlbuf, xml_format);
	puts(xmlbuf.c_str());
	*/

	// Compose SQL statement
	#define const_package_size 0x7FFF
	char stmtsql[const_package_size] = {0};	

	char szProto[const_package_size] = {0};
	char szEscProto[const_package_size] = {0};

	if (pproto->SerializeToArray(szProto, sizeof(szProto)) != true)
	{
		printf("proto %s serialize failed for field %s, buffer size is %d.\n", 
					pproto->GetDescriptor()->name().c_str(), szFieldName, const_package_size);

		delete pproto;
		return -1;
	}

	if (escape_string_for_mysql( szEscProto, sizeof(szEscProto)-1, szProto, pproto->ByteSize()) == -1 )
	{
		printf("EscapeDataString from szProto to szEscProto for field(%s) overflow!\n", szFieldName);

		delete pproto;
		return -1;
	}

	sprintf(stmtsql, "UPDATE %s_%02d SET %s = '%s' WHERE role_id = %d",
			mField2Table.find(szFieldName) == mField2Table.end() ? "UMS_ROLE_DETAIL1" : "UMS_ROLE_DETAIL",
			nTableID, szFieldName, szEscProto, nRoleID);

	// Execute SQL statement
	if (mysql_query(&mysql, stmtsql))
	{
		printf("update UMS_ROLE_DETAIL_%02d set field %s value Error for role_id (%d), mysql_query error %s\n", 
				nTableID, szFieldName, nRoleID, mysql_error(&mysql));

		delete pproto;
		return -1;
	}

	delete pproto;
	return 0;
}



// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************
int role2xml( MYSQL& mysql, int nTableID, int nRoleID, CTokenList& fielddefs, const char* xmlfile)
{
	lk::string<1024> stmtsql("");

	FILE* fp = fopen(xmlfile, "w+");		
	if ( !fp ) 
	{
		printf("Can not open the file %s, error:%s\n", xmlfile, strerror(errno));
		return -1;
	}

	if ( fielddefs.empty())
	{
		stmtsql.append("SELECT a.*, b.* FROM UMS_ROLE_DETAIL_%02d a, UMS_ROLE_DETAIL1_%02d b WHERE a.role_id = %d AND a.role_id = b.role_id", nTableID, nTableID, nRoleID);
	} else 
	{
		stmtsql.append("SELECT");
		CTokenList::const_iterator iter = fielddefs.begin();

		stmtsql.append(" %s", (iter++)->c_str());
		for(; iter != fielddefs.end(); ++iter)
		{
			stmtsql.append(", %s", iter->c_str() );
		}
		stmtsql.append(" FROM UMS_ROLE_DETAIL_%02d a, UMS_ROLE_DETAIL1_%02d b WHERE a.role_id =  %d AND a.role_id = b.role_id ", nTableID, nTableID, nRoleID);
	}

	if (mysql_query(&mysql,stmtsql.c_str()))
	{
		printf("mysql_query Error: %s", mysql_error(&mysql));
		fclose(fp);
		return -1;
	}

	MYSQL_RES* presult = mysql_use_result(&mysql);
	if ( !presult )
	{
		printf("mysql_use_reuslt Error: %s", mysql_error(&mysql));
		fclose(fp);
		return -1;
	}

	unsigned int  nfields = mysql_num_fields(presult);
	MYSQL_FIELD *fields = mysql_fetch_fields(presult);

	if ( nfields != fielddefs.size() && !fielddefs.empty()) 
	{
		mysql_free_result(presult);
		printf("mysql_num_fields fetch number(%d) does not match input(%d)\n", nfields, fielddefs.size());

		fclose(fp);
		return -1;
	}

	if ( !fields )
	{
		mysql_free_result(presult);
		printf("mysql_fetch_fields fetch fields struct error.\n");

		fclose(fp);
		return -1;
	}

	// only fetch one row.
	MYSQL_ROW row ;
	while((row = mysql_fetch_row(presult)))
	{
		// fetch field length
		unsigned long *lengths = mysql_fetch_lengths(presult);

		fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", fp);
		fprintf(fp, "<role_detail role_id=\"%d\">\n", nRoleID);

		for (int i=0; i < nfields; ++i)
		{
			//printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
			CMaxString xmlbuf("");
			if (field2xml(fields[i].name, xmlbuf, row[i], lengths[i]) == 0)
			{
				//output to the xmlfile;
				fputs(xmlbuf.c_str(), fp);
			}
		}

		fputs("</role_detail>\n", fp);
	}

	if(!mysql_eof(presult))
	{
		mysql_free_result(presult);
		fclose(fp);
		return -1;
	}

	mysql_free_result(presult);
	fclose(fp);

	return 0;
}

// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************
int which_table (MYSQL& mysql, int nRoleID, int* pnCampID)
{
	int nAccountID = 0;
	int nCampID = 0;

	lk::string<1024> stmtsql("SELECT account_id, nationality_id FROM UMS_ROLE ");	
	stmtsql.append( "WHERE role_id = %d", nRoleID);

	if (mysql_query(&mysql,stmtsql.c_str()))
	{
		printf("mysql_query Error: %s", mysql_error(&mysql));
		return -1;
	}

	MYSQL_RES* presult = mysql_use_result(&mysql);
	if ( !presult )
	{
		printf("mysql_use_reuslt Error: %s", mysql_error(&mysql));
		return -1;
	}

	unsigned int  nfields = mysql_num_fields(presult);
	if ( nfields != 2) 
	{
		mysql_free_result(presult);
		return -1;
	}

	MYSQL_ROW row;
	while((row = mysql_fetch_row(presult)))
	{
		nAccountID = atoi(row[0]);
		nCampID = atoi(row[1]);
	}

	if(!mysql_eof(presult))
	{
		mysql_free_result(presult);
		return -1;
	}

	mysql_free_result(presult);
	*pnCampID = nCampID;

	return nAccountID%16;
}

// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************
int field2xml(const char* szFieldName, CMaxString& xmlbuf, char* pbcode, unsigned int pblen)
{
	Message* pproto = NULL;

	if (strcmp(szFieldName, "role_basic") == 0)
	{
		pproto = new PBPlayerBasicInfo;
	}else if (strcmp(szFieldName, "role_skill") == 0)
	{
		pproto = new PBSkillList;
	} else if (strcmp(szFieldName, "role_buff") == 0)
	{
		pproto = new PBBuffList;
	} else if (strcmp(szFieldName, "role_cdgroup") == 0)
	{
		pproto = new PBPlayerCDGroupInfo;
	} else if (strcmp(szFieldName, "role_package") == 0)
	{
		pproto = new PBItemBox;
	} else if (strcmp(szFieldName, "role_equipment") == 0)
	{
		pproto = new PBItemBox;
	} else if (strcmp(szFieldName, "role_storage") == 0)
	{
		pproto = new PBItemBox;
	} else if (strcmp(szFieldName, "role_pvp") == 0)
	{
		pproto = new PBPlayerPVPInfo;
	} else if (strcmp(szFieldName, "role_title") == 0)
	{
		pproto = new PBPlayerTitleInfo;
	} else if (strcmp(szFieldName, "role_activity") == 0)
	{
		pproto = new PBPlayerActivityInfo;
	} else if (strcmp(szFieldName, "role_talent") == 0)
	{
		pproto = new PBPlayerTalentInfo;
	} else if (strcmp(szFieldName, "role_mw") == 0)
	{
		pproto = new PBPlayerMWInfo;
	} else if (strcmp(szFieldName, "role_lifeskill") == 0)
	{
		pproto = new PBLifeSkillBox;
	} else if (strcmp(szFieldName, "role_statistic") == 0)
	{
		pproto = new PBPlayerStatisticInfo;
	} else if (strcmp(szFieldName, "role_task") == 0)
	{
		pproto = new PBTaskList;
	} else if (strcmp(szFieldName, "role_task_package") == 0)
	{
		pproto = new PBItemBox;
	} else if (strcmp(szFieldName, "role_monster_record") == 0)
	{
		pproto = new PBMonsterRecordInfo;
	} else if (strcmp(szFieldName, "role_tele_pos") == 0)
	{
		pproto = new PBTelePosInfo;
	} else if (strcmp(szFieldName, "role_wizard") == 0)
	{
		pproto = new PBWizard;
	} else if (strcmp(szFieldName, "role_friend") == 0)
	{
		pproto = new PBFriend;
	} else if (strcmp(szFieldName, "role_repetion") == 0)
	{
		pproto = new PBRepetion;
	} else if (strcmp(szFieldName, "role_client") == 0)
	{
		pproto = new PBPlayerClientInfo;
	} else if (strcmp(szFieldName, "role_charm") == 0)
	{
		pproto = new PBCharm;
	} else if (strcmp(szFieldName, "role_guide") == 0)
	{
		pproto = new PBGuideData;
	} else if (strcmp(szFieldName, "role_times") == 0)
	{
		pproto = new PBTimes;
	} else if (strcmp(szFieldName, "role_matrix") == 0)
	{
		pproto = new PBMatrixInfo;
	} else if (strcmp(szFieldName, "role_exptimeinfo"))
	{
		pproto = new PBExpTimeInfo;
	}else if (strcmp(szFieldName, "role_treasurebowl"))
	{
		pproto = new PBTreasureBowl;
	}else
	{
		printf(" There is no message for the field %s, pls check it\n", szFieldName);	
		return -1;
	}

	if (pproto && pproto->ParseFromArray( pbcode, pblen))
	{
//		xmlbuf.append("<%s proto=\"%s\">\n", szFieldName, pproto->GetDescriptor()->name().c_str());
		protobuf2xml(*pproto, xmlbuf, szFieldName, pproto->GetDescriptor()->name().c_str(), xml_format);
//		xmlbuf.append("</%s>\n", szFieldName);
	}else
	{
		printf("%s Message.ParseFromArray error, len(%d)!", szFieldName, pblen);

		delete pproto;
		return -1;
	}

	delete pproto;
	return 0;
}







// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************
void fill_fielddefs(const char* szNames, CTokenList& fielddefs)
{

	char buff[256]= {0}; 
	memcpy(buff, szNames, sizeof(buff)-1);

	char* pcurr = buff;
	char* pprev = NULL;

	for(; *pcurr != 0; pcurr++)
	{
		switch(*pcurr)
		{
			case ' ':
			case '\t':
			case ',':
			case ';':
			{
				*pcurr = 0;
				if ( pprev )
				{
					fielddefs.push_back(pprev);
				}

				pprev = NULL;
				break;
			}

			default:
			{
				if ( pprev == NULL ) pprev = pcurr;
				break;
			}
		}
	}

	if ( pprev )
	{
		fielddefs.push_back(pprev);
	}
}

// ***************************************************************
// Function:       
// Description:    
// Input:          
// OutPut:         
// Return:         
// Others:         
// Date:           08/26/2009
// 
// ***************************************************************

bool connect_database(MYSQL& mysql, const char* connstr)
{
	TiXmlDocument tDoc;

	const char* szCfgFile = "../config/dbserver.xml";
	if( !tDoc.LoadFile( szCfgFile )) 
	{
		printf("load xml %s failed\n", szCfgFile );
		return false;
	}

	TiXmlElement* tpElement = NULL;
	TiXmlElement* tpChildElm = NULL;

	TiXmlElement* tpRoot = tDoc.FirstChildElement();
	tpElement = tpRoot->FirstChildElement("MYSQL");
	if( tpElement == NULL ) return false;

	const char* szAscIICode = tpElement->Attribute( connstr );
	if( szAscIICode == NULL ) return false;

	CTokenList tokens;
	fill_fielddefs(szAscIICode, tokens);

	//Tokens tokens = StrSplit(szAscIICode, ";");
	CTokenList::iterator iter;

	lk::CString128 host, port_or_socket, user, password, database;
	iter = tokens.begin();

	if(iter != tokens.end()) host = *iter++;
	if(iter != tokens.end()) port_or_socket = *iter++;
	if(iter != tokens.end()) user = *iter++;
	if(iter != tokens.end()) password = *iter++;
	if(iter != tokens.end()) database = *iter++;

	char szUser[32+1] = {0};
	int nUserLength = sizeof(szUser);

	char szPasswd[255+1] = {0};
	int nPasswdLength = sizeof(szPasswd);

	if ( DecryptAscIICode(user.c_str(), user.length(), szUser, &nUserLength) != true)
	{
		printf("ERROR: Can NOT decrypt user(%s) section.", user.c_str());
		return false;
	}

	if ( DecryptAscIICode(password.c_str(), password.length(), szPasswd, &nPasswdLength) != true)
	{
		printf("ERROR: Can NOT decrypt passwd(%s) section.", password.c_str());
		return false;
	}

	if(!mysql_real_connect(&mysql, host.c_str(), szUser,
				szPasswd, database.c_str(), atoi(port_or_socket.c_str()), 0, 0))
	{
		printf("Failed to connect to database: Error: %s\n", mysql_error(&mysql));
		return false;
	}

	return true;
}
//
//bool DecryptAscIICode(const char* szInput, int nInput, char* szOutput, int* pnOutput)
//{
//	static int tKey[ 4 ] = {4356,78534,46677,3545};
//	static unsigned char* tpKey = (unsigned char*)&tKey[0];
//
//	int nBinaryLen = 0;
//	unsigned char sBinaryCode[255+1] = {0};
//
//	size_t nAscIILen = strlen(szInput);
//
//	#define CHARTOHEX( c ) (isdigit(( c )) ? (( c ) - '0') : (tolower(( c )) - 'a' + 10))
//	for (size_t i= 0; i+2 <= nAscIILen; i=i+2, ++nBinaryLen)
//	{
//		if( isxdigit(szInput[i]) && isxdigit(szInput[i+1]) )
//		{
//			sBinaryCode[nBinaryLen] = CHARTOHEX(szInput[i]) * 16 + CHARTOHEX( szInput[i+1] );
//		}else
//		{
//			sBinaryCode[nBinaryLen] = 0;
//		}
//	}
//	#undef  CHARTOHEX
//
//	int nResult = oi_symmetry_decrypt( sBinaryCode, nBinaryLen, tpKey, (unsigned char*) szOutput, pnOutput);
//	if ( nResult == FALSE )
//	{
//		return  false;
//	}
//
//	szOutput[*pnOutput] = 0;
//	return true;
//}


int stream2xml(int nRoleID, CTokenList& fielddefs, const char* xmlfile)
{
	// find the correct stream		
	static char ins_stream[2048000];
	const char* key_table = "UMS_ROLE_DETAIL";
	const char* fmt_role = "(%d,";
	const char* key_end = ")";

	char this_stream[256000];

	char key_role[64] = {0};	
	sprintf(key_role, fmt_role, nRoleID);


	ins_stream[0] = 0;
	this_stream[0] = 0;

	bool isNewVersion = false;
	int  key_len = strlen(key_table);

	while( fgets(ins_stream, sizeof(ins_stream), stdin) != NULL)
	{
		char * pstart = NULL;
		char * pkey = NULL;
		if( (pkey = strstr(ins_stream, key_table)) != NULL
		&&  (pstart = strstr(ins_stream, key_role)) != NULL)
		{
			char* pend = NULL;
			if ( (pend = strstr ( pstart, key_end)) != NULL)
			{
				// get the insert statment.
				if (( pend - pstart ) >  sizeof ( this_stream))
				{
					printf("The Insert Stream Size(%d) is bigger than (%d)\n", pend-pstart, sizeof(this_stream));
					return -1;
				}

				if ( this_stream[0] == 0)
				{
					memcpy( this_stream, pstart + 1, pend - pstart - 1);
					this_stream[pend - pstart - 1] = 0;

					printf("first: %s\n", this_stream);
				}else
				{
					isNewVersion = true;
					if ( pkey[key_len] == '_')
					{
						int n = strlen(this_stream);
						
						memmove( this_stream + (pend - pstart), this_stream,  n + 1);
						memcpy(this_stream, pstart + 1, pend - pstart - 1);
						this_stream[pend - pstart - 1 ] = ',';
					}else
					{
						strncat( this_stream, pstart+1, pend - pstart -1 );
					}

					printf("second: %s\n", this_stream);
				}
			}
		}
	}


	// 
	if ( this_stream[0] == 0)
	{
		printf("Can Not find the specail data for role (%d)\n", nRoleID);	
		return -1;
	}

	std::vector<char*> fieldvalue;
	for( char *p = this_stream; p && *p != '\0'; ++p)
	{
		if ( *p == ',') 
		{
			*p = 0;

			if ( *(p+1) != '\0')
			{
				fieldvalue.push_back(p+1);
			}
		}
	}

	// fields def
	const char* fieldname1[] = {
		"role_hp",
		"role_mp",
		"role_ap",
		"role_basic",
		"role_skill",
		"role_buff",
		"role_cdgroup",
		"role_package",
		"role_equipment",
		"role_storage",
		"role_pvp",
		"role_title",
		"role_activity",
		"role_talent",
		"role_mw",
		"role_lifeskill",
		"role_statistic",
		"role_task",
		"role_task_package",
		"role_monster_record",
		"role_tele_pos",
		"role_wizard",
		"role_friend",
		"role_repetion",
		"role_client",
		"save_bitflag",
		"check_sum",
		"role_charm",
		"role_guide",
		"role_times",
		"role_matrix",
		NULL,
	};

	const char* fieldname2[] = {
		"role_hp",
		"role_mp",
		"role_ap",
		"role_basic",
		"role_skill",
		"role_buff",
		"role_cdgroup",
		"role_package",
		"role_equipment",
		"role_storage",
		"role_title",
		"role_activity",
		"role_task_package",
		"role_repetion",
		"role_pvp",
		"save_bitflag",
		"check_sum",
		"role_id",
		"role_task",
		"role_talent",
		"role_monster_record",
		"role_friend",
		"role_tele_pos",
		"role_wizard",
		"role_lifeskill",
		"role_client",
		"role_charm",
		"role_guide",
		"role_times",
		"role_matrix",
		"role_exptimeinfo",
		"role_mw",
		"role_statistic",
		"role_treasurebowl",
		NULL,
	};

	const char**fieldname = isNewVersion ? fieldname2 : fieldname1;

	// debug
	std::vector<char*>::iterator iter4v = fieldvalue.begin();
	for (int i = 0; iter4v != fieldvalue.end(); ++iter4v, ++i)
	{
		printf("fieldvalue[`%s`]: %s\n", fieldname[i], *(iter4v));
	}


	// create xml file
	FILE* fp = fopen(xmlfile, "w+");		
	if ( !fp ) 
	{
		printf("Can not open the file %s, error:%s\n", xmlfile, strerror(errno));
		return -1;
	}

	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", fp);
	fprintf(fp, "<role_detail role_id=\"%d\">\n", nRoleID);

	// fill field value
	CTokenList::const_iterator iter4d = fielddefs.begin();
	for ( ;iter4d != fielddefs.end(); ++iter4d)
	{
		int nIndex = -1;
		for ( const char**p = fieldname; *p != NULL; ++ p)
		{
			if ( strcasecmp(iter4d->c_str(), *p ) == 0)
			{
				 nIndex = (p - fieldname);
				 break;
			}
		}


		if (nIndex < 0 || nIndex >= fieldvalue.size())
		{
			printf("Could not find the fieldname (%s) in fielddef list\n", iter4d->c_str());
			continue;
		}

		if ( strncasecmp(fieldvalue[nIndex], "0x", 2) == 0)
		{
			//取得pb字节流与长度
			int nAscIILen = strlen(fieldvalue[nIndex]);
			char* szInput = fieldvalue[nIndex];
			char sBinaryCode[64000];
			int nBinaryLen = 0;

			memset(sBinaryCode, 0, sizeof(sBinaryCode));

			#define CHARTOHEX( c ) (isdigit(( c )) ? (( c ) - '0') : (tolower(( c )) - 'a' + 10))
			for (size_t i= 2; i+2 <= nAscIILen; i=i+2, ++nBinaryLen)
			{
				if( isxdigit(szInput[i]) && isxdigit(szInput[i+1]) )
				{
					sBinaryCode[nBinaryLen] = CHARTOHEX(szInput[i]) * 16 + CHARTOHEX( szInput[i+1] );
				}else
				{
					sBinaryCode[nBinaryLen] = 0;
				}
			}
			#undef  CHARTOHEX

			CMaxString xmlbuf("");
			if (field2xml(iter4d->c_str(), xmlbuf, sBinaryCode, nBinaryLen) == 0)
			{
				fputs(xmlbuf.c_str(), fp);
			}
		}else
		{
			printf("The Field (%s) is not a binary field\n", iter4d->c_str());
		}
	}

	fputs("</role_detail>\n", fp);

	return 0;
}

int expand4corps(MYSQL & mysql, CMaxString& stmtins, const char* szTable)
{
	lk::string<256> stmtsql = "select war_battleinfo, corps_id from UMS_CORPS";

	if (mysql_query(&mysql,stmtsql.c_str()))
	{
		printf("mysql_query Error: %s\n", mysql_error(&mysql));
		return -1;
	}

	MYSQL_RES* presult = mysql_use_result(&mysql);
	if ( !presult )
	{
		printf("mysql_use_reuslt Error: %s\n", mysql_error(&mysql));
		return -1;
	}

	unsigned int  nfields = mysql_num_fields(presult);
	MYSQL_FIELD *fields = mysql_fetch_fields(presult);

	if ( nfields != 2 || !fields )
	{
		mysql_free_result(presult);
		printf("mysql_fetch_fields fetch fields struct error.\n");

		return -1;
	}

	// only fetch one row.
	MYSQL_ROW row ;
	while((row = mysql_fetch_row(presult)))
	{
		// fetch field length
		unsigned long *lengths = mysql_fetch_lengths(presult);

		PBCorpsBattleList proto;
		if (!proto.ParseFromArray(row[0], lengths[0]))
		{
			printf("can't parse from array for corps_id(%d) mail\n", row[1]);
			continue;
		}
	
		for (int j = 0; j < proto.battlelist_size(); ++j)
		{
			const PBCorpsBattle& battle = proto.battlelist(j);
			if (stmtins.length () == 0)
			{
				stmtins.append ("INSERT INTO %s VALUES (NULL, %d, %d, %d, %d, %d, %d, %d, %d, %d )",
						szTable, battle.npcid(), battle.npcstatus(), battle.warstatus(), battle.type(), 
						battle.bidmoney(), battle.descorpsid(), battle.mapid(), battle.time(), battle.corpsid());
			}else
			{
				stmtins.append (", ( NULL, %d, %d, %d, %d, %d, %d, %d, %d, %d)", 
						battle.npcid(), battle.npcstatus(), battle.warstatus(), battle.type(), 
						battle.bidmoney(), battle.descorpsid(), battle.mapid(), battle.time(), battle.corpsid()); 
			}
		}
	}

	if(!mysql_eof(presult))
	{
		mysql_free_result(presult);
		return -1;
	}

	mysql_free_result(presult);
	return 0;
}


