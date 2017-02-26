// ********************************************************************** //
// Main.cpp
// 这个文件也不用说了,一看就知道了
//
// Author: Black
// ********************************************************************** //
#include "stdafx.hpp"
#include "scenecfg_manager.h"
#include "clienthandle.h"
#include "version.h"
#include "servercore.h"
#include "scenelogic.h"
#include "Gp_Load.h"
#include "sys/stat.h"
#include "sys/types.h"

char g_szRunPath[256];
int g_nFtokParam;

void ignore_pipe()
{
	struct sigaction sig;

	sig.sa_handler = SIG_IGN;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGPIPE,&sig,NULL);
}


int init_daemon(void)
{
	int lock_fd;
	lock_fd = open("./scenelock.lock", O_RDWR|O_CREAT, 0640);
	if(lock_fd < 0 )
	{
		printf("Open Lock File Failed, Scene Server Init Failed!\n");
		exit( 0 );
	}
	int ret;
	ret = flock(lock_fd, LOCK_EX | LOCK_NB);
	if(ret < 0 )
	{
		printf("Lock File Failed, Scene Server is already Running!\n");
		exit ( 0 );
	}

	pid_t pid;

	if ((pid = fork()) != 0)
	{
		exit(0);
	}
	setsid();

	signal(SIGINT,  SIG_IGN);
	signal(SIGHUP,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGHUP,  SIG_IGN);
	ignore_pipe();

	if ((pid = fork()) != 0)
	{
		exit(0);
	}

	// 改变运行路径
	if( chdir( (const char *)g_szRunPath) )
	{
		printf( "Can't change run dir to %s after init daemon(%s), exit.\n", g_szRunPath, strerror(errno));
		exit(2);
	}
	else
	{
		LOG_NOTICE( "default", "daemon change run dir to %s", g_szRunPath );
	}

	umask(0);

	return 0;

} 

void sigusr1_handle( int iSigVal )
{
	CSceneServer::GetSingletonPtr()->SetRunFlag( CSceneServer::EFLG_CTRL_RELOAD );
	signal(SIGUSR1, sigusr1_handle);
}

void sigusr2_handle( int iSigVal )
{
	CSceneServer::GetSingletonPtr()->SetRunFlag( CSceneServer::EFLG_CTRL_QUIT );
	signal(SIGUSR2, sigusr2_handle);
}

void siguser40_handle( int iSigVal )
{
	CSceneServer::GetSingletonPtr()->SetRunFlag( CSceneServer::EFLG_CTRL_COREDUMP );
	signal(SIGUSR2, sigusr2_handle);
}


void printHint(const char*)
{
	printf("No Help.\n");
}

int Initailize( int vInitFlag )
{
	char szCmd[ FILE_MAX_PATH ] = {0};

	snprintf(szCmd, sizeof(szCmd)-1,"touch %s", "./scenepipefile");
	system(szCmd);

	unsigned int tkeyCore = MakeKey( "./scenepipefile", 'C' );

	// 返回-1为失败
	if( tkeyCore == (unsigned int)(-1)  )
	{
		printf( "make shm key failed. scenepipefile key %d, strerror %s  \n", tkeyCore,  strerror(errno ) );
		LOG_ERROR( "default", "make shm key failed. scenepipefile key %d, strerror %s", tkeyCore, strerror(errno ) );	
		exit(-1);
	}

	LOG_NOTICE("default", "scenepipefile %08X", tkeyCore );

	unsigned int tSize = CSceneServer::CountSize() + sizeof( CSharedMem );
	BYTE* tpCoreShm = CreateShareMem ( tkeyCore, tSize );
	
	if( tpCoreShm == NULL )
	{
		printf( "Create ShareMem of Key %x Size %d failed, so exit ", tkeyCore, tSize );
		exit(1);
	}

	LOG_INFO( "default", "Core Shm Size is %u ", tSize );
	
	CSharedMem::pbCurrentShm = tpCoreShm;
	CSceneServer::mShmPtr = new CSharedMem( tkeyCore, tSize, vInitFlag );

	if( CSceneServer::mShmPtr == NULL )
	{
		printf( "Create Shm of Sceneserver failed" );
		exit( 1 );
	}

	if ( CSceneServer::mShmPtr->GetInitMode( ) == SHM_INIT )
	{
		CObj::msCreateMode = 1;
		printf( "gameserver start at init mode...\n" );
	} 
	else
	{
		CObj::msCreateMode = 0;
		printf( "gameserver start at resume mode...\n" );
	}

	return 0;
}



