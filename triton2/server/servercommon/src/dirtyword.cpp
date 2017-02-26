#include <stdio.h>			 
#include <errno.h>		 
#include <stdlib.h>			 
#include <string.h>		   
#include <sys/types.h>		  
#include <sys/ipc.h>		
#include <sys/shm.h>	  
#include <sys/stat.h>	   
#include <unistd.h>		 
#include "dirtyword.h"
#include "log.h"
//#ifndef _OI_DIRTY_CHECK_C_ 
//#define _OI_DIRTY_CHECK_C_
								
template<> CDirtyWord* CSingleton<CDirtyWord >::spSingleton = NULL;

int CDirtyWord::OI_Dirty_start( )
{		
	if( ReadDirtyWordDB() < 0 )
	{
		return -2;
	}
	return 0;
}

int CDirtyWord::ReadDirtyWordDB( )
{								
	Dirty_DB *tpTempDirtyDB = new Dirty_DB();
	if ( tpTempDirtyDB == NULL )
	{
		LOG_ERROR( "default","[%s,%d] Load DirtyWord Error!", __FUNCTION__,__LINE__ );
		return -1;
	}
	memset( tpTempDirtyDB, 0, sizeof( Dirty_DB ) );
	
	//从脏词库中获取中文脏词
	if( OI_Dirty_Chn_GetFromFile( "../config/data/dirtyword/cf.txt", tpTempDirtyDB ) ) 
	{		
		return -1;
	}	
	
	// 加载中文脏词
	if( OI_Dirty_Chn_LoadToShm( tpTempDirtyDB ) ) 
	{			
		return -2;
	}
	
	// 读取英文脏词
	memset(tpTempDirtyDB, 0, sizeof(Dirty_DB));
	if(OI_Dirty_Eng_GetFromFile("../config/data/dirtyword/ef.txt", tpTempDirtyDB))
	{	
		return -3;
	}
	
	// 加载英文脏词到内存中	
	if(OI_Dirty_Eng_LoadToShm(tpTempDirtyDB))
	{
		return -4;
	}		
	delete 	tpTempDirtyDB;	
	return 0;
}

int CDirtyWord::OI_Drity_Chn_Add_Item( unsigned char * sDirtyWord, unsigned char *sKeyWord, int iCount, short iLevel )
{
	OI_DIRTY_CHN_RECORD    *pstDirtyRecord;		
	unsigned char          *sSubstr;		 
	int                    iIndex,iOffset;	  
	if ( iCount >= C_MAX_WORD_NUM || iCount < 0 )	   
	{	
		//printf("The number of the words is too large or too small\n");	  
		return -1; 
	}
	
	pstDirtyRecord = mDirtyCore.astChnDirtyRec ;
	strcpy( ( char * )( pstDirtyRecord[iCount].sDirtyStr ), ( const char * )sDirtyWord );  
	strcpy( ( char * )( pstDirtyRecord[iCount].sKeyWord ), ( const char * )sKeyWord );	 
	pstDirtyRecord[iCount].iLevel = iLevel;				
	sSubstr = ( unsigned char * )strstr( ( const char * )sDirtyWord, ( const char * )sKeyWord );	 
	if ( sSubstr == NULL )
	{			  
		//printf("Keyword(%s) in this file error",sKeyWord);   
		return -3;	
	}			  
	pstDirtyRecord[iCount].iKeyOffset = ( sSubstr - sDirtyWord );	   
	pstDirtyRecord[iCount].iNextKey = -1;			 

#ifdef _DEBUG_ 

	//printf("Init_Chn_Table:KeyOffset:%d\n",pstDirtyRecord[iCount].iKeyOffset);   
	//printf("Init_Chn_Table:iCount:%d\n",iCount);

#endif					 


	CAL_INDEX_OFFSET( iIndex,sKeyWord[0], sKeyWord[1] )   
	iOffset	= mChnIndexTab.iDirtyIndexTable[iIndex];
	//	printf("%d %d\n",iOffset,iIndex);	  
	if ( iOffset == -1 )
	{		
		mChnIndexTab.iDirtyIndexTable[iIndex] = iCount; 
	}
	else
	{	 
		while ( iOffset != -1 )
		{	 
			if (strcmp( ( const char * )( pstDirtyRecord[iOffset].sDirtyStr ), ( const char * )sDirtyWord ) == 0 )
			{ 
				memset( &( pstDirtyRecord[iCount] ), 0, sizeof( pstDirtyRecord[iCount] ) );
				//	printf("Conflict word(%s) with (%s)\n", sDirtyWord, pstDirtyRecord[iOffset].sDirtyStr);
				return 0; 
			}	
			iIndex = iOffset;		 
			iOffset = pstDirtyRecord[iOffset].iNextKey;  
		}  
		pstDirtyRecord[iIndex].iNextKey = iCount;	 
	}  
	return 0;	
}

