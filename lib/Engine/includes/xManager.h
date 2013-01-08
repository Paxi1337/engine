#ifndef XMANAGER_H
#define XMANAGER_H_H

#include "vertexdeclarations.h"
#include "structdeclarations.h"
#include <vector>
#include <string>

class XManager {
public:
	XManager();
	~XManager();
	
	static void loadXFile(IDirect3DDevice9* dev, const std::string& filename, ID3DXMesh** meshOut, std::vector<Material>* mtrls, std::vector<IDirect3DTexture9*>* texs);
};

#endif