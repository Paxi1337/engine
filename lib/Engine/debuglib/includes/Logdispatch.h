///////////////////////////////////////////////////////////////////////////////
//	@author Paxi
//  
//	This header file is part of the debuglib project
//  ::logdisptach namespace is used to manage all loggers
///////////////////////////////////////////////////////////////////////////////

#ifndef __LOGDISPATCH__
#define __LOGDISPATCH__

#include "Logger.h"

namespace debuglib 
{
	namespace logdispatch 
	{
		void addLogger(debuglib::logger::ILogger*);
		void removeLogger(debuglib::logger::ILogger*);
		void log(const char*, const unsigned int);
		int size();
	}
}

#endif