int CDirtyWord::OI_Dirty_Eng_LoadToShm(Dirty_DB *pstDirtyDB)
{	 
	int iCount = 0;	  
	mDirtyCore.iEnable = 0;	  
	mDirtyCore.aiEngWordCount = 0;  
	memset( mDirtyCore.astEngDirtyRec, 0, sizeof( mDirtyCore.astEngDirtyRec ) );  
	for( iCount = 0; iCount < pstDirtyDB->iDirtyEngItemNum; iCount++ )
	{
		strcpy( ( char * )( mDirtyEngRec[iCount].sDirtyStr ), ( const char * )( pstDirtyDB->stDirtyEngItems[iCount].sDirtyStr ) );
		mDirtyEngRec[iCount].iLevel = pstDirtyDB->stDirtyEngItems[iCount].iLevel;	  
	}
		
	mDirtyCore.aiEngWordCount	= iCount; 
	mDirtyCore.ulEngVer			= pstDirtyDB->ulEngVer;	  
	mDirtyCore.iEnable			= 1;
	return 0;
}	

int CDirtyWord::OI_Dirty_Chn_GetFromFile( const char *szChnFile, Dirty_DB * pstDirtyDB )
{
	FILE *fpChnFile = NULL;	 
	char szLineBuf[1024];  
	char *pNextToken;	 
	int iCount;	   
	Dirty_Chn_Item *pcur; 	
	if( !szChnFile || !pstDirtyDB ) 
	{		   
		return -1;	 
	} 
	fpChnFile = fopen( szChnFile, "r" ); 
	if( !fpChnFile )	 
	{ 
		return -2;	
	}			   
	
	iCount = 0;	 
	for(;;)	 
	{
		fgets( ( char * )szLineBuf, sizeof( szLineBuf )-1, fpChnFile );
		if( feof( fpChnFile ) )
		{			   
			break;	  
		}	   
		TrimStr( szLineBuf );   //获取一个关键词	
		if( szLineBuf[0] == '#' )  //?   
		{		  
			continue;  
		}				   
		pNextToken = strtok( szLineBuf, " \t\r\n" );//分割字符串	  
		if( !pNextToken )	   
		{			
			continue;	  
		}		
		pcur = &pstDirtyDB->stDirtyChnItems[iCount];//把单词存入脏词库	  
		memset( pcur, 0, sizeof( Dirty_Chn_Item ) );	   
		pcur->iLevel = atoi( pNextToken ); //把字符串转换成整数，单词等级	
		pNextToken = strtok( NULL, "|\r\n" );	  
		if( !pNextToken )		  
		{			
			continue;	 
		}	 
		strncpy( ( char * )( pcur->sDirtyStr ), ( const char * )pNextToken, sizeof( pcur->sDirtyStr ) );//脏词   
		pcur->sDirtyStr[ sizeof( pcur->sDirtyStr )-1 ] = 0;//置位	 
		pNextToken = strtok( NULL, "\r\n" );			 
		if( pNextToken )	   
		{	 
			strncpy( ( char * )( pcur->sKeyWord ), ( const char * )pNextToken, sizeof( pcur->sKeyWord ) );  //关键词	
			pcur->sKeyWord[sizeof(pcur->sKeyWord)-1] = 0; 
		}
		iCount++;  

#ifdef _DEBUG_

		//printf("Level : %d, Dirty String : \"%s\", Key Words : \"%s\"\n",
		//pcur->iLevel, pcur->sDirtyStr, pcur->sKeyWord);

#endif 
	}	 
	
	// 添加一些特殊字符
	pcur = &pstDirtyDB->stDirtyChnItems[iCount];
	memset( pcur, 0, sizeof( Dirty_Chn_Item ) );	   
	pcur->iLevel = 2;		
	
	// 添加全角空格
#ifdef CODE_UTF8   	
	pcur->sDirtyStr[0] = ( char )227;
	pcur->sDirtyStr[1] = ( char )128;
	pcur->sDirtyStr[2] = ( char )128;
	pcur->sDirtyStr[3] = '\0';
	pcur->sDirtyStr[ sizeof( pcur->sDirtyStr )-1 ] = 0;	
#else
	strncpy( pcur->sDirtyStr, "　", sizeof( pcur->sDirtyStr ) );		 
#endif
	
	
	strncpy( ( char * )( pcur->sKeyWord ), ( const char * )pcur->sDirtyStr, sizeof( pcur->sKeyWord ) );
	pcur->sKeyWord[sizeof(pcur->sKeyWord)-1] = 0; 

	iCount++;  
	
	fclose( fpChnFile );	
	pstDirtyDB->iDirtyChnItemNum = iCount;		
	return 0;
}

