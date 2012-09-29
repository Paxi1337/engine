#ifndef __TESTAPP_H
#define __TESTAPP_H

#include "Window.h"

class TestApp : public App {
public:
	TestApp();
	explicit TestApp(Window* window);
	~TestApp();

	void onCreateDevice();
	void onResetDevice();
	void onReleaseDevice();
	void onUpdate();
	void onRender();
	void onLostDevice();

	void setWindow(Window* window);

private:
	Window* m_window;

	VertexbufferInfo* m_buffer;
};

#endif