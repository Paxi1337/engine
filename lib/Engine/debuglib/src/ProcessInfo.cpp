#include "../includes/ProcessInfo.h"


// internal linkage of objects in anomynous namespace, therefore can never be accessed by another translation unit
// static keyword before @s_info and @init_s_info was added to ensure the variable does not show up in .map file
namespace {
	bool init_process();
	
	struct PI {
		// process handle
		HANDLE process;
		// thread handle
		HANDLE thread;
		// base address of current programm
		HMODULE hwnd;
		// machine type 
		DWORD image_type;
		// void pointer to first address the executable or dll has access to
		LPVOID lpMinimumApplicationAddress;
		// baseaddress of image loaded into memory
		DWORD image_base;
	};
	
	struct InitHelper {
		InitHelper(void) {
			init_process();
		}
		~InitHelper(void) {}
	};
	
	static PI s_info;
	static InitHelper init_s_info;
}

bool ::init_process() {
	// checks if s_info is already initialized
	// if so it just returns true
	if(!::s_info.process) {
		::ZeroMemory(&::s_info,sizeof(::s_info));
		s_info.process = ::GetCurrentProcess();
		// GetCurrentProcess() always returns a pseudo handle (currently (HANDLE)-1 => 0xFFFFFFFF)
		if(!::s_info.process) {
			return false;
		}

		/*HANDLE hToken;
		LUID luid;
		TOKEN_PRIVILEGES tkp;

		OpenProcessToken(s_info.process, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = luid;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL);

		CloseHandle(hToken); 
*/
		// initializes the symbol handler for the process being debugged
		::SymInitialize(::s_info.process,0,TRUE);

		::s_info.thread = ::OpenThread(THREAD_ALL_ACCESS | THREAD_SET_CONTEXT,0,GetCurrentThreadId());
		 //= ::GetCurrentThread();
		
		if(::s_info.thread == INVALID_HANDLE_VALUE) {
			return false;
		}

		// retrieve base address of current programm 
		// used to obtain headerinfo afterwards in this case
		::s_info.hwnd = GetModuleHandle(0);
		if(!::s_info.hwnd) {
			return false;
		}
	
		// ImageNtHeaders() returns Pointer to already allocated Structure in the PE Header 
		// Remainder: do not delete!!
		IMAGE_NT_HEADERS* ntheader = 0;
		ntheader = ::ImageNtHeader(::s_info.hwnd);
		if(!ntheader) {
			return false;
		}
		::s_info.image_type = ntheader->FileHeader.Machine;
		::s_info.image_base = ntheader->OptionalHeader.ImageBase;
	}
	return true;
}


const DWORD debuglib::process::get_pid() {
	return ::GetProcessId(s_info.process);
}

const DWORD debuglib::process::image_type() {
	return ::s_info.image_type;
}

const HANDLE& debuglib::process::process() {
	return ::s_info.process;
}
	
const HANDLE& debuglib::process::thread() {
	return ::s_info.thread;
}
