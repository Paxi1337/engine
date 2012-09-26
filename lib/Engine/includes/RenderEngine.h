#ifndef __RENDERENGINE_H
#define __RENDERENGINE_H

#include <Windows.h>

class RenderEngine {

public:
	virtual ~RenderEngine() {} ;
	
	virtual void init(HWND hWindow) = 0;
	virtual void release() = 0;
	virtual void* getRenderDevice() const = 0; 
};

#endif