#ifndef __AGIP_USER_FAST_CHRAGE_H__
#define __AGIP_USER_FAST_CHRAGE_H__

#include "SysProtocol.h"

#define CMD_USER_FAST_CHARGE                 0x10003401
#define CMD_USER_FAST_CHARGE_RES             0x20003401

typedef struct _AGIP_USER_FAST_CHRAGE_
{
    _AGIP_HEADER_   header;
    uint32_t        user_id;
    char            card_num[AGIP_CARD_NUM_LEN];
    char            card_code[AGIP_CARD_CODE_LEN];
    uint32_t        user_ip;
    uint8_t         charge_flag;
    int8_t          pad1;
    int16_t         pad2;
    uint32_t        dest_gateway_id;
    char            dest_user_name[AGIP_USER_NAME_LEN];
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserFastCharge, *PSAGIPUserFastCharge;

class Engine_Export AGIPUserFastCharge :
    public SysProtocol
{
public:
    AGIPUserFastCharge(void);
    virtual ~AGIPUserFastCharge(void);

    int GetUserID(uint32_t& user_id) const;
    int GetCardNum(char* card_num) const;
    int GetCardCode(char* card_code) const;
    int GetUserIP(uint32_t& user_ip) const;
    int GetChargeFlag(uint8_t& charge_flag) const;
    int GetDestGatewayID(uint32_t& dest_gateway_id) const;
    int GetDestUserName(char* dest_user_naem) const;

    int SetUserID(uint32_t user_id);
    int SetCardNum(const char* card_num);
    int SetCardCode(const char* card_code);
    int SetUserIP(uint32_t user_ip);
    int SetChargeFlag(uint8_t charge_flag);
    int SetDestGatewayID(uint32_t dest_gateway_id);
    int SetDestUserName(const char* dest_user_naem);

    virtual int showInfo();

private:
    const SAGIPUserFastCharge* PDU(void) const
    {
        return (const SAGIPUserFastCharge*)m_pucPDU;
    }

    SAGIPUserFastCharge* PDU(void)
    {
        return (SAGIPUserFastCharge*)m_pucPDU;
    }
};

// ----------------------------------------------------------------------

typedef struct _AGIP_USER_FAST_CHRAGE_RES_
{
    _AGIP_HEADER_   header;
    int32_t         result_code;
    int32_t         charge_amount;
    _AGIP_CHECKSUM_ checksum;
} SAGIPUserFastChargeRes, *PSAGIPUserFastChargeRes;

class Engine_Export AGIPUserFastChargeRes :
    public SysProtocol
{
public:
    AGIPUserFastChargeRes(void);
    virtual ~AGIPUserFastChargeRes(void);

    int getResultCode(int32_t&  result_code) const;
    int getChargeAmount(int32_t& charge_amount) const;

    int setResultCode(int32_t result_code);
    int setChargeAmount(int32_t charge_amount);

    virtual int showInfo();

private:
    const SAGIPUserFastChargeRes* PDU(void) const
    {
        return (const SAGIPUserFastChargeRes*)m_pucPDU;
    }

    SAGIPUserFastChargeRes* PDU(void)
    {
        return (SAGIPUserFastChargeRes*)m_pucPDU;
    }
};

#endif /*__AGIP_USER_FAST_CHRAGE_H__*/

