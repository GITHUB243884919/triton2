#include "AGIPCreateOrUpdateUser.h"

AGIPCreateOrUpdateUser::AGIPCreateOrUpdateUser(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_CREATE_OR_UPDATE_USER, sizeof(SAGIPCreateOrUpdateUser))
{
}

AGIPCreateOrUpdateUser::~AGIPCreateOrUpdateUser(void)
{
}

// Get Method
int AGIPCreateOrUpdateUser::GetUserID(uint32_t& user_id)
{
    user_id = ntohl(((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_id);
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::GetUserName(char* user_name)
{
    strncpy(user_name, ((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_name, AGIP_USER_NAME_LEN);

    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::GetPassword(char* password)
{
    strncpy(password, ((SAGIPCreateOrUpdateUser*)m_pucPDU)->password, AGIP_PASSWORD_LEN);

    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::GetUserType(int8_t& user_type)
{
    user_type = ((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_type;
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::GetUserClass(int8_t& user_class)
{
    user_class = ((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_class;
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::GetUserPoint(int32_t& user_point)
{
    user_point = ntohl(((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_point);
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::GetPromoterID(uint32_t& promoter_id)
{
    promoter_id = ntohl(((SAGIPCreateOrUpdateUser*)m_pucPDU)->promoter_id);
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::GetUserFlag(uint32_t&   user_flag)
{
    user_flag = ntohl(((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_flag);
    return S_SUCCESS;
}

// Set Method
int AGIPCreateOrUpdateUser::SetUserID(uint32_t user_id)
{
    ((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_id = htonl(user_id);
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::SetUserName(const char* user_name)
{
    strncpy(((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_name, 
            user_name, AGIP_USER_NAME_LEN);

    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::SetPassword(const char* password)
{
    strncpy(((SAGIPCreateOrUpdateUser*)m_pucPDU)->password, 
            password, AGIP_PASSWORD_LEN);

    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::SetUserType(int8_t user_type)
{
    ((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_type = user_type;
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::SetUserClass(int8_t user_class)
{
    ((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_class = user_class;
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::SetUserPoint(int32_t user_point)
{
    ((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_point = htonl(user_point);
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::SetPromoterID(uint32_t promoter_id)
{
    ((SAGIPCreateOrUpdateUser*)m_pucPDU)->promoter_id = htonl(promoter_id);
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::SetUserFlag(uint32_t user_flag)
{
    ((SAGIPCreateOrUpdateUser*)m_pucPDU)->user_flag = htonl(user_flag);
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUser::showInfo()
{
    int nRetCode        = E_ERROR;

    uint32_t        user_id;
    char            user_name[AGIP_USER_NAME_LEN+1] = {0};
    char            password[AGIP_PASSWORD_LEN+1] = {0};
    int8_t          user_type;
    int8_t          user_class;
    int32_t         user_point;
    uint32_t        promoter_id;
    uint32_t        user_flag;

    SysProtocol::showInfo();
    nRetCode = GetUserID(user_id);
    nRetCode = GetUserName(user_name);
    nRetCode = GetPassword(password);
    nRetCode = GetUserType(user_type);
    nRetCode = GetUserClass(user_class);
    nRetCode = GetUserPoint(user_point);
    nRetCode = GetPromoterID(promoter_id);
    nRetCode = GetUserFlag(user_flag);

    printf("----------------------------------------------AGIPCreateOrUpdateUser\n");
    printf("User_ID:            %u\n", user_id);
    printf("User_Name:          %s\n", user_name);
    printf("Password:           %s\n", password);
    printf("User_Type:          %d\n", user_type);
    printf("User_Class:         %d\n", user_class);
    printf("User_Point:         %d\n", user_point);
    printf("Promoter_ID:        %u\n", promoter_id);
    printf("User_Flag:          %u\n", user_flag);
    printf("----------------------------------------------AGIPCreateOrUpdateUser\n");

    return S_SUCCESS;
}

// --------------------------------------------------------------------------

AGIPCreateOrUpdateUserRes::AGIPCreateOrUpdateUserRes(void)
: SysProtocol(AGIP_CURRENT_VERSION, CMD_CREATE_OR_UPDATE_USER_RES, sizeof(SAGIPCreateOrUpdateUserRes))
{
}

AGIPCreateOrUpdateUserRes::~AGIPCreateOrUpdateUserRes(void)
{
}

// Get Method
int AGIPCreateOrUpdateUserRes::GetResultCode(int32_t& result)
{
    result = ntohl(((SAGIPCreateOrUpdateUserRes*)m_pucPDU)->result);
    return S_SUCCESS;
}

// Set Method
int AGIPCreateOrUpdateUserRes::SetResultCode(int32_t result)
{
    ((SAGIPCreateOrUpdateUserRes*)m_pucPDU)->result = htonl(result);
    return S_SUCCESS;
}

int AGIPCreateOrUpdateUserRes::showInfo()
{
    int nRetCode        = E_ERROR;

    int32_t         result;

    SysProtocol::showInfo();
    nRetCode = GetResultCode(result);
    printf("--------------------------------------------------AGIPCreateOrUpdateUserRes\n");
    printf("Result_Code:        %d\n", result);
    printf("--------------------------------------------------AGIPCreateOrUpdateUserRes\n");

    return S_SUCCESS;
}

