#ifndef __APP_H
#define __APP_H

#include <Windows.h>

class App {
	
public :
	virtual ~App() {};
	virtual void onCreateDevice() = 0;
	virtual void onResetDevice() = 0;
	virtual void onReleaseDevice() = 0;
	virtual void onUpdate() = 0;
	virtual void onRender() = 0;
	virtual void onLostDevice() = 0;
	virtual void onKeyPressed(WPARAM keyCode) = 0;
	virtual void onRawMouseInputReceived(RAWINPUT const& rawMouseInput) = 0;
	virtual void onCustomUserFunction() = 0;
};

#endif