
#include <stdlib.h>
//#include <fstream.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>

#include "tcpdef.h"
#include "config.h"
#include "tcpctrl.h"
#include "shm.h"
#include "code_queue.h"
#include "log.h"


int Initialize(int iInitMode);
int InitDaemon(int iInitMode);
void sigusr1_handle( int iSigVal );
void sigusr2_handle( int iSigVal );

CTCPCtrl *g_pTCPCtrl;
CConfigFile *g_pConfig;
int g_nFtokParam;
char g_szRunPath[256];

int main( int argc, char **argv )
{
    int iTempRet;
    int iInitMode = 0;

    //支持版本信息查询
    if(1 < argc && !strcasecmp(argv[1], "-v" ) )
    {
        char szTemp1[100] = {0};
        char szTemp2[100] = {0};
        char szTemp3[100] = {0};
        snprintf(szTemp1, sizeof(szTemp1)-1, "Epoll TCP");

#ifdef _DEBUG_
        snprintf(szTemp3, sizeof(szTemp3)-1, "debug");
#else
        snprintf(szTemp3, sizeof(szTemp3)-1, "release");
#endif

        printf("Tcp %s(support %d fd) Server %s %s V1.0 build in %s %s\n", 
                szTemp1, MAX_SOCKET_NUM,  szTemp3, szTemp2, __DATE__, __TIME__);

        exit(0);
    }

#ifdef _DEBUG_
	INIT_ROLLINGFILE_LOG( "default", "../log/tcpsvrd.log", LEVEL_DEBUG );
#else
	INIT_ROLLINGFILE_LOG( "default", "../log/tcpsvrd.log", LEVEL_NOTICE );
#endif

    if(1 < argc && !strcasecmp(argv[1], "d" ) )
    {
        iInitMode = 0;
    }
    else
    {
        iInitMode = 1;
    }

	//int tFtokParam = 0;
	g_nFtokParam=0;
	if(1 < argc && ( strncmp(argv[1], "-k", 2 )==0 || strncmp(argv[1], "-K", 2 )==0 ) )
	{
		char strParam[100] = {0};
		strncpy( strParam, &argv[1][2], 99 );
		g_nFtokParam = ::atoi(strParam);
		//printf("-----%d-----", g_nFtokParam);
	}

	// 获得运行路径
	getcwd( g_szRunPath, sizeof( g_szRunPath )-1 ); 	
	LOG_NOTICE( "default", "TcpServer run at %s", g_szRunPath );

    iTempRet = Initialize(iInitMode);
    if(0 != iTempRet)
    {
        exit(0);
    }

    g_pTCPCtrl = CTCPCtrl::CreateTCPCtrl();
	if( g_pTCPCtrl == NULL )
	{
		exit(0);
	}

    iTempRet = g_pTCPCtrl->Initialize();
    if( 0 != iTempRet )
    {
		printf("init err!");
		exit(0);
    }

	if( 1 < argc )
	{
		g_pTCPCtrl->SetRunArgs( argv[1] );
	}

    g_pTCPCtrl->PrepareToRun();
	
	//printf( "xyj-tcpserver startup success!" );
	LOG_NOTICE( "default", "xyj-tcpserver %s startup success!", g_pTCPCtrl->GetRunArgs() );

    g_pTCPCtrl->Run();

    return 0;
}

int Initialize(int iInitMode)
{
    int iTempRet;

    iTempRet = InitDaemon(iInitMode);
    if( 0 != iTempRet )
    {
        exit(0);
    }

    signal(SIGUSR1, sigusr1_handle);
    signal(SIGUSR2, sigusr2_handle);

    return iTempRet;
}

void sigusr1_handle( int iSigVal )
{
    g_pTCPCtrl->SetRunFlag(reloadcfg);
    signal(SIGUSR1, sigusr1_handle);
}

void sigusr2_handle( int iSigVal )
{
    g_pTCPCtrl->SetRunFlag(tcpexit);
    signal(SIGUSR2, sigusr2_handle);
}



int InitDaemon( int iInitDaemon )
{
	char tPath[ 256 ];
	long tPathLen = sizeof( tPath ) - 1;
	getcwd( tPath, tPathLen );

    system("touch tcppipefile");
    int lock_fd;
    lock_fd = open("./tcplock.lock", O_RDWR|O_CREAT, 0640);
    if(lock_fd < 0 )
    {
        printf("Open Lock File Failed, Tcp Server Init Failed!\n");
        return -1;
    }
    int ret;
    ret = flock(lock_fd, LOCK_EX | LOCK_NB);
    if(ret < 0 )
    {
        printf("Lock File Failed, Tcp Server is already Running!\n");
        return -1;
    }

    if( SERVER_INIT_DAEMON != iInitDaemon )
    {
        /*如果不是以后台方式运行，返回成功*/
        return 0;
    }

    int i;
    pid_t pid;

    if ((pid = fork() ) != 0 )
    {
        exit( 0);
    }

    setsid();

    signal( SIGINT,  SIG_IGN);
    signal( SIGHUP,  SIG_IGN);
    signal( SIGQUIT, SIG_IGN);
    signal( SIGPIPE, SIG_IGN);
    signal( SIGTTOU, SIG_IGN);
    signal( SIGTTIN, SIG_IGN);
    signal( SIGCHLD, SIG_IGN);
    signal( SIGTERM, SIG_IGN);

    struct sigaction sig;

    sig.sa_handler = SIG_IGN;
    sig.sa_flags = 0;
    sigemptyset( &sig.sa_mask);
    sigaction( SIGHUP,&sig,NULL);

    if ((pid = fork() ) != 0 )
    {
        exit( 0);
    }

	if( chdir((const char *)tPath) )
	{
		printf( "Can't change run dir to %s after init daemon(%s), exit.\n", tPath, strerror(errno));
		exit(2);
	}

    umask( 0);
    setpgrp();

    return 0;
}
