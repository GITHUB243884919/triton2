#include "AGIPUserCashExchangeEx1.h"

AGIPUserCashExchangeEx1::AGIPUserCashExchangeEx1(void)
 : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CASH_EXCHANGE_EX1, sizeof(SAGIPUserCashExchangeEx1))
{
}

AGIPUserCashExchangeEx1::~AGIPUserCashExchangeEx1(void)
{
}

uint64_t AGIPUserCashExchangeEx1::getDetailID(int32_t nFlag) const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_detail_id_h))), nFlag);
}

void  AGIPUserCashExchangeEx1::setDetailID(uint64_t ullDetailID, int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_detail_id_h))) = htonll(ullDetailID, nFlag);
}

uint32_t AGIPUserCashExchangeEx1::getSrcUserID(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_src_user_id );
}

void  AGIPUserCashExchangeEx1::setSrcUserID(uint32_t unUserID)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_src_user_id = htonl(unUserID);
}

uint32_t AGIPUserCashExchangeEx1::getSrcRoleID(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_src_role_id );
}

void  AGIPUserCashExchangeEx1::setSrcRoleID(uint32_t unRoleID)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_src_role_id = htonl(unRoleID);
}

uint32_t AGIPUserCashExchangeEx1::getDstUserID(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_dst_user_id );
}

void  AGIPUserCashExchangeEx1::setDstUserID(uint32_t unUserID)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_dst_user_id = htonl(unUserID);
}

uint32_t AGIPUserCashExchangeEx1::getDstRoleID(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_dst_role_id );
}

void  AGIPUserCashExchangeEx1::setDstRoleID(uint32_t unRoleID)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_dst_role_id = htonl(unRoleID);
}

int16_t  AGIPUserCashExchangeEx1::getSubjectID(void) const
{
    return ntohs( ((PSAGIPUserCashExchangeEx1)m_pucPDU)->n_subject_id );
}

void  AGIPUserCashExchangeEx1::setSubjectID(int16_t nsSubjectID)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->n_subject_id = htons(nsSubjectID);
}

int32_t  AGIPUserCashExchangeEx1::getAmount(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx1)m_pucPDU)->n_amount );
}

void  AGIPUserCashExchangeEx1::setAmount(int32_t nAmount)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->n_amount = htonl(nAmount);
}

uint32_t AGIPUserCashExchangeEx1::getExchangeTime(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_exchange_time);
}

void  AGIPUserCashExchangeEx1::setExchangeTime(uint32_t unExchangeTime)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_exchange_time= htonl(unExchangeTime);
}

int16_t  AGIPUserCashExchangeEx1::getFlag(void) const
{
    return ntohs( ((PSAGIPUserCashExchangeEx1)m_pucPDU)->n_flag);
}

void  AGIPUserCashExchangeEx1::setFlag(int16_t nsFlag)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->n_flag = htons(nsFlag);
}

int32_t AGIPUserCashExchangeEx1::getType(void) const
{
    return ntohl(((PSAGIPUserCashExchangeEx1)m_pucPDU)->n_type);
}

void AGIPUserCashExchangeEx1::setType(int32_t nType)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->n_type = htonl(nType);
}

uint32_t AGIPUserCashExchangeEx1::getSrcGameID(void) const
{
    return ntohl(((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_src_game_id);
}

void AGIPUserCashExchangeEx1::setSrcGameID(uint32_t unGameID)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_src_game_id = htonl(unGameID);
}

uint32_t AGIPUserCashExchangeEx1::getDstGameID(void) const
{
    return ntohl(((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_dst_game_id);
}

void AGIPUserCashExchangeEx1::setDstGameID(uint32_t unGameID)
{
    ((PSAGIPUserCashExchangeEx1)m_pucPDU)->un_dst_game_id = htonl(unGameID);
}

int   AGIPUserCashExchangeEx1::showInfo(void)
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserCashExchangeEx1\n");

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
	printf("src_game_id     %u\n",      getSrcGameID());
	printf("dst_game_id     %u\n",      getDstGameID());

    printf("--------------------------------------------------------AGIPUserCashExchangeEx1\n");
    return S_SUCCESS;
}

//AGIPUserCashExchangeEx1Res
AGIPUserCashExchangeEx1Res::AGIPUserCashExchangeEx1Res(void)
 : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CASH_EXCHANGE_EX1_RES, sizeof(SAGIPUserCashExchangeEx1Res))
{
}

AGIPUserCashExchangeEx1Res::~AGIPUserCashExchangeEx1Res(void)
{
}

int32_t  AGIPUserCashExchangeEx1Res::getResultCode(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx1Res)m_pucPDU)->n_result_code);
}

void  AGIPUserCashExchangeEx1Res::setResultCode(int32_t nResultCode)
{
    ((PSAGIPUserCashExchangeEx1Res)m_pucPDU)->n_result_code = htonl(nResultCode);
}

uint64_t AGIPUserCashExchangeEx1Res::getDetailID(int32_t nFlag) const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserCashExchangeEx1Res)m_pucPDU)->un_detail_id_h))), nFlag);
}

void   AGIPUserCashExchangeEx1Res::setDetailID(uint64_t ullDetailID,int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserCashExchangeEx1Res)m_pucPDU)->un_detail_id_h))) = htonll(ullDetailID, nFlag);
}

int16_t  AGIPUserCashExchangeEx1Res::getFlag(void) const
{
    return ntohs( ((PSAGIPUserCashExchangeEx1Res)m_pucPDU)->n_flag);
}

void   AGIPUserCashExchangeEx1Res::setFlag(int16_t nsFlag)
{
    ((PSAGIPUserCashExchangeEx1Res)m_pucPDU)->n_flag = htons(nsFlag);
}

int32_t  AGIPUserCashExchangeEx1Res::getTotalExchange(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeEx1Res)m_pucPDU)->n_total_exchange);
}

void  AGIPUserCashExchangeEx1Res::setTotalExchange(int32_t nTotalExchange)
{
    ((PSAGIPUserCashExchangeEx1Res)m_pucPDU)->n_total_exchange = htonl(nTotalExchange);
}

int AGIPUserCashExchangeEx1Res::showInfo(void)
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserCashExchangeEx1Res\n");

    printf("result_code     %d\n",      getResultCode());
    printf("detail_id       %llu\n",    getDetailID());
    printf("flag            %d\n",      getFlag());
    printf("total_exchange  %d\n",      getTotalExchange());
    printf("--------------------------------------------------------AGIPUserCashExchangeEx1Res\n");
    return S_SUCCESS;
}

