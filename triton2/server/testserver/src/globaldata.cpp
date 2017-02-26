// ********************************************************************** //
// globaldata.cpp
// mysql操作
// 完成和mysql连接、断开、查询、插入、更新
//
// Author: xuliang
// ********************************************************************** //
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include "object.h"
#include "coretype.h"
#include "log.h"
#include "dbinterface.h"
#include "servertool.h"
#include "lk_string.h"
#include "globallogic.h"
#include "globaldata.h"

#define GS_SQL_MAXLENGTH        1024*32
#define GS_GATEBUFFER_MAXLENGTH 1024*8

template<> CGlobalPlayerData* CSingleton<CGlobalPlayerData>::spSingleton = NULL;

//============================================================
// <T>和数据库建立连接</T>
//
// @return 连接是否成功。
//============================================================
bool CGlobalPlayerData::Connect(){
	CGlobalServerConfiguration& configuration = CGlobalServer::GetSingleton().Configuration();
	char* host = configuration.dataHost;
    int port = configuration.dataPort;
	char* user = configuration.dataLoginUser;
	char* passwd = configuration.dataLoginPassword;
	char* db = configuration.dataDatabase;
   // 初始化环境
   MYSQL* pInit = mysql_init(NULL);
   if (NULL == pInit){
      return false;
   }
   // 链接数据库
   LOG_INFO("default", "[%s:%d][%s] Connect mysql (host=%s, port=%d, user=%s, password=%s, database=%s)",
         __LK_FILE__, __LINE__, __FUNCTION__,
         host, port, user, passwd, db);
	_pConnection = mysql_real_connect(pInit, host, user, passwd, db, port, NULL, 0);
	if(NULL == _pConnection){
		return false;
	}
	_isConnected = true;
	return true;
}

//============================================================
// <T>和数据库断开连接</T>
//
// @断开连接是否成功。
//============================================================
bool CGlobalPlayerData::Disconnect(){
	if(_isConnected){
		mysql_close(_pConnection);
		_isConnected = false;
		return true;
	}
	return true;
}

//============================================================
// <T>通过账号信息从数据库中取得数据</T>
//
// @param pPlayer 全局用户信息
// @param pPassport 通行账号
// @return取得数据是否成功
//============================================================
bool CGlobalPlayerData::FetchByPassport(CGlobalPlayer* pPlayer, char* pPassport){
	char sql[GS_SQL_MAXLENGTH];
	sprintf(sql,"SELECT ACCOUNT_ID,SERVER_INFOS FROM GS_ACCOUNT WHERE PASSPORT='%s'",pPassport);
	if(0 != mysql_query(_pConnection, sql)){
		LOG_ERROR("default", "Run SQL<%s> Error:%s\n",sql,mysql_error(_pConnection));
		return false;
	} 
   bool result = false;
	MYSQL_RES* pResult = mysql_store_result(_pConnection);
	if(NULL != pResult){
		MYSQL_ROW ppRow = mysql_fetch_row(pResult);
		if(ppRow){
			LOG_DEBUG("default","Run SQL: <%s> Return <ACCOUNT_ID:%s,SERVER_INFOS:%s>",sql,ppRow[0],ppRow[1]);
			pPlayer->accountId = atoi(ppRow[0]);
			char gates[1024];
			RBytes::FromBytes(ppRow[1],strlen(ppRow[1]),gates,1024);
			int len = strlen(gates);
			pPlayer->gateCount = len / sizeof(SGlobalPlayerGateInfo);
			memcpy(pPlayer->gates, gates,len);
			result = true;
      }
      mysql_free_result(pResult);
	}else{
		LOG_DEBUG("default","Run SQL: <%s> return NULL",sql);
	}
   return result;
}

//============================================================
// <T>通过账号信息从数据库中取得数据</T>
//
// @param pPlayer 全局用户信息
// @param pPassport 通行账号
// @return取得数据是否成功
//============================================================
bool CGlobalPlayerData::FetchByPassport2(CCertifyPlayer* pPlayer, char* pPassport){

	char sql[GS_SQL_MAXLENGTH];
	sprintf(sql,"SELECT ACCOUNT_ID,SERVER_INFOS FROM GS_ACCOUNT WHERE PASSPORT='%s'",pPassport);
	if(0 != mysql_query(_pConnection, sql)){
		LOG_ERROR("default", "Run SQL<%s> Error:%s\n",sql,mysql_error(_pConnection));
		return false;
	} 
	bool result = false;
	MYSQL_RES* pResult = mysql_store_result(_pConnection);
	if(NULL != pResult){
		MYSQL_ROW ppRow = mysql_fetch_row(pResult);
		if(ppRow){
			LOG_DEBUG("default","Run SQL: <%s> Return <ACCOUNT_ID:%s,SERVER_INFOS:%s>",sql,ppRow[0],ppRow[1]);
			pPlayer->_accountId = atoi(ppRow[0]);
			char gates[1024];
			RBytes::FromBytes(ppRow[1],strlen(ppRow[1]),gates,1024);
			int len = strlen(gates);
			pPlayer->_gateCount = len / sizeof(SGlobalPlayerGateInfo);
			memcpy(pPlayer->_gates, gates,len);
			result = true;
		}
		mysql_free_result(pResult);
	}else{
		LOG_DEBUG("default","Run SQL: <%s> return NULL",sql);
	}
	return result;
}

//============================================================
// <T>通过账号编号从数据库中取得数据</T>
//
// @param pPlayer 全局用户信息
// @param pAccountId 账号编号
// @return取得数据是否成功
//============================================================

