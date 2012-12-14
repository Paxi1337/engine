/*
 * Remainder: Never use const int variables in header! (http://www.mikrocontroller.net/topic/97521)
 */
#ifndef __DEBUGLIB__
#define __DEBUGLIB__

#include "Logdispatch.h"
#include "CallStack.h"
//#include "Breakpoints.h"

#pragma comment(lib, "dbghelp.lib")

//#define __DEBUG__
// set excpetion handler
// if sw breakpoint is not handeld by debugger show messagebox and quit program
//#define SWBP SetUnhandledExceptionFilter(debuglib::breakpoints::ExceptionFilter); __debugbreak();
#define __DEBUG__

// custom ASSERT makro
#ifdef __DEBUG__
	#define ASSERT(cond, msg) \
			do { \
				if(!(cond)) { \
					debuglib::callstack::CallStack cs; \
					debuglib::logdispatch::log(cs.to_str(), debuglib::logger::FATAL_ERR); \
					debuglib::logdispatch::log(msg, debuglib::logger::FATAL_ERR); \
					__debugbreak(); \
				} \
			} \
			while(0)
#else
	#define ASSERT(cond, msg) \
			do { \
				(void)sizeof(cond); \
			} \
			while(0)
#endif



// makro for setting current CPU context
#define CURRENTCONTEXT(c) \
	::ZeroMemory(&c,sizeof(c)); \
	c.ContextFlags = CONTEXT_DEBUG_REGISTERS | CONTEXT_FULL; \
	::RtlCaptureContext(&c);


#endif