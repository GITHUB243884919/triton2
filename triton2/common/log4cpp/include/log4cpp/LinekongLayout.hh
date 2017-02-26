/*
* LinekongLayout.hh
*
* Copyright 2008, LineKong . All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#ifndef _LOG4CPP_LINEKONGLAYOUT_HH
#define _LOG4CPP_LINEKONGLAYOUT_HH

#include <log4cpp/Portability.hh>
#include <log4cpp/Layout.hh>

namespace log4cpp {

	/**
	* LinekongLayout is a simple fixed format Layout implementation. 
	**/
	class LOG4CPP_EXPORT LinekongLayout : public Layout {
	public:
		LinekongLayout();
		virtual ~LinekongLayout();

		/**
		* Formats the LoggingEvent in LinekongLayout style:<br>
		* "timeStamp priority category ndc: message"
		**/
		virtual std::string format(const LoggingEvent& event);
	};        
}

#endif // _LOG4CPP_LINEKONGLAYOUT_HH
