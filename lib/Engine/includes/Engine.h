#ifndef __ENGINE_H
#define __ENGINE_H

#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 4668)
#pragma warning(pop)

#include "Window.h"
#include "Matrix3.h"

class TinyEngine {

public:
	void setApp(App*);
	int getDirectX9RenderDevice();

private :
	//Window* 
};


#endif