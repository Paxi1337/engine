///////////////////////////////////////////////////////////////////////////////
//	@author Paxi
//  
//	This header file is part of the debuglib project
//  ::process namespace is used to obtain infos or handles to the current process/thread beeing debugged
///////////////////////////////////////////////////////////////////////////////

#ifndef __PROCESSINFO__
#define __PROCESSINFO__

#include <Windows.h>
#include <DbgHelp.h>
#include <vector>

namespace debuglib {
	
	namespace process {
		
		///////////////////////////////////////////////////////////////////////////////
		// returns HANDLE of process being debugged
		///////////////////////////////////////////////////////////////////////////////
		const HANDLE& process(void);
		
		///////////////////////////////////////////////////////////////////////////////
		// returns HANDLE of thread within process being debugged
		// no multithreadsupport for now
		///////////////////////////////////////////////////////////////////////////////
		const HANDLE& thread(void);
		
		///////////////////////////////////////////////////////////////////////////////
		// return image_type (x86, x64, IA64) from executable
		// x86 is only architecture supported for now
		///////////////////////////////////////////////////////////////////////////////
		const DWORD image_type(void);

		///////////////////////////////////////////////////////////////////////////////
		// returns pid of process being debugged
		///////////////////////////////////////////////////////////////////////////////
		const DWORD get_pid(void);
	}
}

#endif