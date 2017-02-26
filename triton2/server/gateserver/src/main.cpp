// ********************************************************************** //
// Main.cpp
// 这个文件不用说了, 一看就知道
// ********************************************************************** //

#include "westserver.h"
#include "gatelogic.h"
#include "version.h"
#ifdef WIN32
#include "win32network.h"
#elif LINUX
#include "linuxnetwork.h"
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include "sharemem.h"
#include "repetionmodule.h"
#include "sys/stat.h"
#include "sys/types.h"
#endif

using namespace GateServer;
#ifdef LINUX
void ignore_pipe()
{
	struct sigaction sig;

	sig.sa_handler = SIG_IGN;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGPIPE,&sig,NULL);
}

void init_daemon(void)
{
	int lock_fd;
	lock_fd = open("./gatelock.lock", O_RDWR|O_CREAT, 0640);
	if(lock_fd < 0 )
	{
		printf("Open Lock File Failed, Gate Server Init Failed!\n");
		exit( 0 );
	}
	int ret;
	ret = flock(lock_fd, LOCK_EX | LOCK_NB);
	if(ret < 0 )
	{
		printf("Lock File Failed, Gate Server is already Running!\n");
		exit ( 0 );
	}

	pid_t pid;

	if ((pid = fork()) != 0) exit(0);

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
		exit(0);                                                                                                  

	char tPath[ 256 ];
	//snprintf( tPath, sizeof( tPath ) - 1 , "%s/server/gateserver/bin", getenv("TRITON_HOME") );
	getcwd( tPath, sizeof( tPath ) );
	if( chdir((const char *)tPath) )
	{
		printf( "Can't change run dir to %s after init daemon(%s), exit.\n", tPath, strerror(errno));
		exit(2);
	}

	umask(0);
}

void sigusr1_handle( int iSigVal )
{
	CGateLogic::GetSingletonPtr()->SetRunFlag( EFLG_CTRL_RELOAD );
	signal(SIGUSR1, sigusr1_handle);
}

void sigusr2_handle( int iSigVal )
{
	CGateLogic::GetSingletonPtr()->SetRunFlag( EFLG_CTRL_QUIT );
	signal(SIGUSR2, sigusr2_handle);
}

void siguser40_handle( int iSigVal )
{
	CGateLogic::GetSingletonPtr()->SetRunFlag( EFLG_CTRL_COREDUMP );
	signal( 40, siguser40_handle);
}

void siguser41_handle( int iSigVal )
{
	CGateLogic::GetSingletonPtr()->SetRunFlag( EFLG_CTRL_REFRESHRANK );
	signal( 41, siguser41_handle);
}

void siguser42_handle( int iSigVal )
{
	CGateLogic::GetSingletonPtr()->SetRunFlag( EFLG_CTRL_PROCESSEXPIREDMAIL );
	signal( 42, siguser42_handle );
}

void siguser43_handle( int iSigVal )
{
	CGateLogic::GetSingletonPtr()->SetRunFlag( EFLG_CTRL_SAFESHUTDOWN );
	signal( 43, siguser43_handle );
}


int Initailize( int vInitFlag )
{
	char szCmd[ FILE_MAX_PATH ] = {0};
	snprintf(szCmd, sizeof(szCmd)-1,"touch %s", "./gatepipefile");
	system(szCmd);

	unsigned int tkeyCore = MakeKey( "./gatepipefile", 'C' );
	unsigned int tSize = CGateLogic::CountSize() + sizeof( CSharedMem );
	BYTE* tpCoreShm = CreateShareMem ( tkeyCore, tSize );

	LK_ASSERT( ( tpCoreShm != NULL ), exit(1) );

	LOG_INFO( "gateserver", "Core Shm Size is %u ", tSize );

	CSharedMem::pbCurrentShm = tpCoreShm;
	CGateLogic::mShmPtr = new CSharedMem( tkeyCore, tSize, vInitFlag );
	LK_ASSERT( CGateLogic::mShmPtr != NULL, exit( 1 ) );

	if ( CGateLogic::mShmPtr->GetInitMode( ) == SHM_INIT )
	{
		CObj::msCreateMode = 1;
	} 
	else
	{
		CObj::msCreateMode = 0;
	}

	return 0;
}

#endif



void printHint(const char*)
{
	printf("No Help.\n");
}

int main( int argc, char* argv[] )
{
	int tnDaemon = 0;
	int tInitFlag = 1;

	if( argc > 1 )
	{
		if( !strcasecmp(argv[1], "-v") )
		{
#ifdef _DEBUG_
			printf("xyj gateserver debug %s  build at %s %s\n", _GATE_VER, __DATE__, __TIME__);
#else
			printf("xyj gateserver release %s  build at %s %s\n", _GATE_VER, __DATE__, __TIME__);
#endif
			exit(0);
		}
	}

	for(int i = 1; i < argc; ++i)
	{
		if( strcasecmp((const char *)argv[i], "-I") == 0 )
		{
			tInitFlag = 1;
		}

		if( strcasecmp((const char *)argv[i], "-R") == 0 )
		{
			tInitFlag = 0;
		}

		if( strcasecmp((const char *)argv[i], "-D") == 0 )
		{
			tnDaemon = 1;
		}
	}
	// 创建log目录
	mkdir( "../log", 0777 );

	// 默认先初始化一个日志，后面根据参数修改
	INIT_ROLLINGFILE_LOG( "default", "../log/gateserver.log", (LogLevel)LEVEL_DEBUG, 10000000, 10 );

	//守护模式
	if (tnDaemon) init_daemon();

	Initailize( tInitFlag );

	//创建GlobalServer
	CGateLogic* tpGateLogic= new CGateLogic;
	tpGateLogic->Initialize( tInitFlag );
	tpGateLogic->Launch();

	signal(SIGUSR1, sigusr1_handle);
	signal(SIGUSR2, sigusr2_handle);
	signal( 40, siguser40_handle);
	signal( 41, siguser41_handle);
	signal( 42, siguser42_handle );
	signal( 43, siguser43_handle );

	tpGateLogic->Run( tnDaemon);

	delete tpGateLogic;

	LOG_SHUTDOWN_ALL;

	return 0;
}
