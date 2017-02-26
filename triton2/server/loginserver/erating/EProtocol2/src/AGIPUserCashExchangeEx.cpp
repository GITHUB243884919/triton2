#include "AGIPUserCashExchangeEx.h"

AGIPUserCashExchangeEx::AGIPUserCashExchangeEx(void)
 : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CASH_EXCHANGE_EX, sizeof(SAGIPUserCashExchangeEx))
{
}

AGIPUserCashExchangeEx::~AGIPUserCashExchangeEx(void)
{
}

uint64_t AGIPUserCashExchangeEx::getDetailID(int32_t nFlag) const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserCashExchangeEx)m_pucPDU)->un_detail_id_h))), nFlag);
}

void  AGIPUserCashExchangeEx::setDetailID(uint64_t ullDetailID, int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserCashExchangeEx)m_pucPDU)->un_detail_id_h))) = htonll(ullDetailID, nFlag);
}

uint32_t AGIPUserCashExchangeEx::getSrcUserID(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx)m_pucPDU)->un_src_user_id );
}

void  AGIPUserCashExchangeEx::setSrcUserID(uint32_t unUserID)
{
    ((PSAGIPUserCashExchangeEx)m_pucPDU)->un_src_user_id = htonl(unUserID);
}

uint32_t AGIPUserCashExchangeEx::getSrcRoleID(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx)m_pucPDU)->un_src_role_id );
}

void  AGIPUserCashExchangeEx::setSrcRoleID(uint32_t unRoleID)
{
    ((PSAGIPUserCashExchangeEx)m_pucPDU)->un_src_role_id = htonl(unRoleID);
}

uint32_t AGIPUserCashExchangeEx::getDstUserID(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx)m_pucPDU)->un_dst_user_id );
}

void  AGIPUserCashExchangeEx::setDstUserID(uint32_t unUserID)
{
    ((PSAGIPUserCashExchangeEx)m_pucPDU)->un_dst_user_id = htonl(unUserID);
}

uint32_t AGIPUserCashExchangeEx::getDstRoleID(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx)m_pucPDU)->un_dst_role_id );
}

void  AGIPUserCashExchangeEx::setDstRoleID(uint32_t unRoleID)
{
    ((PSAGIPUserCashExchangeEx)m_pucPDU)->un_dst_role_id = htonl(unRoleID);
}

int16_t  AGIPUserCashExchangeEx::getSubjectID(void) const
{
    return ntohs( ((PSAGIPUserCashExchangeEx)m_pucPDU)->n_subject_id );
}

void  AGIPUserCashExchangeEx::setSubjectID(int16_t nsSubjectID)
{
    ((PSAGIPUserCashExchangeEx)m_pucPDU)->n_subject_id = htons(nsSubjectID);
}

int32_t  AGIPUserCashExchangeEx::getAmount(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx)m_pucPDU)->n_amount );
}

void  AGIPUserCashExchangeEx::setAmount(int32_t nAmount)
{
    ((PSAGIPUserCashExchangeEx)m_pucPDU)->n_amount = htonl(nAmount);
}

uint32_t AGIPUserCashExchangeEx::getExchangeTime(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx)m_pucPDU)->un_exchange_time);
}

void  AGIPUserCashExchangeEx::setExchangeTime(uint32_t unExchangeTime)
{
    ((PSAGIPUserCashExchangeEx)m_pucPDU)->un_exchange_time= htonl(unExchangeTime);
}

int16_t  AGIPUserCashExchangeEx::getFlag(void) const
{
    return ntohs( ((PSAGIPUserCashExchangeEx)m_pucPDU)->n_flag);
}

void  AGIPUserCashExchangeEx::setFlag(int16_t nsFlag)
{
    ((PSAGIPUserCashExchangeEx)m_pucPDU)->n_flag = htons(nsFlag);
}

int32_t AGIPUserCashExchangeEx::getType(void) const
{
    return ntohl(((PSAGIPUserCashExchangeEx)m_pucPDU)->n_type);
}

void AGIPUserCashExchangeEx::setType(int32_t nType)
{
    ((PSAGIPUserCashExchangeEx)m_pucPDU)->n_type = htonl(nType);
}

int   AGIPUserCashExchangeEx::showInfo(void)
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserCashExchangeEx\n");

    printf("detail_id       %llu\n",    getDetailID());
    printf("src_user_id     %u\n",      getSrcUserID());
    printf("src_role_id     %u\n",      getSrcRoleID());
    printf("dst_user_id     %u\n",      getDstUserID());
    printf("dst_role_id     %u\n",      getDstRoleID());
    printf("subject_id      %d\n",      getSubjectID());
    printf("amount          %d\n",      getAmount());
    printf("exchange_time   %u\n",      getExchangeTime());
    printf("flag            %d\n",      getFlag());
    printf("type            %d\n",      getType());

    printf("--------------------------------------------------------AGIPUserCashExchangeEx\n");
    return S_SUCCESS;
}

//AGIPUserCashExchangeExRes
AGIPUserCashExchangeExRes::AGIPUserCashExchangeExRes(void)
 : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CASH_EXCHANGE_EX_RES, sizeof(SAGIPUserCashExchangeExRes))
{
}

AGIPUserCashExchangeExRes::~AGIPUserCashExchangeExRes(void)
{
}

int32_t  AGIPUserCashExchangeExRes::getResultCode(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeExRes)m_pucPDU)->n_result_code);
}

void  AGIPUserCashExchangeExRes::setResultCode(int32_t nResultCode)
{
    ((PSAGIPUserCashExchangeExRes)m_pucPDU)->n_result_code = htonl(nResultCode);
}

uint64_t AGIPUserCashExchangeExRes::getDetailID(int32_t nFlag) const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserCashExchangeExRes)m_pucPDU)->un_detail_id_h))), nFlag);
}

void   AGIPUserCashExchangeExRes::setDetailID(uint64_t ullDetailID,int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserCashExchangeExRes)m_pucPDU)->un_detail_id_h))) = htonll(ullDetailID, nFlag);
}

int16_t  AGIPUserCashExchangeExRes::getFlag(void) const
{
    return ntohs( ((PSAGIPUserCashExchangeExRes)m_pucPDU)->n_flag);
}

void   AGIPUserCashExchangeExRes::setFlag(int16_t nsFlag)
{
    ((PSAGIPUserCashExchangeExRes)m_pucPDU)->n_flag = htons(nsFlag);
}

int32_t  AGIPUserCashExchangeExRes::getTotalExchange(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeExRes)m_pucPDU)->n_total_exchange);
}

void  AGIPUserCashExchangeExRes::setTotalExchange(int32_t nTotalExchange)
{
    ((PSAGIPUserCashExchangeExRes)m_pucPDU)->n_total_exchange = htonl(nTotalExchange);
}

int AGIPUserCashExchangeExRes::showInfo(void)
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserCashExchangeExRes\n");

    printf("result_code     %d\n",      getResultCode());
    printf("detail_id       %llu\n",    getDetailID());
    printf("flag            %d\n",      getFlag());
    printf("total_exchange  %d\n",      getTotalExchange());
    printf("--------------------------------------------------------AGIPUserCashExchangeExRes\n");
    return S_SUCCESS;
}

