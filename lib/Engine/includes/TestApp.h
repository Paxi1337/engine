#ifndef __TESTAPP_H
#define __TESTAPP_H

#include "Window.h"
#include "FPCamera.h"

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
	void onKeyPressed(WPARAM keyCode);
	void onRawMouseInputReceived(RAWINPUT const& rawMouseInput);
	void setWindow(Window* window);
	
private:
	Window* m_window;
	VertexbufferInfo* m_buffer;
	VertexbufferInfo* m_buffer2;
	VertexbufferInfo* m_buffer3;
	VertexbufferInfo* m_buffer4;
	VertexbufferInfo* m_buffer5;

	double m_timeSinceElapsedTimeReset;
	CFPCamera* mNewCamera;
};

#endif