int CDirtyWord::OI_Dirty_Chn_LoadToShm( Dirty_DB *pstDirtyDB )
{	 
	int iCount = 0;	
	Dirty_Chn_Item *pcur = NULL;		
	mDirtyCore.iEnable			= 0;	
	mDirtyCore.aiChnWordCount	= 0;	//过滤个数	 
	memset( mDirtyCore.astChnDirtyRec, 0, sizeof( mDirtyCore.astChnDirtyRec ) );
	for ( iCount = 0; iCount < C_MAX_TABLE_LEN; iCount++ )
	{		
		mChnIndexTab.iDirtyIndexTable[iCount] = -1; 
	}

	for( iCount =0; iCount < pstDirtyDB->iDirtyChnItemNum; iCount++ )
	{  
		pcur = &pstDirtyDB->stDirtyChnItems[iCount];  		
		if ( OI_Drity_Chn_Add_Item( pcur->sDirtyStr, pcur->sKeyWord, iCount, pcur->iLevel ) < 0 )
		{  
			//printf("Fail to init dirty word table! %d %s %s\n",pcur->iLevel,pcur->sKeyWord,pcur->sDirtyStr);
			mDirtyCore.iEnable = 1;	
			return -2;	
		}  
	} 
	mDirtyCore.aiChnWordCount	= iCount;
	mDirtyCore.ulChnVer			= pstDirtyDB->ulChnVer; 
	mDirtyCore.iEnable			=1;	   
	return 0;			 
}


