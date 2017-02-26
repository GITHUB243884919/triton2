/************************************************************
  Copyright ?  2002-2003,
  FileName:     ConfigFile.h
  Author:        
  Version:      1.0
  Date:         2002年5月8日
  Description:
    实现对配置文件的读取。
    使用方法如下：
      1 创建本类的对象实例，在构造函数中传入配置文件的文件名
      2 调用成员函数IsOpen()判断是否读取成功
      3 调用GetItemValue得到具体的值
  Others: 
  Function List:
    1. CConfigFile::CConfigFile
       构造函数，初始化类的成员，读取配置文件
    2. CConfigFile::~CConfigFile
       析构函数，释放资源
    3. CConfigFile::IsOpen
       返回配置文件读取状态
    4. CConfigFile::GetItemValue字符串
       取指定的键值
    5. CConfigFile::LocateSection
       定位section的开始地址和结束地址
    6. CConfigFile::LocateKeyValue
       在指定的缓冲区范围中搜索Key，返回与Key匹配的值的开始地址和结束地址
    7. CConfigFile::LocateStr
       在一个字符串中搜索另一个字符串
    8. CConfigFile::MapToContent
       将Shadow中的地址映射到Content中
    9. CConfigFile::MapToShadow
       将Content中的地址映射到Shadow中
    10.CConfigFile::uT_main
       类的测试用成员主函数。静态函数
    11.CConfigFile::uT_GetContentBuffer
    12.CConfigFile::uT_SetBuffer
    13.CConfigFile::uT_ClearBuffer
    14.CConfigFile::uT_SetContent
    15.CConfigFile::uT_SetShadow
    16.CConfigFile::uT_SetIsOpenFlag
    17.ToLower
       将字符串中的大写字母转换成小写字母
    18.CConfigFile::OpenFile
       加载配置文件到内存
    19.CConfigFile::CloseFile
       释放配置文件加载到内存后占用的资源
    20.CConfigFile::SetItemValue字符串
       从内存缓冲区中找到KeyName，将值拷贝到指定的空间
    21.CConfigFile::LocateKeyRange
       在指定的缓冲区范围中搜索Key，返回与Key匹配的值的开始地址和结束地址
    22.CConfigFile::SetItemValue数字
    23.CConfigFile::GetItemValue数字
    24.CConfigFile::GetItemValue带缺省值数字
    25.CConfigFile::GetItemValue带缺省值字符串
  History: 
             2002/05/08        1.0        建立本模块
             2002/05/13        1.0        增加SetItemValue字符串,
                                             OpenFile, CloseFile,
                                             LocateKeyRange
             2002/06/08        1.0        增加SetItemValue数字,
                                             增加GetItemValue数字
             2002/06/10        1.0        使GetItemValue, SetItemValue支持资源锁定
             2002/06/11        1.0        增加带缺省值的GetItemValue数字, GetItemValue字符串
***********************************************************/

#ifndef __CONFIGFILE_H__
#define __CONFIGFILE_H__

#include <stdio.h>


class CConfigFile
{
private:
    char    *m_pszContent;      /* 配置文件的原始内容 */
    char    *m_pszShadow;       /* 配置文件的内容全部转换成小写 */
    size_t  m_nSize;            /* 配置文件内容的长度，不包括最后的NULL */
    short   m_bIsOpen;          /* 配置文件是否打开成功的标志 */

public:
    char    *m_pszFilename;     /* 存放需要读取的配置文件名 */
	CConfigFile();  
    ~CConfigFile();

    unsigned int IsOpen();
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        char *pszReturnedString, 
                        unsigned int nSize );
    unsigned int SetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        const char *pszKeyValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int &lReturnedValue );
     unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        short &lReturnedValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        unsigned short &lReturnedValue );
    unsigned int SetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int lKeyValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int &lReturnedValue,
                        int lDefaultValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        char *pszReturnedString, 
                        unsigned int nSize, 
                        const char *pszDefaultValue);

	int  OpenFile(const char *pszFilename);
    
	void CloseFile();

private:
    
    unsigned int LocateSection(const char *pszSectionName, 
                        char * &pszSectionBegin, 
                        char * &pszSectionEnd);
    unsigned int LocateKeyRange(const char *pszKeyName, 
                        const char *pszSectionBegin, 
                        const char *pszSectionEnd, 
                        char * &pszKeyBegin, 
                        char * &pszKeyEnd);
    unsigned int LocateKeyValue(const char *pszKeyName, 
                        const char *pszSectionBegin, 
                        const char *pszSectionEnd, 
                        char * &pszValueBegin, 
                        char * &pszValueEnd);
    char *LocateStr(    const char *pszCharSet, 
                        const char *pszBegin, 
                        const char *pszEnd );
	char *SearchMarchStr(const char *pszBegin, const char *pszCharSet);

    char *MapToContent(const char *p);
    char *MapToShadow(const char *p);

	void ToLower( char * pszSrc, size_t len);

};


#endif

