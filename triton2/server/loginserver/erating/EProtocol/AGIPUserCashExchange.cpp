#include "AGIPUserCashExchange.h"

AGIPUserCashExchange::AGIPUserCashExchange(void)
 : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CASH_EXCHANGE, sizeof(SAGIPUserCashExchange))
{
}

AGIPUserCashExchange::~AGIPUserCashExchange(void)
{
}

uint32_t AGIPUserCashExchange::getGatewayID(void) const
{
    return ntohl( ((PSAGIPUserCashExchange)m_pucPDU)->un_gateway_id );
}

void  AGIPUserCashExchange::setGatewayID(uint32_t nGatewayID)
{
    ((PSAGIPUserCashExchange)m_pucPDU)->un_gateway_id = htonl(nGatewayID);
}

uint64_t AGIPUserCashExchange::getDetailID(int32_t nFlag) const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserCashExchange)m_pucPDU)->un_detail_id_h))), nFlag);
}

void  AGIPUserCashExchange::setDetailID(uint64_t ullDetailID, int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserCashExchange)m_pucPDU)->un_detail_id_h))) = htonll(ullDetailID, nFlag);
}

uint32_t AGIPUserCashExchange::getSrcUserID(void) const
{
    return ntohl( ((PSAGIPUserCashExchange)m_pucPDU)->un_src_user_id );
}

void  AGIPUserCashExchange::setSrcUserID(uint32_t unUserID)
{
    ((PSAGIPUserCashExchange)m_pucPDU)->un_src_user_id = htonl(unUserID);
}

uint32_t AGIPUserCashExchange::getSrcRoleID(void) const
{
    return ntohl( ((PSAGIPUserCashExchange)m_pucPDU)->un_src_role_id );
}

void  AGIPUserCashExchange::setSrcRoleID(uint32_t unRoleID)
{
    ((PSAGIPUserCashExchange)m_pucPDU)->un_src_role_id = htonl(unRoleID);
}

uint32_t AGIPUserCashExchange::getDstUserID(void) const
{
    return ntohl( ((PSAGIPUserCashExchange)m_pucPDU)->un_dst_user_id );
}

void  AGIPUserCashExchange::setDstUserID(uint32_t unUserID)
{
    ((PSAGIPUserCashExchange)m_pucPDU)->un_dst_user_id = htonl(unUserID);
}

uint32_t AGIPUserCashExchange::getDstRoleID(void) const
{
    return ntohl( ((PSAGIPUserCashExchange)m_pucPDU)->un_dst_role_id );
}

void  AGIPUserCashExchange::setDstRoleID(uint32_t unRoleID)
{
    ((PSAGIPUserCashExchange)m_pucPDU)->un_dst_role_id = htonl(unRoleID);
}

int16_t  AGIPUserCashExchange::getSubjectID(void) const
{
    return ntohs( ((PSAGIPUserCashExchange)m_pucPDU)->n_subject_id );
}

void  AGIPUserCashExchange::setSubjectID(int16_t nsSubjectID)
{
    ((PSAGIPUserCashExchange)m_pucPDU)->n_subject_id = htons(nsSubjectID);
}

int32_t  AGIPUserCashExchange::getAmount(void) const
{
    return ntohl( ((PSAGIPUserCashExchange)m_pucPDU)->n_amount );
}

void  AGIPUserCashExchange::setAmount(int32_t nAmount)
{
    ((PSAGIPUserCashExchange)m_pucPDU)->n_amount = htonl(nAmount);
}

uint32_t AGIPUserCashExchange::getExchangeTime(void) const
{
    return ntohl( ((PSAGIPUserCashExchange)m_pucPDU)->un_exchange_time);
}

void  AGIPUserCashExchange::setExchangeTime(uint32_t unExchangeTime)
{
    ((PSAGIPUserCashExchange)m_pucPDU)->un_exchange_time= htonl(unExchangeTime);
}

int16_t  AGIPUserCashExchange::getFlag(void) const
{
    return ntohs( ((PSAGIPUserCashExchange)m_pucPDU)->n_flag);
}

void  AGIPUserCashExchange::setFlag(int16_t nsFlag)
{
    ((PSAGIPUserCashExchange)m_pucPDU)->n_flag = htons(nsFlag);
}

int   AGIPUserCashExchange::showInfo(void)
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserCashExchange\n");

    printf("gateway_id      %u\n",      getGatewayID());
    printf("detail_id       %llu\n",    getDetailID());
    printf("src_user_id     %u\n",      getSrcUserID());
    printf("src_role_id     %u\n",      getSrcRoleID());
    printf("dst_user_id     %u\n",      getDstUserID());
    printf("dst_role_id     %u\n",      getDstRoleID());
    printf("subject_id      %d\n",      getSubjectID());
    printf("amount          %d\n",      getAmount());
    printf("exchange_time   %u\n",      getExchangeTime());
    printf("flag            %d\n",      getFlag());

    printf("--------------------------------------------------------AGIPUserCashExchange\n");
    return S_SUCCESS;
}

//AGIPUserCashExchangeRes
AGIPUserCashExchangeRes::AGIPUserCashExchangeRes(void)
 : SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_CASH_EXCHANGE_RES, sizeof(SAGIPUserCashExchangeRes))
{
}

AGIPUserCashExchangeRes::~AGIPUserCashExchangeRes(void)
{
}

int32_t  AGIPUserCashExchangeRes::getResultCode(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeRes)m_pucPDU)->n_result_code);
}

void  AGIPUserCashExchangeRes::setResultCode(int32_t nResultCode)
{
    ((PSAGIPUserCashExchangeRes)m_pucPDU)->n_result_code = htonl(nResultCode);
}

uint64_t AGIPUserCashExchangeRes::getDetailID(int32_t nFlag) const
{
    return ntohll(*((uint64_t*)(&(((PSAGIPUserCashExchange)m_pucPDU)->un_detail_id_h))), nFlag);
}

void   AGIPUserCashExchangeRes::setDetailID(uint64_t ullDetailID,int32_t nFlag)
{
    *((uint64_t*)(&(((PSAGIPUserCashExchange)m_pucPDU)->un_detail_id_h))) = htonll(ullDetailID, nFlag);
}

int16_t  AGIPUserCashExchangeRes::getFlag(void) const
{
    return ntohs( ((PSAGIPUserCashExchangeRes)m_pucPDU)->n_flag);
}

void   AGIPUserCashExchangeRes::setFlag(int16_t nsFlag)
{
    ((PSAGIPUserCashExchangeRes)m_pucPDU)->n_flag = htons(nsFlag);
}

int32_t  AGIPUserCashExchangeRes::getTotalExchange(void) const
{
    return ntohl( ((PSAGIPUserCashExchangeRes)m_pucPDU)->n_total_exchange);
}

void  AGIPUserCashExchangeRes::setTotalExchange(int32_t nTotalExchange)
{
    ((PSAGIPUserCashExchangeRes)m_pucPDU)->n_total_exchange = htonl(nTotalExchange);
}

int AGIPUserCashExchangeRes::showInfo(void)
{
    SysProtocol::showInfo();
    printf("--------------------------------------------------------AGIPUserCashExchangeRes\n");

    printf("result_code     %u\n",      getResultCode());
    printf("detail_id       %llu\n",    getDetailID());
    printf("flag            %d\n",      getFlag());
    printf("total_exchange  %d\n",      getTotalExchange());
    printf("--------------------------------------------------------AGIPUserCashExchangeRes\n");
    return S_SUCCESS;
}

