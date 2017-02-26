#ifndef __GP_LOAD_H__
#define __GP_LOAD_H__

//////////////////////////////

typedef int _FUNC_GP_CHECK();
typedef int _FUNC_GP_CHECK_AND_KEY(unsigned char *ucGameKey);

#ifdef __cplusplus
extern "C" {
#endif


	extern "C"  int GpCheckInit();
	extern "C"  int GpCheckAndKeyFirst(unsigned char *ucGameKey);
	extern "C"  int GpCheckFirst();
	extern "C"  int GpCheckSecond();
	extern "C"  int GpCheckThird();
	extern "C"  void GpCheckEnd();

#ifdef __cplusplus
}
#endif


#endif

