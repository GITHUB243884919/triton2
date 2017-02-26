#include "proxy_ctrl.h"
#include "proxy_comm.h"
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "sys/stat.h"
#include "sys/types.h"

CFlag g_byRunFlag;
char   g_szRunPath[256];						//运行目录
CProxyCtrl g_stProxyCtrl;

void sigusr1_handle( int iSigVal )
{
	g_byRunFlag.SetFlag( FLG_CTRL_RELOAD );
	signal(SIGUSR1, sigusr1_handle);
}

void sigusr2_handle( int iSigVal )
{
	g_byRunFlag.SetFlag( FLG_CTRL_QUIT );
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
void InitDaemon(void)
{
	int i;
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
	mkdir( "../statistic", 0777 );

	//支持版本信息查询
	if( argc >1 && !strcasecmp(argv[1], "-v") )
	{
#ifdef _DEBUG_
		printf("Game Proxy Server debug V1.0 build in %s %s\n", __DATE__, __TIME__);
#else
		printf("Game Proxy Server release V1.0 build in %s %s\n", __DATE__, __TIME__);
#endif
		exit(0);
	}

	//初始化运行路径
	//snprintf(g_szRunPath, sizeof(g_szRunPath)-1, "%s/server/proxyserver/bin", getenv("TRITON_HOME"));
	getcwd( g_szRunPath, sizeof( g_szRunPath ) );
	if( chdir((const char *)g_szRunPath) )
	{
		printf("Can't change run dir to %s, exit.\n", g_szRunPath);
		exit(1);
	}

	INIT_ROLLINGFILE_LOG( "default", "../log/proxyd.log", LEVEL_DEBUG);

	//初始化为守护进程
	InitDaemon();

	if( g_stProxyCtrl.Initialize("../config/proxysvrd.xml") )
	{
		LOG_ERROR("default", "Proxy control init failed.\n");
		exit(1);
	}

	if( g_stProxyCtrl.PrepareToRun() )
	{
		LOG_ERROR("default", "Proxy control prepare to run failed.\n");
		exit(2);
	}

	//安装信号处理函数
	signal(SIGUSR1, sigusr1_handle);
	signal(SIGUSR2, sigusr2_handle);

	LOG_INFO("default", "xyj-proxyserver startup success!" );

	printf( "xyj-proxyserver startup success!\n");

	g_stProxyCtrl.Run();

	return 0;
}
