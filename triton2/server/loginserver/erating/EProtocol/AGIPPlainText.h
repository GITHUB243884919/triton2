// Plain text AGIP protocol.
// Copyright(C) 2009 LineKong.com
// @file AGIPPlainText.h
// @author: Benjamin
// @date: 2009-05-02

#ifndef __AGIP_PLAIN_TEXT_H__
#define __AGIP_PLAIN_TEXT_H__

#include "SysProtocol.h"

#define CMD_PLATFORM_AUTHEN             0x30003001
#define CMD_PLATFORM_AUTHEN_RES         0x40003001
#define CMD_PLATFORM_CHARGE             0x30003002
#define CMD_PLATFORM_CHARGE_RES         0x40003002
#define CMD_PF_BALANCE_QUERY            0x30003402
#define CMD_PF_BALANCE_QUERY_RES        0x40003402
#define CMD_PF_ITEM_SEND                0x30003504
#define CMD_PF_ITEM_SEND_RES            0x40003504

#define __IN
#define __OUT
#define __IN_OUT

class Engine_Export AGIPPlainText :
    public SysProtocol
{
public:
    AGIPPlainText(uint8_t ucVersion, uint32_t unCommandID,
        char sep1, char sep2, char sep3 = ';', char sep4 = ':', char sep5 = '|');
    virtual ~AGIPPlainText(void);

    int getParameter(const char *szName, char* szValue, int nValLen) const;
    int setParameter(const char *szName, const char* szValue);

    virtual int showInfo();
protected:
    char m_cParamSep;   // parameter separator, such as &, ;, <, etc.
    char m_cKeyValSep;  // key&value separator, such as =, :, >, etc.
    char m_cParamSep2;  // sub-parameter separator, such as &, ;, <, etc.
    char m_cKeyValSep2; // sub-key&value separator, such as =, :, >, etc.
    char m_cGroupSep;   // group separator, such as |, #, etc.
};

//////////////////////////////////////////////////////////////////////////
class Engine_Export AGIPPlatformAuthen :
    public AGIPPlainText
{
public:
    AGIPPlatformAuthen(char sep1, char sep2)
        : AGIPPlainText(AGIP_CURRENT_VERSION, CMD_PLATFORM_AUTHEN, sep1, sep2){}
    virtual ~AGIPPlatformAuthen(){}

};

//////////////////////////////////////////////////////////////////////////
class Engine_Export AGIPPlatformAuthenRes :
    public AGIPPlainText
{
public:
    AGIPPlatformAuthenRes(char sep1, char sep2)
        : AGIPPlainText(AGIP_CURRENT_VERSION, CMD_PLATFORM_AUTHEN_RES, sep1, sep2){}
    virtual ~AGIPPlatformAuthenRes(){}

};

//////////////////////////////////////////////////////////////////////////
class Engine_Export AGIPPlatformCharge :
    public AGIPPlainText
{
public:
    AGIPPlatformCharge(char sep1, char sep2) :
        AGIPPlainText(AGIP_CURRENT_VERSION, CMD_PLATFORM_CHARGE, sep1, sep2) {}
    virtual ~AGIPPlatformCharge(void) {}
};

//////////////////////////////////////////////////////////////////////////
class Engine_Export AGIPPlatformChargeRes :
    public AGIPPlainText
{
public:
    AGIPPlatformChargeRes(char sep1, char sep2) :
        AGIPPlainText(AGIP_CURRENT_VERSION, CMD_PLATFORM_CHARGE_RES, sep1, sep2) {}
    virtual ~AGIPPlatformChargeRes(void) {}
};

//////////////////////////////////////////////////////////////////////////
class Engine_Export AGIPPfBalanceQuery : public AGIPPlainText
{
public:
    AGIPPfBalanceQuery(char sep1, char sep2) :
        AGIPPlainText(AGIP_CURRENT_VERSION, CMD_PF_BALANCE_QUERY, sep1, sep2) {}
    virtual ~AGIPPfBalanceQuery(void) {}
};

//////////////////////////////////////////////////////////////////////////
class Engine_Export AGIPPfBalanceQueryRes : public AGIPPlainText
{
public:
    struct BalanceInfo
    {
        int nSubjectID;
        int nAmount;
    };

    AGIPPfBalanceQueryRes(char sep1, char sep2, char sep3, char sep4, char sep5) :
        AGIPPlainText(AGIP_CURRENT_VERSION, CMD_PF_BALANCE_QUERY_RES, sep1, sep2, sep3, sep4, sep5){}
    virtual ~AGIPPfBalanceQueryRes(void) {}
    int setBalanceDetail(int nCount, const BalanceInfo* psBalanceArray);
    int setBalanceDetail(int nCount, const int* pcSubjectArray, const int* pnAmountArray);
    int getBalanceDetail(__IN_OUT int* pnCount, BalanceInfo* psBalanceArray);
    int getBalanceDetail(__IN_OUT int* pnCount, int* pcSubjectArray, int* pnAmountArray);

};

//////////////////////////////////////////////////////////////////////////
class Engine_Export AGIPPfItemSend : public AGIPPlainText
{
public:
    struct ItemInfo
    {
        char szItemCode[AGIP_ITEM_CODE_LEN+1];
        int  nItemNum;
    };

    AGIPPfItemSend(char sep1, char sep2, char sep3, char sep4, char sep5) :
    AGIPPlainText(AGIP_CURRENT_VERSION, CMD_PF_ITEM_SEND, sep1, sep2, sep3, sep4, sep5){}
    virtual ~AGIPPfItemSend(void) {}
    int setItemDetail(int nCount, const ItemInfo* psItemArray);
    int setItemDetail(int nCount, const void* pItemCodeArray, int pnElementSize, const int* pnItemNumArray);
    int getItemDetail(__IN_OUT int* pnCount, ItemInfo* psItemArray);
    int getItemDetail(__IN_OUT int* pnCount, void* pItemCodeArray, int nElementSize, int* pnItemNumArray);

};

//////////////////////////////////////////////////////////////////////////
class Engine_Export AGIPPfItemSendRes : public AGIPPlainText
{
public:
    AGIPPfItemSendRes(char sep1, char sep2) :
      AGIPPlainText(AGIP_CURRENT_VERSION, CMD_PF_ITEM_SEND_RES, sep1, sep2) {}
      virtual ~AGIPPfItemSendRes(void) {}
};

#endif//(__AGIP_PLAIN_TEXT_H__)

