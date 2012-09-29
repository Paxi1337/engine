#include "../includes/TestApp.h" 



TestApp::TestApp() : m_window(NULL) {

}

TestApp::TestApp(Window* window) : m_window(window) {

}

TestApp::~TestApp() {

}

void TestApp::onCreateDevice() {
	
	CustomVertexTransformedColor vertices[] =
	{
		{ 200.0f, 32.5f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
		{ 350.0f, 200.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
		{ 50.0f, 200.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
	};

	
	if(m_window) {
		m_buffer = m_window->getRenderDevice()->createVertexBuffer(3, CUSTOMVERTEXTRANSFORMEDCOLORFORMAT, std::string("test"));
		//m_window->getRenderDevice()->setVertexBufferData(std::string("test"), vertices);

		void* vram = m_window->getRenderDevice()->getRawVideoMemoryPtr(std::string("test"));
		memcpy(vram,vertices,3*sizeof(CustomVertexTransformedColor));
		m_window->getRenderDevice()->unlockRawVideoMemoryPtr(std::string("test"));
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
