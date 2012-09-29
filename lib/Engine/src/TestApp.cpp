#include "../includes/TestApp.h" 



TestApp::TestApp() : m_window(NULL) {

}

TestApp::TestApp(Window* window) : m_window(window) {

}

TestApp::~TestApp() {

}

void TestApp::onCreateDevice() {
	if(m_window) {
		m_buffer = m_window->getRenderDevice()->createVertexBuffer(3,CUSTOMVERTEX3COLORFORMAT,std::string("test"));
	}
}

void TestApp::onResetDevice() {

}

void TestApp::onReleaseDevice() {

}

void TestApp::onUpdate() {

}

void TestApp::onRender() {
	if(m_window) {
		m_window->getRenderDevice()->renderVertexbuffer(D3DPT_TRIANGLELIST,std::string("test"));
	}
}

void TestApp::onLostDevice() {

}

void TestApp::setWindow(Window* window) {
	m_window = window;
}
