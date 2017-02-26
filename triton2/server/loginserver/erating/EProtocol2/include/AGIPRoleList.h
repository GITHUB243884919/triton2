#ifndef __AGIP_ROLE_LIST_H__
#define __AGIP_ROLE_LIST_H__

#include "SysProtocol.h"

#define CMD_ROLE_LIST                   0x10003101
#define CMD_ROLE_LIST_RES               0x20003101

typedef struct _AGIP_ROLE_LIST_
{
    _AGIP_HEADER_   header;
    uint32_t        un_User_ID;
    _AGIP_CHECKSUM_ checksum;
} SAGIPRoleList, *PSAGIPRoleList;

typedef struct _AGIP_ROLE_LIST_RES_
{
    _AGIP_HEADER_   header;
    int32_t         n_Result_Code;
    int32_t         n_Role_Count;
    uint32_t        un_Role_ID;
    char            str_Role_Name[AGIP_ROLE_NAME_LEN];
    int8_t          cRoleGender;
    int8_t          cRoleOccupation;
    uint32_t        unRoleCreateTime;
    _AGIP_CHECKSUM_ checksum;
} SAGIPRoleListRes, *PSAGIPRoleListRes;
/*
    uint32_t unRoleID;
    char     strRoleName[AGIP_ROLE_NAME_LEN];
    int8_t   cRoleGender;
    int8_t   cRoleOccupation;
    uint32_t unRoleCreateTime;
 */

class Engine_Export AGIPRoleList :
    public SysProtocol
{
public:
    AGIPRoleList(void);
    
    int getUserID(uint32_t *punUserID);

    int setUserID(uint32_t unUserID);
    virtual int showInfo();

public:
    virtual ~AGIPRoleList(void);
};

class Engine_Export AGIPRoleListRes :
    public SysProtocol
{
    friend class AGIPRoleList;
public:
    AGIPRoleListRes(void);
    virtual ~AGIPRoleListRes(void);

    virtual int initialize();
public:
    int getResultCode(int32_t *pnResultCode);
    int getRoleCount(int32_t *pnRoleCount);
    // int getRoleInfo(int32_t nIndex, uint32_t *punRoleID, char *strRoleName);
    int getRoleInfo(
            int32_t         nIndex, 
            uint32_t*       punRoleID, 
            char*           strRoleName,
            int8_t*         pcRoleGender,
            int8_t*         pcRoleOccupation,
            uint32_t*       punRoleCreateTime);
    int getRoleInfo(int32_t nIndex, ROLE_INFO *pRoleInfo);


    int setResultCode(int32_t nResultCode);
    // int addRoleInfo(uint32_t unRoleID, const char *strRoleName);
    // int addRoleInfo(const ROLE_INFO *pRoleInfo);
    int addRoleInfo(
            uint32_t    unRoleID, 
            const char* strRoleName,
            int8_t      cRoleGender,
            int8_t      cRoleOccupation,
            uint32_t    unRoleCreateTime);
    int addRoleInfo(const ROLE_INFO *pRoleInfo);

    virtual int showInfo();
    
private:
    int setRoleCount(int32_t nRoleCount);
};


#endif

