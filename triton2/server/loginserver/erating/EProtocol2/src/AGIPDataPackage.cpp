#include "AGIPDataPackage.h"

#undef offsetof
#define offsetof(T,M) ((size_t) &((T *)0)->M)

AGIPDataPackage::AGIPDataPackage() :
    SysProtocol(AGIP_CURRENT_VERSION, CMD_DATA_PACKAGE, sizeof(SAGIPDataPackage))
{
    setSubLength(LENGOFSUBLENGTH);
}

AGIPDataPackage::~AGIPDataPackage()
{
}

uint16_t AGIPDataPackage::getSubLength(void) const
{
    return (uint16_t)ntohs(((PSAGIPDataPackage)m_pucPDU)->sub_Length);
}

void AGIPDataPackage::setSubLength(uint16_t sub_len)
{
    ((PSAGIPDataPackage)m_pucPDU)->sub_Length = (uint16_t)htons(sub_len);
}

int16_t AGIPDataPackage::getPackageType(void) const
{
    return ntohs(((PSAGIPDataPackage)m_pucPDU)->package_Type);
}

void AGIPDataPackage::setPackageType(int16_t package_type)
{
    ((PSAGIPDataPackage)m_pucPDU)->package_Type = htons(package_type);
}

uint32_t AGIPDataPackage::getIP(void) const
{
    return (uint32_t)ntohl(((PSAGIPDataPackage)m_pucPDU)->ip);
}

void AGIPDataPackage::setIP(uint32_t ip)
{
    ((PSAGIPDataPackage)m_pucPDU)->ip = (uint32_t)htonl(ip);
}

uint16_t AGIPDataPackage::getPort(void) const
{
    return (uint16_t)ntohs(((PSAGIPDataPackage)m_pucPDU)->port);
}

void AGIPDataPackage::setPort(uint16_t port)
{
    ((PSAGIPDataPackage)m_pucPDU)->port = (uint16_t)htons(port);
}

int16_t AGIPDataPackage::getFlag(void) const
{
    return ntohs(((PSAGIPDataPackage)m_pucPDU)->flag);
}

void AGIPDataPackage::setFlag(int16_t flag)
{
    ((PSAGIPDataPackage)m_pucPDU)->flag = htons(flag);
}

uint32_t AGIPDataPackage::getPackageTimeL(void)
{
    return ntohl(((PSAGIPDataPackage)m_pucPDU)->package_Time_L);
}

void AGIPDataPackage::setPackageTimeL(uint32_t package_time_l)
{
    ((PSAGIPDataPackage)m_pucPDU)->package_Time_L = htonl(package_time_l);
}

uint16_t AGIPDataPackage::getTotalLength(void) const
{
    return (uint16_t)ntohs(m_pSAGIPHeader->us_Total_Length);
}

uint16_t AGIPDataPackage::getDataLength(void) const
{
    return getTotalLength() - sizeof(SAGIPDataPackage);
}

uint32_t AGIPDataPackage::getCmdID(void) const
{
    return ntohl(((PSAGIPDataPackage)m_pucPDU)->command_ID);
}

void AGIPDataPackage::setCmdID(uint32_t command_id)
{
    ((PSAGIPDataPackage)m_pucPDU)->command_ID = htonl(command_id);
}

uint32_t AGIPDataPackage::getData(void *buff_ref, size_t buff_size) const
{
    uint16_t data_len = getDataLength();

    // 数据长度为0
    if (data_len == 0 || buff_size < data_len)
        return 0;

    memcpy(buff_ref, (m_pucPDU + offsetof(SAGIPDataPackage, checksum)), data_len);

    return data_len;
}

int AGIPDataPackage::setData(const void *data, size_t data_len)
{
    if (data_len > AGIP_MAX_BUFFER_SIZE - sizeof(SAGIPDataPackage))
    {
        return E_PDU_LENGTH_ERROR;
    }

    memcpy(m_pucPDU + offsetof(SAGIPDataPackage, checksum), data, data_len);
    m_usTotalLength += data_len;
    setTotalLength(m_usTotalLength);
    return S_SUCCESS;
}

int AGIPDataPackage::setData(SysProtocol *proto)
{
    int result;
    int32_t data_len;
    // setCmdID? setGatewayID? setGameID?
    result = proto->getDecodedPDU(m_pucPDU + offsetof(SAGIPDataPackage, checksum),
            AGIP_MAX_BUFFER_SIZE, &data_len);
    if (S_SUCCESS != result)
    {
        return result;
    }

    m_usTotalLength += (uint16_t)data_len;
    setTotalLength(m_usTotalLength);
    return S_SUCCESS;
}

int AGIPDataPackage::showInfo()
{
    SysProtocol::showInfo();

    uint32_t    command_id;
    char        ip_string[32];
    uint32_t    ip = getIP();

    uint8_t*    pucIPSeg = NULL;
    const size_t MAX_IP_LEN = 16;

    pucIPSeg = (uint8_t*)&ip;
    snprintf(ip_string, MAX_IP_LEN, "%u.%u.%u.%u",
#if (defined(WIN32) || defined(LINUX))
            pucIPSeg[3], pucIPSeg[2], pucIPSeg[1], pucIPSeg[0]
#else
            pucIPSeg[0], pucIPSeg[1], pucIPSeg[2], pucIPSeg[3]
#endif
        );

    getCommandID(&command_id);
    printf(
            "--------------------------------------------------------AGIPDataPackage\n"
            "CommandID:         %#x\n"
            "PackageType:       %d\n"
            "IP:                %s\n"
            "Port:              %u\n"
            "Flag:              %d\n"
            "-------------------!!\n"
            "CmdID:             %#x\n"
            "DataLength:        %u\n"
            "--------------------------------------------------------AGIPDataPackage\n",
            command_id,
            getPackageType(),
            ip_string,
            getPort(),
            getFlag(),
            // --
            getCmdID(),
            getDataLength()
          );

    return S_SUCCESS;
}


