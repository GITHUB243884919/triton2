#include "AGIPUserSerialQuery.h"

AGIPUserSerialQuery::AGIPUserSerialQuery(void)
 : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_SERIAL_QUERY, sizeof(SAGIPUserSerialQuery))
{
}

AGIPUserSerialQuery::~AGIPUserSerialQuery(void)
{
}

uint32_t AGIPUserSerialQuery::getUserID(void) const
{
    return ntohl( ((PSAGIPUserSerialQuery)m_pucPDU)->un_user_id );
}

void AGIPUserSerialQuery::setUserID(uint32_t unUserID)
{
    ((PSAGIPUserSerialQuery)m_pucPDU)->un_user_id = htonl(unUserID);
}

uint32_t AGIPUserSerialQuery::getActivityID(void) const
{
    return ntohl( ((PSAGIPUserSerialQuery)m_pucPDU)->un_activity_id );
}

void AGIPUserSerialQuery::setActivityID(uint32_t unActivityID)
{
    ((PSAGIPUserSerialQuery)m_pucPDU)->un_activity_id = htonl(unActivityID);
}

int AGIPUserSerialQuery::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserSerialQuery\n");

    printf("user_id         %u\n",      getUserID());
    printf("activity_id     %u\n",      getActivityID());

    printf("--------------------------------------------------------AGIPUserSerialQuery\n");

    return S_SUCCESS;
}


//AGIPUserSerialQueryRes
AGIPUserSerialQueryRes::AGIPUserSerialQueryRes(void)
 : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_SERIAL_QUERY_RES, 
               sizeof(SAGIPUserSerialQueryRes)
              )
{
}

AGIPUserSerialQueryRes::~AGIPUserSerialQueryRes(void)
{
}

int32_t AGIPUserSerialQueryRes::getResultCode(void) const
{
     return ntohl( ((PSAGIPUserSerialQueryRes)m_pucPDU)->n_result_code);
}

void AGIPUserSerialQueryRes::setResultCode(int32_t nResultCode)
{
    ((PSAGIPUserSerialQueryRes)m_pucPDU)->n_result_code = htonl(nResultCode);
}

uint32_t AGIPUserSerialQueryRes::getUseTime(void) const
{
    return ntohl( ((PSAGIPUserSerialQueryRes)m_pucPDU)->un_use_time );
}

void AGIPUserSerialQueryRes::setUseTime(uint32_t unUseTime)
{
    ((PSAGIPUserSerialQueryRes)m_pucPDU)->un_use_time = htonl(unUseTime);
}

int AGIPUserSerialQueryRes::showInfo()
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserSerialQueryRes\n");

    printf("result_code     %d\n",      getResultCode());
    printf("Use_Time        %s(%u)\n",  stringTime((time_t)getUseTime()), getUseTime());

    printf("--------------------------------------------------------AGIPUserSerialQueryRes\n");

    return S_SUCCESS;
}

