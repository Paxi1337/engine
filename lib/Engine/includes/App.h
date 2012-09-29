#ifndef __APP_H
#define __APP_H

class App {
	
public :
	virtual ~App() {};
	virtual void onCreateDevice() = 0;
	virtual void onResetDevice() = 0;
	virtual void onReleaseDevice() = 0;
	virtual void onUpdate() = 0;
	virtual void onRender() = 0;
	virtual void onLostDevice() = 0;
};

#endif