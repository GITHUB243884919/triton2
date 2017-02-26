#ifndef _GLOBAL_DATA_H_
#define _GLOBAL_DATA_H_

#include <mysql.h>
#include "entity.h"

//============================================================
// CGlobalPlayer
//============================================================
class CGlobalPlayerData : public CSingleton<CGlobalPlayerData>{
protected:
	bool _isConnected;
   MYSQL* _pConnection;
public:
   bool Connect();
   bool Disconnect();
   bool FetchByPassport(CGlobalPlayer* pPlayer, char* pPassport);
	bool FetchByPassport2(CCertifyPlayer* pPlayer, char* pPassport);
   bool FetchByAccountId(CGlobalPlayer* pPlayer, uint32 accountId);
   bool Insert(CGlobalPlayer* pPlayer);
	bool Insert2(CCertifyPlayer* pPlayer);
   bool Update(CGlobalPlayer* pPlayer);
};

#endif // _GLOBAL_DATA_H_