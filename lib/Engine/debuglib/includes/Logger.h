///////////////////////////////////////////////////////////////////////////////
//	@author Paxi
//  
//	This header file is part of the debuglib project
//  ::logger namespace defines Logger und Outputter base class and verbosity Levels
///////////////////////////////////////////////////////////////////////////////

#ifndef __LOGGER__
#define __LOGGER__
#include <iostream>

namespace debuglib 
{
	namespace logger 
	{
		// Verbosity Levels
		enum LogLevel {
			UNDEFINED = 0,
			DEBUG = 1, 
			INFO = 2, 
			WARN = 3, 
			ERR = 4,
			FATAL_ERR = 5
		};
		
		// every Outputter must derive from IOutputter
		class IOutputter {
		public:
			// this function is used to proceed the msg and logging level to the beloning outputter 
			virtual void out(const char*, const int) = 0;	
		};
		
		// every logger must derive from ILogger
		class ILogger {
		public :
			ILogger(void);
			virtual ~ILogger(void);
			ILogger* m_next;
			ILogger* m_prev;
			int m_verbosity;
			IOutputter* m_out;
			virtual void log(const char*, const int) = 0;

		protected:	
			virtual void outputter(IOutputter*) = 0;
			virtual void verbosity(const int) = 0;
		};

	}// logger Ende
}// debuglib Ende
#endif