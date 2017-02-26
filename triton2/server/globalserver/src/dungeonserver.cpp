// dungeonserver.cpp : Defines the entry point for the console application.
//



#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "sys/stat.h"
#include "sys/types.h"
#include "log.h"
#include "dungeonlogic.h"
#include "macro.h"
#include "sharemem.h"

char   g_szRunPath[256];						//运行目录
void sigusr1_handle( int iSigVal )
{
	CDungeonServer::GetSingleton().SetRunFlag( CDungeonServer::EFLG_CTRL_RELOAD );
	signal(SIGUSR1, sigusr1_handle);
}

void sigusr2_handle( int iSigVal )
{	
	CDungeonServer::GetSingleton().SetRunFlag( CDungeonServer::EFLG_CTRL_QUIT );
	signal(SIGUSR2, sigusr2_handle);
}

void ignore_pipe()
{
	struct sigaction sig;

	sig.sa_handler = SIG_IGN;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGPIPE,&sig,NULL);
}

int Initailize( int vInitFlag )
{
	char szCmd[ FILE_MAX_PATH ] = {0};

	snprintf(szCmd, sizeof(szCmd)-1,"touch %s", "./dungeonpipefile");
	system(szCmd);

	unsigned int tkeyCore = MakeKey( "./dungeonpipefile", 'C' );

	// 返回-1为失败
	if( tkeyCore == (unsigned int)(-1)  )
	{
		printf( "make shm key failed. dungeonpipefile key %d, strerror %s  \n", tkeyCore,  strerror(errno ) );
		LOG_ERROR( "default", "make shm key failed. dungeonpipefile key %d, strerror %s", tkeyCore, strerror(errno ) );	
		exit(-1);
	}

	LOG_NOTICE("default", "dungeonpipefile %08X", tkeyCore );

	unsigned int tSize = CDungeonServer::CountSize() + sizeof( CSharedMem );
	BYTE* tpCoreShm = CreateShareMem ( tkeyCore, tSize );

	if( tpCoreShm == NULL )
	{
		printf( "Create ShareMem of Key %x Size %d failed, so exit ", tkeyCore, tSize );
		exit(1);
	}

	LOG_INFO( "default", "Core Shm Size is %u ", tSize );

	CSharedMem::pbCurrentShm = tpCoreShm;
	CDungeonServer::m_pShareMem = new CSharedMem( tkeyCore, tSize, vInitFlag );

	if( CDungeonServer::m_pShareMem == NULL )
	{
		printf( "Create Shm of dungeonserver failed" );
		exit( 1 );
	}

	if ( CDungeonServer::m_pShareMem->GetInitMode( ) == SHM_INIT )
	{
		CObj::msCreateMode = 1;
		printf( "dungeonserver start at init mode...\n" );
	} 
	else
	{
		CObj::msCreateMode = 0;
		printf( "dungeonserver start at resume mode...\n" );
	}

	return 0;
}

//初始化为守护进程的函数
void InitDaemon(void)
{	
	int lock_fd = open("./dunserver.lock", O_RDWR|O_CREAT, 0640);
	if(lock_fd < 0 )
	{
		printf("Open Lock File Failed, dunserver  Init Failed!\n");
		exit( 0 );
	}

	int ret = flock(lock_fd, LOCK_EX | LOCK_NB);
	if(ret < 0 )
	{
		printf("Lock File Failed, dunserver is already Running!\n");
		exit ( 0 );
	}

	pid_t pid;

	if ((pid = fork()) != 0)
		exit(0);

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

	umask(0);	
}

int main(int argc, char **argv)
{
	// 创建LOG 目录
	mkdir( "../log", 0777 );	

	//支持版本信息查询
	if( argc >1 && !strcasecmp(argv[1], "-v") )
	{
#ifdef _DEBUG_
		printf("Dungeon Server debug V1.0 build in %s %s\n", __DATE__, __TIME__);
#else
		printf("Dungeon Server release V1.0 build in %s %s\n", __DATE__, __TIME__);
#endif
		exit(0);
	}

	int tInitFlag = 0;	
	if( argc > 1 && strcasecmp((const char *)argv[1], "-D") == 0 )
	{
		tInitFlag = 1;
	}

	//初始化运行路径	
	getcwd( g_szRunPath, sizeof( g_szRunPath ) );
	if( chdir((const char *)g_szRunPath) )
	{
		printf("Can't change run dir to %s, exit.\n", g_szRunPath);
		exit(1);
	}

	INIT_ROLLINGFILE_LOG( "default", "../log/dungeon.log", LEVEL_DEBUG);

	//初始化为守护进程
	if ( tInitFlag == 1 )
	{
		InitDaemon();
	}	

	// 初始化
	Initailize( SHM_INIT );
	CDungeonServer *tpServer = new CDungeonServer;
	
	if( tpServer->Initialize( ) )
	{
		LOG_ERROR("default", "dungeonserver init failed.\n");
		exit(1);
	}

	if( tpServer->PrepareToRun() )
	{
		LOG_ERROR("default", "dungeonserverl prepare to run failed.\n");
		exit(2);
	}

	//安装信号处理函数
	signal(SIGUSR1, sigusr1_handle);
	signal(SIGUSR2, sigusr2_handle);

	LOG_INFO("default", "xyj-dungeonserver startup success!" );

	printf( "xyj-dungeonserver startup success!\n");

	tpServer->Run();

	return 0;
}
