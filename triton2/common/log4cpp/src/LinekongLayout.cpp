/*
* LinekongLayout.cpp
*
* Copyright 2000, LifeLine Networks BV (www.lifeline.nl). All rights reserved.
* Copyright 2000, Bastiaan Bakker. All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#include "PortabilityImpl.hh"
#include <log4cpp/LinekongLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/FactoryParams.hh>
#ifdef LOG4CPP_HAVE_SSTREAM
#include <sstream>
#endif


#ifndef LOG4CPP_HAVE_GETTIMEOFDAY
#	include <time.h>
#endif

namespace log4cpp {

	LinekongLayout::LinekongLayout() {
	}

	LinekongLayout::~LinekongLayout() {
	}

	std::string LinekongLayout::format(const LoggingEvent& event) {
		std::ostringstream message;

		char buff[64];
		time_t time = (time_t)event.timeStamp.getSeconds();

#ifndef LOG4CPP_HAVE_GETTIMEOFDAY

		struct tm newtime;   
		localtime_s( &newtime, &time ); 

   
		int year	= newtime.tm_year+1900;   
		int month	= newtime.tm_mon+1;   
		int day		= newtime.tm_mday;     
		int hour	= newtime.tm_hour;     
		int minute	= newtime.tm_min;     
		int seconds	= newtime.tm_sec;

		_snprintf(buff, 64, "[%04d-%02d-%02d %02d:%02d:%02d]", year, month, day, hour, minute, seconds );
#else 
		struct tm newtime;   
		localtime_r( &time, &newtime ); 

		int year	= newtime.tm_year+1900;   
		int month	= newtime.tm_mon+1;   
		int day		= newtime.tm_mday;     
		int hour	= newtime.tm_hour;     
		int minute	= newtime.tm_min;     
		int seconds	= newtime.tm_sec;

		snprintf(buff, 64, "[%04d-%02d-%02d %02d:%02d:%02d]", year, month, day, hour, minute, seconds );

#endif
		const std::string& priorityName = Priority::getPriorityName(event.priority);
		message << buff  << " " << priorityName << " " 
			/*<< event.categoryName << " "*/ << event.ndc << ": " 
			<< event.message << std::endl;

		return message.str();
	}


}
