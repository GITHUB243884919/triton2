#include "AGIPUserFastCharge.h"

AGIPUserFastCharge::AGIPUserFastCharge(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_FAST_CHARGE, sizeof(SAGIPUserFastCharge))
{
}

AGIPUserFastCharge::~AGIPUserFastCharge(void)
{
}

int AGIPUserFastCharge::GetUserID(uint32_t& user_id) const
{
    user_id = ntohl(PDU()->user_id);
    return S_SUCCESS;
}

int AGIPUserFastCharge::GetCardNum(char* card_num) const
{
    ASSERT(NULL != card_num);
    strncpy(card_num, PDU()->card_num, AGIP_CARD_NUM_LEN);
    return S_SUCCESS;
}

int AGIPUserFastCharge::GetCardCode(char* card_code) const
{
    ASSERT(NULL != card_code);
    strncpy(card_code, PDU()->card_code, AGIP_CARD_CODE_LEN);
    return S_SUCCESS;
}

int AGIPUserFastCharge::GetUserIP(uint32_t& user_ip) const
{
    user_ip = ntohl(PDU()->user_ip);
    return S_SUCCESS;
}

int AGIPUserFastCharge::GetChargeFlag(uint8_t& charge_flag) const
{
    charge_flag = PDU()->charge_flag;
    return S_SUCCESS;
}

int AGIPUserFastCharge::GetDestGatewayID(uint32_t& dest_gateway_id) const
{
    dest_gateway_id = ntohl(PDU()->dest_gateway_id);
    return S_SUCCESS;
}

int AGIPUserFastCharge::GetDestUserName(char* dest_user_name) const
{
    ASSERT(NULL != dest_user_name);
    strncpy(dest_user_name, PDU()->dest_user_name, AGIP_USER_NAME_LEN);
    return S_SUCCESS;
}

int AGIPUserFastCharge::SetUserID(uint32_t user_id)
{
    PDU()->user_id = htonl(user_id);
    return S_SUCCESS;
}

int AGIPUserFastCharge::SetCardNum(const char* card_num)
{
    ASSERT(NULL != card_num);
    strncpy(PDU()->card_num, card_num, AGIP_CARD_NUM_LEN);
    return S_SUCCESS;
}

int AGIPUserFastCharge::SetCardCode(const char* card_code)
{
    ASSERT(NULL != card_code);
    strncpy(PDU()->card_code, card_code, AGIP_CARD_CODE_LEN);
    return S_SUCCESS;
}

int AGIPUserFastCharge::SetUserIP(uint32_t user_ip)
{
    PDU()->user_ip = htonl(user_ip);
    return S_SUCCESS;
}

int AGIPUserFastCharge::SetChargeFlag(uint8_t charge_flag)
{
    PDU()->charge_flag = charge_flag;
    return S_SUCCESS;
}

int AGIPUserFastCharge::SetDestGatewayID(uint32_t dest_gateway_id)
{
    PDU()->dest_gateway_id = htonl(dest_gateway_id);
    return S_SUCCESS;
}

int AGIPUserFastCharge::SetDestUserName(const char* dest_user_name)
{
    ASSERT(NULL != dest_user_name);
    strncpy(PDU()->dest_user_name, dest_user_name, AGIP_USER_NAME_LEN);
    return S_SUCCESS;
}

int AGIPUserFastCharge::showInfo()
{
    uint32_t user_id = 0;
    char card_num[AGIP_CARD_NUM_LEN+1] = {0};
    char card_code[AGIP_CARD_CODE_LEN+1] = {0};
    uint32_t user_ip = 0;
    uint8_t charge_flag = 0;
    uint32_t dest_gateway_id = 0;
    char dest_user_name[AGIP_USER_NAME_LEN+1] = {0};
    char user_ip_string[AGIP_MAX_IP_LEN+1] = {0};

    GetUserID(user_id);
    GetCardNum(card_num);
    GetCardCode(card_code);
    GetUserIP(user_ip);
    IP2String(user_ip, user_ip_string);
    GetChargeFlag(charge_flag);
    GetDestGatewayID(dest_gateway_id);
    GetDestUserName(dest_user_name);

    SysProtocol::showInfo();

    printf("--------------------------------------------------AGIPUserFastCharge\n");
    printf("User_ID:         %u\n", user_id);
    printf("Card_Num:        %s\n", card_num);
    printf("Card_Code:       %s\n", card_code);
    printf("User_IP:         %s\n", user_ip_string);
    printf("Charge_Flag:     %u\n", charge_flag);
    printf("Dest_Gateway_ID: %u\n", dest_gateway_id);
    printf("Dest_User_Name:  %s\n", dest_user_name);
    printf("--------------------------------------------------AGIPUserFastCharge\n");

    return S_SUCCESS;
}

// AGIPUserFastChargeRes -----------------------------------------------------------

AGIPUserFastChargeRes::AGIPUserFastChargeRes(void) : 
    SysProtocol(AGIP_CURRENT_VERSION, CMD_USER_FAST_CHARGE_RES, sizeof(SAGIPUserFastChargeRes))
{
}

AGIPUserFastChargeRes::~AGIPUserFastChargeRes(void)
{
}

int AGIPUserFastChargeRes::getResultCode(int32_t&  result_code) const
{
    result_code = ntohl(PDU()->result_code);
    return S_SUCCESS;
}

int AGIPUserFastChargeRes::getChargeAmount(int32_t& charge_amount) const
{
    charge_amount = ntohl(PDU()->charge_amount);
    return S_SUCCESS;
}

int AGIPUserFastChargeRes::setResultCode(int32_t result_code)
{
    PDU()->result_code = htonl(result_code);
    return S_SUCCESS;
}

int AGIPUserFastChargeRes::setChargeAmount(int32_t charge_amount)
{
    PDU()->charge_amount = htonl(charge_amount);
    return S_SUCCESS;
}

int AGIPUserFastChargeRes::showInfo()
{
    int32_t result_code = 0;
    int32_t charge_amount = 0;

    getResultCode(result_code);
    getChargeAmount(charge_amount);

    SysProtocol::showInfo();

    printf("--------------------------------------------------AGIPUserFastChargeRes\n");
    printf("Result_Code:        %d\n", result_code);
    printf("Charge_Amount:      %d\n", charge_amount);
    printf("--------------------------------------------------AGIPUserFastChargeRes\n");

    return S_SUCCESS;
}