int main( int argc, char* argv[] )
{
	int tnDaemon = 0;
	int tInitFlag = 1;
	int tSave = 0;

	//int tFtokParam = 0;
	if( argc > 1 )
	{
		//支持版本信息查询
		if( !strcasecmp(argv[1], "-v") )
		{
#ifdef _DEBUG_
	#ifdef _FB_
			printf("xyj fbgameserver debug %s  build at %s %s\n", _GAME_VER, __DATE__, __TIME__);
	#else
			printf("xyj gameserver debug %s  build at %s %s\n", _GAME_VER, __DATE__, __TIME__);
	#endif // _FB_
#else
	#ifdef _FB_
			printf("xyj fbgameserver release %s  build at %s %s\n", _GAME_VER, __DATE__, __TIME__);
	#else
			printf("xyj gameserver release %s  build at %s %s\n", _GAME_VER, __DATE__, __TIME__);
	#endif // _FB_
#endif // _DEBUG_
			exit(0);
		}

		for(int i = 1; i < argc; i++ )
		{
			if( strcasecmp((const char *)argv[i], "Init") == 0 )
			{
				tInitFlag = 1;
			}
			else if( strcasecmp((const char *)argv[i], "Resume") == 0  )
			{
				tInitFlag = 0;
			}
			else if( strcasecmp((const char *)argv[i], "-D") == 0 )
			{
				tnDaemon = 1;
			}
			else if( strcasecmp( (const char* )argv[ i ], "Save" ) == 0 )
			{
				tInitFlag = 0;
				tSave = 1;
			}
			else if( strncmp( (const char* )argv[ i ], "-k", 2 ) == 0 || strncmp( (const char* )argv[ i ], "-K", 2 ) == 0 )
			{
				char strParam[100] = {0};
				strncpy( strParam, &argv[i][2], 99 );
				g_nFtokParam = ::atoi(strParam);
			}
		}
	}

	// 创建LOG 目录
	mkdir( "../log", 0777 );

	INIT_ROLLINGFILE_LOG( "default", "../log/sceneserver.log", LEVEL_DEBUG );

	// 初始化运行路径
	getcwd( g_szRunPath, sizeof( g_szRunPath ) ); 	
	if( chdir( (const char *)g_szRunPath) )
	{
		printf( "Can't change run dir to %s after init daemon(%s), exit.\n", g_szRunPath, strerror(errno));
		exit(2);
	}
	else
	{
		LOG_NOTICE( "default", "change run dir to %s", g_szRunPath );
	}

	//守护模式
	if (tnDaemon)
	{
		init_daemon( );
	}

	#ifdef _GPCHECK_	
	int nGpCheck = GpCheckInit();
	if ( nGpCheck != 0 )
	{
		LOG_ERROR("default", "Initialize GP failed %d", nGpCheck);
		GpCheckEnd();

		return -1;
	}

	nGpCheck = GpCheckFirst();
	if ( nGpCheck != 0 )
	{
		LOG_ERROR("default", "GP first check failed %d", nGpCheck);
		GpCheckEnd();

		return -1;
	}
	#endif

	Initailize( tInitFlag );

	CSceneServer* tpSenceServer = new CSceneServer;

	tpSenceServer->Initailize( tInitFlag );	

	#ifdef _GPCHECK_	
	nGpCheck = GpCheckSecond();
	if ( nGpCheck != 0 )
	{
		LOG_ERROR("default", "GP second check failed %d", nGpCheck);
		GpCheckEnd();

		return -1;
	}
	#endif


	tpSenceServer->PrepareToRun();

	signal(SIGUSR1, sigusr1_handle);
	signal(SIGUSR2, sigusr2_handle);
	signal(40,		siguser40_handle );
	

	if( tSave == 1 )
	{
		CSceneServer::GetSingletonPtr( )->SaveAllPlayer( SAVE_FILE ); 
	}

	// 启动反外挂服务
	StartApexProxy();

	#ifdef _GPCHECK_	
	nGpCheck = GpCheckThird();
	if ( nGpCheck != 0 )
	{
		LOG_ERROR("default", "GP third check failed %d", nGpCheck);
		GpCheckEnd();
	}

	GpCheckEnd();
	#endif


	tpSenceServer->Run();

	// 发送到网关关闭回应 
	CSceneServer::GetSingletonPtr()->SendShutdownAck();

	// 放到了安全退出里面
	// 停止反外挂服务
	StopApexProxy();

	// 释放逻辑内存
	CSceneLogic::GetSingletonPtr()->Shutdown( );

	// 释放protobuf中lib和msg占用的内存
	::google::protobuf::ShutdownProtobufLibrary();

	LOG_SHUTDOWN_ALL;

	return 0;
}