bool CGlobalPlayerData::FetchByAccountId(CGlobalPlayer* pPlayer, uint32 accountId){
	char sql[GS_SQL_MAXLENGTH];
	sprintf(sql,"SELECT PASSPORT, SERVER_INFOS FROM GS_ACCOUNT WHERE ACCOUNT_ID='%d'",accountId);
	if(0 != mysql_query(_pConnection, sql)){
		LOG_ERROR("default", "Run SQL<%s> Error:%s\n",sql,mysql_error(_pConnection));
		return false;
	}
	bool result = false;
	MYSQL_RES* pResult = mysql_store_result(_pConnection);
	if(NULL != pResult){
		MYSQL_ROW ppRow = mysql_fetch_row(pResult);
		if(ppRow){
			LOG_DEBUG("default","Run SQL: <%s> Return <PASSPORT:%s,SERVER_INFOS:%s>",sql,ppRow[0],ppRow[1]);
			strcpy(pPlayer->passport, ppRow[0]);
			char gates[1024];
			RBytes::FromBytes(ppRow[1],strlen(ppRow[1]),gates,1024);
			int len = strlen(gates);
			pPlayer->gateCount = len / sizeof(SGlobalPlayerGateInfo);
			memcpy(pPlayer->gates, gates,len);
			result = true;
		}
		mysql_free_result(pResult);
	}else{
		LOG_DEBUG("default","Run SQL: <%s> return NULL",sql);
	}
	return result;

}

//============================================================
// <T>往数据库中加入信息</T>
//
// @param pPlayer 全局用户信息
// @return 加入信息是否成功
//============================================================
bool CGlobalPlayerData::Insert(CGlobalPlayer* pPlayer){
	char buffer[GS_GATEBUFFER_MAXLENGTH];
	int size = sizeof(SGlobalPlayerGateInfo) * pPlayer->gateCount;
	RBytes::ToChars((char*)pPlayer->gates, size, buffer, GS_GATEBUFFER_MAXLENGTH);
	char sql[GS_SQL_MAXLENGTH];
	sprintf(sql,"INSERT GS_ACCOUNT(ACCOUNT_ID, PASSPORT, SERVER_INFOS) VALUES('%d', '%s', '%s')",
		pPlayer->accountId,
		pPlayer->passport,
		buffer
		);
	if(0 != mysql_query(_pConnection, sql)){
		LOG_ERROR("default", "Run SQL<%s> Error:%s\n",sql,mysql_error(_pConnection));
		return false;
	}
	int affected = mysql_affected_rows(_pConnection);
	if(-1 == affected){
		LOG_ERROR("default", "Run SQL<%s> No Effect.\n",sql);
		return false;
	}
	LOG_DEBUG("default", "Run SQL<%s> Insert new line.\n",sql);
	return true;	
}

//============================================================
// <T>往数据库中加入信息</T>
//
// @param pPlayer 全局用户信息
// @return 加入信息是否成功
//============================================================
bool CGlobalPlayerData::Insert2(CCertifyPlayer* pPlayer){
	char buffer[GS_GATEBUFFER_MAXLENGTH];
	int size = sizeof(SGlobalPlayerGateInfo) * pPlayer->_gateCount;
	RBytes::ToChars((char*)pPlayer->_gates, size, buffer, GS_GATEBUFFER_MAXLENGTH);
	char sql[GS_SQL_MAXLENGTH];
	sprintf(sql,"INSERT GS_ACCOUNT(ACCOUNT_ID, PASSPORT, SERVER_INFOS) VALUES('%d', '%s', '%s')",
		pPlayer->_accountId,
		pPlayer->_passport,
		buffer
		);
	LOG_DEBUG("default", "Run SQL<%s>",sql);
	if(0 != mysql_query(_pConnection, sql)){
		LOG_ERROR("default", "Run SQL<%s> Error:%s\n",sql,mysql_error(_pConnection));
		return false;
	}
	int affected = mysql_affected_rows(_pConnection);
	if(-1 == affected){
		LOG_ERROR("default", "Run SQL<%s> No Effect.\n",sql);
		return false;
	}
	LOG_DEBUG("default", "Run SQL<%s> Insert new line.\n",sql);
	return true;	
}
//============================================================
// <T>更新用户信息</T>
//
// @param pPlayer 全局用户信息
// @return 更新信息时候成功
//============================================================
bool CGlobalPlayerData::Update(CGlobalPlayer* pPlayer){
	char buffer[GS_GATEBUFFER_MAXLENGTH];
	int size = sizeof(SGlobalPlayerGateInfo) * pPlayer->gateCount;
	RBytes::ToChars((char*)&pPlayer->gates[0], size, buffer, GS_GATEBUFFER_MAXLENGTH);
	char sql[GS_SQL_MAXLENGTH];
	sprintf(sql, "UPDATE GS_ACCOUNT SET SERVER_INFOS='%s' WHERE PASSPORT='%s'",
         buffer, pPlayer->passport);
	if(0 != mysql_query(_pConnection, sql)){
		LOG_ERROR("default", "Run SQL<%s> Error:%s\n",sql,mysql_error(_pConnection));
		return false;
	}
	int affected = mysql_affected_rows(_pConnection);
	if(affected == -1){
		LOG_ERROR("default", "Run SQL<%s> No Effect.\n",sql);
		return false;
	}
	LOG_DEBUG("default", "Run SQL<%s> Update a line.\n",sql);
	return true;
}