//混合检查  只保留 中英数
int CDirtyWord::OI_Dirty_Mix_Check( int iLevel, unsigned char *sCheckStr, int iCheckType )	 
{     
	unsigned char	cLoChar,cHiChar;
	unsigned char *sReservBuff;	  
	int				iIsDirty = 0;
	int				iCount = 0;
	int				iIndex = 0;
	int				iOffset = 0;
	int				iKeyOff = 0;	
	OI_DIRTY_CHN_RECORD	*pstDirtyRecord; 
	if ( sCheckStr == NULL || strlen( ( const char * )sCheckStr ) == 0 )
	{ 
		return -1;
	}
	
	//获取待比较的字符串的内存指针
	sReservBuff=sCheckStr;		  
		
	//注释掉的部分
	iCount =0;	
	pstDirtyRecord =mDirtyCore.astChnDirtyRec;//获取脏词	
	while( ( cLoChar = sReservBuff[iCount++] ) != '\0' )//比较脏词	 
	{
		//if(  IS_ENGLISH_CHAR( cLoChar ) || IS_DIGIT( cLoChar ) || cLoChar == ' '
		//	|| cLoChar == '%'  )//如果是英文单词或者数字或者空格  
		if ( IS_ASCII_WORD( cLoChar ) )		
		{  
			iIndex=0; 
			while ( mDirtyEngRec[iIndex++].sDirtyStr[0] != '\0' )//如果英文脏字库？ 
			{ 				
				if( ( iLevel & mDirtyEngRec[iIndex-1].iLevel )  && EQUAL_ENGLISH_CHAR( cLoChar, mDirtyEngRec[iIndex-1].sDirtyStr[0] ) )		//屏蔽等级没有超过限定词的标记并且首字符相等
					
					if(strncasecmp((const char *)(sReservBuff+iCount-1), (const char *)( mDirtyEngRec[iIndex-1].sDirtyStr),
						strlen((const char *)( mDirtyEngRec[iIndex-1].sDirtyStr) ) ) == 0 )//含有脏字
					{
						if( iCheckType == 1 )
						{
							unsigned char *ChangeArea=sReservBuff+iCount-1;
							for (unsigned int i=0;i<strlen((const char *)( mDirtyEngRec[iIndex-1].sDirtyStr) );i++)
							{
								*ChangeArea='*';
								ChangeArea++;
							}
						}
						iIsDirty=4;
						iCount=iCount-1+strlen((const char *)( mDirtyEngRec[iIndex-1].sDirtyStr));	
						break;	
					}

			}//while engdirty  
		}//if eng char			
		else if(IS_DOUBLE_CHAR(cLoChar))	//是多字节字符	
		{	
			cHiChar=sReservBuff[iCount++];	//获取高位字节
			int tChByte = 2;
			#ifdef CODE_UTF8
			int tWordLength = GetByteOfUTF8( ( const char * )&cLoChar );
			if ( tWordLength > 2 )
			{
				tChByte += tWordLength - 2;
				iCount += tWordLength - 2;	
			}				
			#endif
			 
			if(cHiChar =='\0') break;		//如果高位字节为空则退出外层循环  
			if(IS_CHINESE_CHAR(cLoChar,cHiChar))//如果是中文字符   
			{											  
				CAL_INDEX_OFFSET(iIndex,cLoChar,cHiChar);		  
				iOffset= mChnIndexTab.iDirtyIndexTable[iIndex]; 
				
				//该字是否关键字//   
				while ((iOffset!=-1)&&(iOffset>=0)&&(iOffset<C_MAX_WORD_NUM))//比较中文字符	
				{	
					iKeyOff=pstDirtyRecord[iOffset].iKeyOffset;	  //获取脏字关键字的偏移量
						
					if(pstDirtyRecord[iOffset].sKeyWord[0]!=cLoChar	 
						||pstDirtyRecord[iOffset].sKeyWord[1]!=cHiChar)
							 
						//关键字不匹配可能发生在loadmem的时候
						break;

					if ( pstDirtyRecord[iOffset].iLevel & iLevel )//如果屏蔽等级达到指定值	
					{

						if( iCount - tChByte >= pstDirtyRecord[iOffset].iKeyOffset )   
							if( strncasecmp((const char *)(sReservBuff+(iCount-tChByte - pstDirtyRecord[iOffset].iKeyOffset) ),
								(const char *)(pstDirtyRecord[iOffset].sDirtyStr), strlen(( const char * )pstDirtyRecord[iOffset].sDirtyStr) )==0)//开始比较中文字符
							 {
								if( iCheckType == 1 )
								{
									unsigned char *ChangeArea=sReservBuff+iCount - tChByte - pstDirtyRecord[iOffset].iKeyOffset;							   
									for (unsigned int i=0;i<strlen((const char *)(pstDirtyRecord[iOffset].sDirtyStr));i++)
									{
										*ChangeArea='*';
										if(IS_CHINESE_CHAR((unsigned char)pstDirtyRecord[iOffset].sDirtyStr[i],(unsigned char)pstDirtyRecord[iOffset].sDirtyStr[i+1]))
										{											
											i++;
											#ifdef 	CODE_UTF8
											int tWordLength = GetByteOfUTF8( ( const char * )&cLoChar );
											if ( tWordLength > 2 )
											{									
												i += tWordLength - 2;	
											}														
											#endif									
										}									
										ChangeArea++;
									}                                
	                                
									unsigned char *DesChar=sReservBuff+iCount- tChByte +strlen((const char *)(pstDirtyRecord[iOffset].sDirtyStr));
									int copystrlen=strlen((const char *)DesChar);  
									iCount=ChangeArea-sReservBuff;	    
									for (int i=0;i<copystrlen;i++)
									{
										*ChangeArea=*DesChar;
										DesChar++;
										ChangeArea++;
									}
									*ChangeArea='\0';  
								}
								iIsDirty=3;		  
								break;			
							}			
					}	

					iOffset=pstDirtyRecord[iOffset].iNextKey;  // 下一个含有该关键字得短语/
				}//chndirty

			}// if chinese char		   		

		}//if doublechar

	}//end while	  
	//free((void*)sReservBuff);	 
	return iIsDirty;	  
}

