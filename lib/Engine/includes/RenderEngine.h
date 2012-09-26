#ifndef __RENDERENGINE_H
#define __RENDERENGINE_H

#include "Window.h"

class RenderEngine {

public:
	virtual ~RenderEngine() {} ;
	
	virtual void init() = 0;
	virtual void release() = 0;
	virtual void* getRenderDevice() const = 0; 
};

#endif