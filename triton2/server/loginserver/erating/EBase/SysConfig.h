#ifndef __SYS_CONFIG_H__
#define __SYS_CONFIG_H__
#include "Macro.h"
#include <map>
#include <string>
using namespace std;

class Engine_Export SysConfig
{
    typedef map<string, string> KEYVALUE;
public:
    SysConfig(void);
    virtual int initialize();
    int load(const char* pcConfigFileName, int32_t *pnLineParsed);
    virtual int release();
public:
    virtual ~SysConfig(void);

    int getIntVal(const char *pcKey, int32_t *pnIntValue, int32_t nDefaultValue = 0);
    int getStringVal(const char *pcKey, char *pcStringValue, const char* pcDefaultValue = "\0");

    int getItemCount(size_t *pstItemCount);

private:
    char m_pcConfigFileName[FILE_MAX_PATH];
    KEYVALUE m_confData;
    int ltrim(char *buffer);
    int rtrim(char *buffer);
};

#endif
