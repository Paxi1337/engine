#include "../includes/VSLogger.h"

/*
//////////////////////
=====================
VSLogger implementation
=====================
//////////////////////
*/
VSLogger::VSLogger() {
	
}

VSLogger::~VSLogger() {

}

void VSLogger::outputter(debuglib::logger::IOutputter* out) {
	m_out = out;
}

void VSLogger::verbosity(const int level) {
	m_verbosity = level;
}

void VSLogger::log(const char* msg, const int level) {
	if(m_out) {
		if(!m_verbosity) {
			::MessageBoxA(0, "No verbosity level set. Message will not be logged.", "DEBUGLIB", MB_OK);
			return;
		}
		else if(level >= m_verbosity) {
			m_out->out(msg, level);
		}
	}
	else {
		::MessageBoxA(0, "No Outputter set. Message will not be logged.", "DEBUGLIB", MB_OK);
		return;
	}
}

/*
//////////////////////
=====================
VSOutputter implementation
=====================
//////////////////////
*/
VSOutputter::VSOutputter() {
	
}

VSOutputter::~VSOutputter() {

}

void VSOutputter::out(const char* msg, const int level) {
	size_t size = strlen(msg);
	char* fullmsg = new char[size+20];
	sprintf_s(fullmsg,size+20,"Msg: %s Level: %d\n",msg,level);
	::OutputDebugStringA(fullmsg);
	delete fullmsg;
}