#include "AGIPBindNotify.h"

AGIPBindNotify::AGIPBindNotify(void)
    : SysProtocol(AGIP_CURRENT_VERSION, CMD_BIND_NOTIFY, sizeof(SAGIPBindNotify))
{
}

AGIPBindNotify::~AGIPBindNotify(void)
{
}

void AGIPBindNotify::setResultCode(int32_t result_code)
{
    ((PSAGIPBindNotify)m_pucPDU)->result_Code = htonl(result_code);
}

int32_t AGIPBindNotify::getResultCode(void)
{
    return ntohl(((PSAGIPBindNotify)m_pucPDU)->result_Code);
}

void AGIPBindNotify::setBindIP(uint32_t bind_ip)
{
    ((PSAGIPBindNotify)m_pucPDU)->bind_IP = htonl(bind_ip);
}

uint32_t AGIPBindNotify::getBindIP(void)
{
    return ntohl(((PSAGIPBindNotify)m_pucPDU)->bind_IP);
}

void AGIPBindNotify::setBindPort(uint16_t bind_port)
{
    ((PSAGIPBindNotify)m_pucPDU)->bind_Port = htons(bind_port);
}
uint16_t AGIPBindNotify::getBindPort(void)
{
    return ntohs(((PSAGIPBindNotify)m_pucPDU)->bind_Port);
}

void AGIPBindNotify::setOpType(uint16_t op_type)
{
    ((PSAGIPBindNotify)m_pucPDU)->op_Type = htons(op_type);
}
uint16_t AGIPBindNotify::getOpType(void)
{
    return ntohs(((PSAGIPBindNotify)m_pucPDU)->op_Type);
}

int AGIPBindNotify::showInfo()
{
    SysProtocol::showInfo();

    char    buff[32];
    IP2String(getBindIP(), buff);
    buff[31] = 0;

    printf("--------------------------------------------------------AGIPBindNotify\n");
    printf("result_Code:     %d\n", getResultCode());
    printf("bind_IP:         %s\n", buff);
    printf("bind_Port:       %u\n", getBindPort());
    printf("op_Type:         %d\n", getOpType());
    printf("--------------------------------------------------------AGIPBindNotify\n");

    return S_SUCCESS;
}

