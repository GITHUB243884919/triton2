#include "logctrl.h"
#include "base_log.h"
#include "sharemem.h"
#include <unistd.h>
#include "stdafx.hpp"
#include "sys/stat.h"
#include "sys/types.h"

int Initialize(int iInitFlag = 0);



void sigusr1_handle( int iSigVal )
{
	CLogCtrl::GetSingletonPtr()->SetRunFlag( CLogCtrl::EFLG_CTRL_RELOAD );
    signal(SIGUSR1, sigusr1_handle);
}

void sigusr2_handle( int iSigVal )
{
	CLogCtrl::GetSingletonPtr()->SetRunFlag( CLogCtrl::EFLG_CTRL_QUIT );
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

//初始化为守护进程的函数
int InitDaemon(void)
{
    int lock_fd;
    int ret;

    lock_fd = open("./loglock.lock", O_RDWR|O_CREAT, 0640);
    if(lock_fd < 0 )
    {
        printf("Open Lock File Failed, Log Server Init Failed!\n");
        return -1;
    }

    ret = flock(lock_fd, LOCK_EX | LOCK_NB);
    if(ret < 0 )
    {
        printf("Lock File Failed, Log Server is already Running!\n");
        return -1;
    }
 
    int i;
    pid_t pid;

    if ((pid = fork()) != 0)
        exit(0);

	char tPath[ 256 ];
	//snprintf( tPath, sizeof( tPath ) - 1 , "%s/server/logserver/bin", getenv("TRITON_HOME") );

	getcwd( tPath, sizeof( tPath ) ); 	

	if( chdir((const char *)tPath) )
	{
		printf( "Can't change run dir to %s after init daemon(%s), exit.\n", tPath, strerror(errno));
		exit(2);
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
        exit(0);

    umask(0);
    return 0;
}
//

int Initialize(int iInitFlag )
{
	char szCmd[ 128 ] = {0};
	snprintf(szCmd, sizeof(szCmd)-1,"touch %s", "./logpipefile");
	system(szCmd);

	unsigned int tkeylog = MakeKey( "./logpipefile", 'D' );
	unsigned int tSize = CLogCtrl::CountSize() + sizeof( CSharedMem );
	BYTE* tpLogShm = CreateShareMem ( tkeylog, tSize );

	LK_ASSERT( ( tpLogShm != NULL ), exit(1) );

	LOG_DEBUG( "logctrl", "Log Shm Size is %u ", tSize );

	CSharedMem::pbCurrentShm = tpLogShm;
	CLogCtrl::mShmPtr = new CSharedMem( tkeylog, tSize, iInitFlag );
	LK_ASSERT( CLogCtrl::mShmPtr != NULL, exit( 1 ) );

	if ( CLogCtrl::mShmPtr->GetInitMode( ) == SHM_INIT )
	{
		CObj::msCreateMode = 1;
	} 
	else
	{
		CObj::msCreateMode = 0;
	}
    return 0;
}

int main(int argc, char **argv)
{
    int iInitFlag = 1;
	int nDaemon = 0;

	// 创建LOG目录
	mkdir( "../log", 0777 );
	mkdir( "../statistic", 0777 );

  //  INIT_ROLLINGFILE_LOG( "logctrl", "../log/logsvrd.log", LEVEL_DEBUG );
  //  INIT_ROLLINGFILE_LOG( "default", "../log/logsvrd.log", LEVEL_DEBUG );

	if( argc > 1 )
	{
		//支持版本信息查询
		if( !strcasecmp(argv[1], "-v") )
		{
#ifdef _DEBUG_
			printf("xyj log Server debug V1.0 build at %s %s\n", __DATE__, __TIME__);
#else
			printf("xyj log Server release V1.0 build at %s %s\n", __DATE__, __TIME__);
#endif
			exit(0);
		}

		if( strcasecmp((const char *)argv[1], "-D") == 0 )
		{
			nDaemon = 1;
		}

	}
    //初始化守护进程
    if( nDaemon == 1 )
    {
		InitDaemon();
    }

    if( Initialize(iInitFlag) )
    {
        exit(-1);
    }

	CLogCtrl* tpLogCtrl = new CLogCtrl;
	if( tpLogCtrl->Initialize () )
	{
		exit(-1);
	}

    if( tpLogCtrl->PrepareToRun() )
    {
        exit(-1);
	}

	signal(SIGUSR1, sigusr1_handle);
	signal(SIGUSR2, sigusr2_handle);

	printf("\n");
	printf("-----------------------------------\n");
	printf("|  xyj-logserver startup success! |\n");
	printf("-----------------------------------\n");

	LOG_NOTICE("default", " xyj-logserver startup success!" );

	tpLogCtrl->Run();

    return 0;
}

