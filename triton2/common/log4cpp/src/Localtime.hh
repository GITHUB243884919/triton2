#ifndef _LOG4CPP_LOCALTIME_HH
#define _LOG4CPP_LOCALTIME_HH

#include <time.h>

namespace log4cpp
{
   void localtime(const ::time_t* time, ::tm* t);


//#if defined(_MSC_VER) && defined(LOG4CPP_HAVE_LOCALTIME_R)
//   void localtime(const ::time_t* time, ::tm* t)
//   {
//	   localtime_s(t, time);  
//   }
//#endif
//
//#if !defined(_MSC_VER) && defined(LOG4CPP_HAVE_LOCALTIME_R)
//   void localtime(const ::time_t* time, ::tm* t)
//   {
//	   localtime_r(time, t);
//   }
//#endif
//
//#if !defined(LOG4CPP_HAVE_LOCALTIME_R)
//   void localtime(const ::time_t* time, ::tm* t)
//   {
//	   ::tm* tmp = ::localtime(time);
//	   memcpy(t, tmp, sizeof(::tm));
//   }
//#endif
}

#endif
