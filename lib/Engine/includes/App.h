#ifndef __APP_H
#define __APP_H

class App {
	
public :
	virtual ~App() {};
	virtual void OnCreateDevice() = 0;
	virtual void OnResetDevice() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
};

#endif