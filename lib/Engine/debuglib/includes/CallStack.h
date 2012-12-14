///////////////////////////////////////////////////////////////////////////////
//	@author Paxi
//  
//	This header file is part of the debuglib project
//  ::callstack namespace is used to get callstack including symbol information
///////////////////////////////////////////////////////////////////////////////

#ifndef __CALLSTACK__
#define __CALLSTACK__

#include <vector>
#include <Windows.h>
#include <DbgHelp.h>
#include "ProcessInfo.h"
#include <String.h>

namespace debuglib {
	namespace callstack {

		// max length of one symbol
		#define MAX_SYMBOL_LEN 1024

		// struct with that stores usefull information for one callstack entry
		typedef struct CallStackEntry {
			DWORD64 base_address;
			DWORD64 return_address;
			char* file_name;
			char* symbol_name;
			DWORD line_number;

			CallStackEntry(DWORD64 ba, DWORD64 ra, char* f_name, char* s_name, DWORD ln) : 
							base_address(ba), return_address(ra), file_name(new char[strlen(f_name)+1]), symbol_name(new char[strlen(s_name)+1]), line_number(ln)  
			{
				memcpy(file_name,f_name, strlen(f_name)+1);
				memcpy(symbol_name,s_name,strlen(s_name)+1);
			}

		} CS;

		class CallStack {
		
		public:
			CallStack(void);
			~CallStack(void);

			// return callstack as string (used by Logger)
			const char* to_str();
		
		private:
			// obtains callstack
			bool get_call_stack(void);

			// saved callstackentries
			std::vector<CS> m_ces;
		};
	}
}

#endif