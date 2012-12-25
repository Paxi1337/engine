#ifndef XMANAGER_H
#define XMANAGER_H_H

#include "vertexdeclarations.h"
#include "structdeclarations.h"
#include <vector>
#include <string>

//TODO: better design, think about features
// VertexDecl should be dynamically changeable, still need to do some things in DX9 class before
class XManager {
	XManager();
	~XManager();
	
	void loadXFile(IDirect3DDevice9* dev, const std::string& filename, ID3DXMesh** meshOut, std::vector<Material>& mtrls, std::vector<IDirect3DTexture9*>& texs);
};

#endif