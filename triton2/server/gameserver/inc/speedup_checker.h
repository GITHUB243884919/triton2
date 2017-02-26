
/////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////

class CSpeedChecker
{
private:
	unsigned long long	mullServerTimeBaseLine;
	unsigned long long	mullClientTimeBaseLine;
	unsigned long long  mullLastClientTime;
	unsigned long long  mullLastServerTime;
	long long			mllLastDiff;
    
    int                 miLastSpeed;
    unsigned int        muiSpeedFailedCounts;

	unsigned int        muiCheckCountsForCSTimeDiff;
	unsigned int        muiFailedCountsForCSTimeDiff;
	unsigned int        muiCheckCountsClientSpeed;
    unsigned int        muiFailedCountsForClientSpeed;
public:
    CSpeedChecker();
    ~CSpeedChecker();

public:
    bool CheckCSTimeDiff( int iPlayerEntityId, 
                          int iPlayerCharId, 
                          std::string strPlayerCharName, 
                          unsigned long long ullClientTimeNow );
    bool CheckClientSpeed( int iPlayerEntityId, 
                           int iPlayerCharId, 
                           std::string strPlayerCharName, 
                           unsigned long long ullClientTimeNow, 
                           int iSpeed );
	std::string DebugInfoForCSTimeDiff( void );
	std::string DebugInfoForClientSpeed( unsigned long long ullClientTimeNow );
	inline void ClearFailedCount()
	{
		muiCheckCountsForCSTimeDiff = 0;
		muiFailedCountsForCSTimeDiff = 0;
		muiCheckCountsClientSpeed = 0;
		muiFailedCountsForClientSpeed = 0;
	}
    static void ShowConfig( void );
};

/////////////////////////////////////////////////////////////////////////////////////////
// End of file
/////////////////////////////////////////////////////////////////////////////////////////
