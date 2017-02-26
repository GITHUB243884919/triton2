#include "sharemem.h"

#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <errno.h>
#include "log.h"
#include <unistd.h>
#include "base.h"



unsigned int MakeKey( const char* pFile, int vId )
{
	return MakeShmKey( pFile, vId );
}

BYTE* CreateShareMem( unsigned int vKey, int vSize )
{

	key_t iKey = ( key_t ) vKey;
	size_t iTempShmSize;
	int iShmID;

	if( iKey < 0 )
	{
		exit(-1);
	}

	iTempShmSize = ( size_t ) vSize;
	//iTempShmSize += sizeof(CSharedMem);

	LOG_DEBUG( "default", "Try to malloc share memory of %d bytes...", iTempShmSize);

	iShmID = shmget( iKey, iTempShmSize, IPC_CREAT|IPC_EXCL|0666 );

	if( iShmID < 0 )
	{
		if( errno != EEXIST )
		{
			LOG_DEBUG( "default", "Alloc share memory failed, %s", strerror(errno));
			exit(-1);
		}

		LOG_DEBUG( "default", "Same shm seg (key=%08X) exist, now try to attach it...", iKey);

		iShmID = shmget( iKey, iTempShmSize, 0666 );
		if( iShmID < 0 )
		{
			LOG_DEBUG( "default", "Attach to share memory %d failed, %s. Now try to touch it", iShmID, strerror(errno));
			iShmID = shmget( iKey, 0, 0666 );
			if( iShmID < 0 )
			{
				LOG_DEBUG( "default", "Fatel error, touch to shm failed, %s.", strerror(errno));
				exit(-1);
			}
			else
			{
				LOG_DEBUG( "default", "First remove the exist share memory %d", iShmID);
				if( shmctl(iShmID, IPC_RMID, NULL) )
				{
					LOG_DEBUG( "default", "Remove share memory failed, %s", strerror(errno));
					exit(-1);
				}
				iShmID = shmget( iKey, iTempShmSize, IPC_CREAT|IPC_EXCL|0666 );
				if( iShmID < 0 )
				{
					LOG_DEBUG( "default", "Fatal error, alloc share memory failed, %s", strerror(errno));
					exit(-1);
				}
			}
		}
		else
		{
			LOG_DEBUG( "default", "Attach to share memory succeed.");
		}
	}



	LOG_DEBUG( "default", "Successfully alloced share memory block, key = %08X, id = %d, size = %d", iKey, iShmID, iTempShmSize);

	BYTE* tpShm = (BYTE *)shmat(iShmID, NULL, 0);

	if( !tpShm )
	{
		return NULL;
	}

	return tpShm;

}



int DestroyShareMem( unsigned int vKey )
{
	key_t iKey = (key_t)vKey;
	int iShmID;

	if( iKey < 0 )
	{
		LOG_DEBUG( "default", "Error in ftok, %s.", strerror(errno));
		return -1;
	}

	LOG_DEBUG( "default", "Touch to share memory key = 0X%08X...", iKey);

	iShmID = shmget( iKey, 0, 0666 );
	if( iShmID < 0 )
	{
		LOG_DEBUG( "default", "Error, touch to shm failed, %s", strerror(errno));
		return -1;
	}
	else
	{
		LOG_DEBUG( "default", "Now remove the exist share memory %d", iShmID);
		if( shmctl(iShmID, IPC_RMID, NULL) )
		{
			LOG_DEBUG( "default", "Remove share memory failed, %s", strerror(errno));
			return -1;
		}
		LOG_DEBUG( "default", "Remove shared memory(id = %d, key = 0X%08X) succeed.", iShmID, iKey);
	}

	return 0;
}
