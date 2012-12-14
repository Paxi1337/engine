#include "../includes/Logdispatch.h"

// global head pointer, probably ** pointer to pointer ??
debuglib::logger::ILogger* g_head = 0;
// global tmp pointer
// always points to the element last added
debuglib::logger::ILogger* g_tmp = 0;

// TODO this function may not work as expected sometimes
void debuglib::logdispatch::addLogger(debuglib::logger::ILogger* l) {
	// if headpointer is still 0, this is the headpointer
	if(!g_head) {
		l->m_prev = 0;
		l->m_next = 0;
		g_head = l;
		g_tmp = l;
	}
	else {
		l->m_prev = g_tmp;
		g_tmp->m_next = l;
		l->m_next = 0;
		g_tmp = l;
	}
}

// TODO this function may not work as expected sometimes
void debuglib::logdispatch::removeLogger(debuglib::logger::ILogger* l) {
	debuglib::logger::ILogger* tmp = g_head;

	while(true) {
		if(tmp == l) {
			// first element gets deleted
			if(!l->m_prev) {
				g_head = g_head->m_next;
			}
			// last element gets deleted
			else if(!l->m_next) {
				l->m_prev->m_next = 0;
			}
			else {
				l->m_prev->m_next = l->m_next;
			}
			break;
		}
		tmp = tmp->m_next;
	}
}

int debuglib::logdispatch::size() {
	if(!g_head) return 0;
	//if(!g_head.next) return 1;
	int cnt = 1;
	debuglib::logger::ILogger* tmp = g_head;
	
	while(tmp->m_next) {
		cnt++;
		tmp = tmp->m_next;
	}
	
	return cnt;
}

void debuglib::logdispatch::log(const char* msg, const unsigned int level) {
	debuglib::logger::ILogger* tmp = g_head;
	if(size()) {
		while(tmp) {
			tmp->log(msg, level);
			tmp = tmp->m_next;
		}
	}
}

