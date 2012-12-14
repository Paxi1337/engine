#include "../includes/CallStack.h"
#include "../includes/ProcessInfo.h"
#include "../includes/Logger.h"
#include "../includes/DebugLib.h"


//using namespace debuglib::callstack;


debuglib::callstack::CallStack::CallStack() {
	
}

debuglib::callstack::CallStack::~CallStack() {
	
}

const char* debuglib::callstack::CallStack::to_str() {
	get_call_stack();

	std::vector<CS>::iterator itr;
	// offset used by memcpy
	int offset = 0;
	// max memory per symbol + remains
	char buffer[MAX_SYMBOL_LEN+200];
	// allocating max memory that could be needed
	char* all = new char[(MAX_SYMBOL_LEN+200)*m_ces.size()];

	// skip first function (to_str())
	for(itr = m_ces.begin()+1; itr < m_ces.end(); ++itr ) {	
		sprintf_s(buffer,MAX_SYMBOL_LEN+200, "%s() called\r\nBaseAddress: 0x%08llx\r\nReturnaddress: 0x%08llx\r\nIn File: %s (Line: %d)\r\n\r\n",itr->symbol_name, itr->base_address, itr->return_address, itr->file_name, itr->line_number);
		memcpy(all+offset,buffer,strlen(buffer));
		offset+=strlen(buffer);
	}
	// terminate string with 0 byte
	all[offset] = '\0';
	return all;
}

bool debuglib::callstack::CallStack::get_call_stack() {
	// used for SymGetAddress and SymGetLine
	DWORD64 displacement64 = 0;
	DWORD displacement = 0;

	unsigned int framecnt = 0;
	CONTEXT c;
	STACKFRAME64 stackframe;
	IMAGEHLP_SYMBOL64 *symbol;
	IMAGEHLP_LINE64 line;
	
	// initialize line
	::ZeroMemory(&line, sizeof(IMAGEHLP_LINE64));
	line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
	
	// initialize symbol
	symbol = (IMAGEHLP_SYMBOL64 *)::operator new(sizeof(*symbol) + MAX_SYMBOL_LEN);
	::ZeroMemory(symbol, sizeof(IMAGEHLP_SYMBOL64)+MAX_SYMBOL_LEN);
	symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
	symbol->MaxNameLength = MAX_SYMBOL_LEN;
	
	// initialize stackframe
	::ZeroMemory(&stackframe, sizeof(STACKFRAME64));
	
	// makro defined in CallStack.h
	// initializes CONTEXT c and loads current context of CPU
	CURRENTCONTEXT(c);

#ifdef _M_IX86
	// X86 Architecture
	  stackframe.AddrPC.Offset = c.Eip;
	  stackframe.AddrPC.Mode = AddrModeFlat;
	  stackframe.AddrFrame.Offset = c.Ebp;
	  stackframe.AddrFrame.Mode = AddrModeFlat;
	  stackframe.AddrStack.Offset = c.Esp;
	  stackframe.AddrStack.Mode = AddrModeFlat;
#elif defined(_M_AMD64)
	// X64 Archicture (Intel and AMD)
	return false;
#else
	// Intel Itanium
	return false;
#endif	
	// make sure to clear m_ces before filling it
	m_ces.clear();
	do {
		if(! ::StackWalk64(process::image_type(),process::process(),process::thread(),&stackframe,&c,0,0,0,0)) {
			//StackWalk64 does not set the last error code!
			return false;
		}
		if(! ::SymGetSymFromAddr64(process::process(),stackframe.AddrPC.Offset,&displacement64,symbol)) {
			return false;
		}
		if(! ::SymGetLineFromAddr64(process::process(),stackframe.AddrPC.Offset,&displacement,&line)) {
			return false;
		}
		
		char* fname = strrchr(line.FileName,'\\')+1;
		//std::cout << symbol.Name << "!COUT() called\n" << "Baseaddress: 0x" << std::hex << stackframe.AddrPC.Offset << "\nReturnaddress: 0x" << stackframe.AddrReturn.Offset << "\nFile: " << fname << " (Line: " << std::dec << line.LineNumber << ")\n\n!";
		CS cs(stackframe.AddrPC.Offset, stackframe.AddrReturn.Offset, fname, symbol->Name,line.LineNumber);
		m_ces.push_back(cs);
		framecnt++;
	}
	while(stackframe.AddrPC.Offset != 0 && strcmp(symbol->Name,"main"));

	delete symbol;
	return true;
}
