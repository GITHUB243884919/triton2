#include "dbctrl.h"
#include "base_db.h"
#include "sharemem.h"
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "sys/stat.h"
#include "sys/types.h"

int Initialize(int iInitFlag = 0);



void sigusr1_handle( int iSigVal )
{
	CDBCtrl::GetSingletonPtr()->SetRunFlag( CDBCtrl::EFLG_CTRL_RELOAD );
    signal(SIGUSR1, sigusr1_handle);
}

void sigusr2_handle( int iSigVal )
{
	CDBCtrl::GetSingletonPtr()->SetRunFlag( CDBCtrl::EFLG_CTRL_QUIT );
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

    lock_fd = open("./dblock.lock", O_RDWR|O_CREAT, 0640);
    if(lock_fd < 0 )
    {
        printf("Open Lock File Failed, DB Server Init Failed!\n");
        return -1;
    }

    ret = flock(lock_fd, LOCK_EX | LOCK_NB);
    if(ret < 0 )
    {
        printf("Lock File Failed, DB Server is already Running!\n");
        return -1;
    }
 
    int i;
    pid_t pid;

    if ((pid = fork()) != 0)
        exit(0);

	char tPath[ 256 ];
	//snprintf( tPath, sizeof( tPath ) - 1 , "%s/server/dbserver/bin", getenv("TRITON_HOME") );

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
	snprintf(szCmd, sizeof(szCmd)-1,"touch %s", "./dbpipefile");
	system(szCmd);

	unsigned int tkeydb = MakeKey( "./dbpipefile", 'D' );
	unsigned int tSize = CDBCtrl::CountSize() + sizeof( CSharedMem );
	BYTE* tpDBShm = CreateShareMem ( tkeydb, tSize );

	LK_ASSERT( ( tpDBShm != NULL ), return -1 );

	LOG_DEBUG( "dbctrl", "DB Shm Size is %u ", tSize );

	CSharedMem::pbCurrentShm = tpDBShm;
	CDBCtrl::mShmPtr = new CSharedMem( tkeydb, tSize, iInitFlag );
	LK_ASSERT( CDBCtrl::mShmPtr != NULL, return -2 );

	if ( CDBCtrl::mShmPtr->GetInitMode( ) == SHM_INIT )
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


	// 创建LOG 目录
	mkdir( "../log", 0777 );

  //  INIT_ROLLINGFILE_LOG( "dbctrl", "../log/dbsvrd.log", LEVEL_DEBUG );
  //  INIT_ROLLINGFILE_LOG( "default", "../log/dbsvrd.log", LEVEL_DEBUG );

	if( argc > 1 )
	{
		//支持版本信息查询
		if( !strcasecmp(argv[1], "-v") )
		{
#ifdef _DEBUG_
			printf("xyj db server %s debug build at %s %s\n", "V1.0" , __DATE__, __TIME__);
#else
			printf("xyj db server %s release build at %s %s\n", "V1.0" , __DATE__, __TIME__);
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

	int nResult = Initialize(iInitFlag);
    if( nResult < 0 )
    {
		LOG_ERROR( "default", "dbserver initialize failed error=%d", nResult );
		printf( "Error: dbserver initialize failed error=%d", nResult );
        exit(-1);
    }

	CDBCtrl* tpDBCtrl = new CDBCtrl;
	if( tpDBCtrl->Initialize () )
	{
		LOG_ERROR( "default", "dbctrl initialize failed" );
		printf( "Error: dbctrl initialize failed" );
		exit(-1);
	}

    if( tpDBCtrl->PrepareToRun() )
    {
		LOG_ERROR( "default", "dbctrl prepare failed" );
		printf( "Error: dbctrl prepare failed" );
        exit(-1);
	}

	signal(SIGUSR1, sigusr1_handle);
	signal(SIGUSR2, sigusr2_handle);

	printf( "\n" );
	printf("------------------------------------\n");
	printf("|  xyj-dbserver startup success!   |\n");
	printf("------------------------------------\n");
	LOG_NOTICE( "default", "xyj-dbserver startup success!" );

	tpDBCtrl->Run();

    return 0;
}

