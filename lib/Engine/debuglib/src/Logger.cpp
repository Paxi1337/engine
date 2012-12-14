#include "../includes/Logdispatch.h"

// Logger baseclass
debuglib::logger::ILogger::ILogger() {
	m_out = 0;
	m_verbosity = UNDEFINED;

	debuglib::logdispatch::addLogger(this);
	//printf("Element %08x added, Current Size: %d\n", this, debuglib::logdispatch::size());
}

debuglib::logger::ILogger::~ILogger() {
	debuglib::logdispatch::removeLogger(this);
	//printf("Element %08x deleted, Current Size:%d\n", this, debuglib::logdispatch::size());
}
