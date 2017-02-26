#include "Gp_Load.h"
#include <iostream>
#include <dlfcn.h>

static _FUNC_GP_CHECK_AND_KEY *GpChkAndKeyFirst = NULL;
static _FUNC_GP_CHECK *GpChkFirst = NULL;
static _FUNC_GP_CHECK *GpChkSecond = NULL;
static _FUNC_GP_CHECK *GpChkThird = NULL;



void  *ghGpClientLib = NULL;     

extern "C"  int   GpCheckInit()
{	

  	if (!ghGpClientLib)
  	{
 		ghGpClientLib = dlopen("./lib/libGP_Client.so",RTLD_NOW );
		char *dlError = dlerror();
		if( dlError ) 
		{
			std::cout<<dlError << std::endl;
 		}
		if (!ghGpClientLib)
		{
			return -1;
		}
		
		GpChkAndKeyFirst = (_FUNC_GP_CHECK_AND_KEY *) dlsym(ghGpClientLib, "GpCheckAndKeyFirst");
		dlError = dlerror();
		if( dlError )
		{
			std::cout<<dlError << std::endl;
		}

  		GpChkFirst = (_FUNC_GP_CHECK *) dlsym(ghGpClientLib, "GpCheckFirst");
  		dlError = dlerror();
		if( dlError ) 
		{
			std::cout<<dlError << std::endl;
 		}

 		GpChkSecond = (_FUNC_GP_CHECK *) dlsym(ghGpClientLib, "GpCheckSecond");
  		dlError = dlerror();
		if( dlError ) 
		{
			std::cout<<dlError << std::endl;
 		}

 		GpChkThird = (_FUNC_GP_CHECK *) dlsym(ghGpClientLib, "GpCheckThird");
  		dlError = dlerror();
		if( dlError ) 
		{
			std::cout<<dlError << std::endl;
 		}
	}

	if (!GpChkFirst || !GpChkSecond || !GpChkThird)
	{
 		if(!ghGpClientLib)
	 	{
	 		dlclose(ghGpClientLib);
	 	}

		ghGpClientLib=NULL;
		GpChkFirst = NULL;
		GpChkSecond = NULL;
		GpChkThird = NULL;
		return -2;
	}
 	return 0;
}

extern "C" void GpCheckEnd()
{ 
	if (NULL != ghGpClientLib)
	{
		dlclose(ghGpClientLib);
	}

	GpChkAndKeyFirst = NULL;
	GpChkFirst = NULL;
	GpChkSecond = NULL;
	GpChkThird = NULL;
}

extern "C"  int GpCheckAndKeyFirst(unsigned char *ucGameKey)
{
	int nRet = 0;
	nRet = GpChkAndKeyFirst(ucGameKey);
	if (0 != nRet)
	{
		return -3;
	}
	return 0;
}

extern "C"  int GpCheckFirst()
{
	int nRet = 0;
	nRet = GpChkFirst();
	if (0 != nRet)
	{
		return -3;
	}
	return 0;
}

extern "C"  int GpCheckSecond()
{
	int nRet = 0;
	nRet = GpChkSecond();
	if (0 != nRet)
	{
		return -3;
	}
	return 0;
}

extern "C"  int GpCheckThird()
{
	int nRet = 0;
	nRet = GpChkThird();
	if (0 != nRet)
	{
		return -3;
	}
	return 0;
}

/*
extern "C" void   CHSSetFunc(void * pFunc,int nFuncFlag)
{
	if(g_sh_setfunc)
		g_sh_setfunc(pFunc,nFuncFlag);
	return;
}*/
