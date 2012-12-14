#ifndef __ENGINE_H
#define __ENGINE_H

#include "Window.h"

// TODO this class should contain window and wrapp all functions again
// so user can just get everything from TinyEngine inside the App
class TinyEngine {

public:
	void setApp(App*);
	int getDirectX9RenderDevice();

private :
	
};

#endif