int CDirtyWord::OI_Dirty_Check(int iLevel, char *sMsg, int iCheckType)
{
	int iRet = 0;		

	if( !mDirtyCore.iEnable ) 
	{
		return -2; 
	}	 
	iRet = OI_Dirty_Mix_Check( iLevel, ( unsigned char * )sMsg, iCheckType );  
	if( iRet ) return iRet;		 
	return 0;		 
}

int CDirtyWord::OI_Dirty_Eng_GetFromFile( const char *szEngFile, Dirty_DB * pstDirtyDB )
{																						 
	FILE *fpEngFile = NULL;				 
	char szLineBuf[1024] = { 0 };	   
	char *pNextToken = NULL;		   
	int iCount = 0;
	Dirty_Eng_Item *pcur = NULL;

	if( !szEngFile || !pstDirtyDB )  
	{				  
		return -1;		  
	}					
	fpEngFile = fopen( szEngFile, "r" ); 
	if( !fpEngFile ) 
	{			   
		return -2;	   
	}		 
	iCount = 0;	   
	for(;;)		  
	{
		fgets( ( char * )szLineBuf, sizeof( szLineBuf ) - 1, fpEngFile ); 
		if( feof( fpEngFile ) )	
		{		  
			break;		
		}		   
		TrimStr( szLineBuf );	 
		if( szLineBuf[0] == '#' )	
		{		  
			continue;  
		}	
		pNextToken = strtok( szLineBuf, " \t\r\n" );	
		if( !pNextToken )	 
		{		 
			continue;	   
		}									 
		pcur = &pstDirtyDB->stDirtyEngItems[iCount];
		memset( pcur, 0, sizeof( Dirty_Chn_Item ) ); 
		pcur->iLevel = atoi( pNextToken );	 
		pNextToken = strtok( NULL, "\r\n" );	  
		if( !pNextToken )
		{	  
			continue;
		}
		strncpy( ( char * )( pcur->sDirtyStr ), ( const char * )pNextToken, sizeof( pcur->sDirtyStr ) ); 
		pcur->sDirtyStr[sizeof(pcur->sDirtyStr)-1] = 0;			  
		iCount++;				 
#ifdef _DEBUG_

		//printf("Level : %d, Dirty String : \"%s\"\n", pcur->iLevel, pcur->sDirtyStr);

#endif

	}
	// 添加空格到屏蔽字中
	pcur = &pstDirtyDB->stDirtyEngItems[iCount];	
	memset( pcur, 0, sizeof( Dirty_Chn_Item ) ); 
	pcur->iLevel = 2;		// 等级设置为2	
	strncpy( ( char * )( pcur->sDirtyStr ), " ", sizeof( char ) ); 
	pcur->sDirtyStr[sizeof(pcur->sDirtyStr)-1] = 0;
	iCount++;	
				   
	fclose( fpEngFile );
	pstDirtyDB->iDirtyEngItemNum = iCount;
	return 0;
}

CDirtyWord::CDirtyWord( )
{
	memset( DirtyGbTable, 0, sizeof( DirtyGbTable ) );	 
	memset( DirtyEngTable, 0, sizeof( DirtyEngTable ) );
}

CDirtyWord::~CDirtyWord()
{

}  
int CDirtyWord::GetByteOfUTF8( const char *pStr )
{	
	if ( pStr == NULL )
	{
		return 0;
	}
	unsigned char nFirByte = 	( unsigned char )*pStr;
	int nLength = 0;
	unsigned char nUnicode = UTF8_CODE; 
	while( nFirByte & nUnicode )
	{
		++nLength;	
		nUnicode = nUnicode >> 1;
	}
	return nLength;
}	
