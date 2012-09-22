#ifndef __RENDERENGINE_H
#define __RENDERENGINE_H

class RenderEngine {

public:
	virtual ~RenderEngine();
	virtual void release() = 0; 
	virtual void onCreateDevice(HWND hWindow) = 0; 
	virtual void onResetDevice() = 0; 
	virtual void onLostDevice() = 0; 
	virtual void onDestroyDevice() = 0; 
	virtual void onUpdateFrame() = 0; 
	virtual void onRenderFrame() = 0; 
	virtual void onKeyDown() = 0;
};

#endif