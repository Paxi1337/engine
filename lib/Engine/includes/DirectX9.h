#ifndef __DIRECTX9_H
#define __DIRECTX9_H

#include "App.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <map>

//typedef enum D3DPRIMITIVETYPE { 
//	D3DPT_POINTLIST       = 1,
//	D3DPT_LINELIST        = 2,
//	D3DPT_LINESTRIP       = 3,
//	D3DPT_TRIANGLELIST    = 4,
//	D3DPT_TRIANGLESTRIP   = 5,
//	D3DPT_TRIANGLEFAN     = 6,
//	D3DPT_FORCE_DWORD     = 0x7fffffff 
//} D3DPRIMITIVETYPE, *LPD3DPRIMITIVETYPE;
typedef D3DPRIMITIVETYPE T_PRIMITIVE;
// http://msdn.microsoft.com/en-us/library/windows/desktop/bb172599(v=vs.85).aspx
typedef D3DRENDERSTATETYPE T_RENDERSTATE;

//typedef enum D3DTRANSFORMSTATETYPE { 
//	D3DTS_VIEW          = 2,
//	D3DTS_PROJECTION    = 3,
//	D3DTS_TEXTURE0      = 16,
//	D3DTS_TEXTURE1      = 17,
//	D3DTS_TEXTURE2      = 18,
//	D3DTS_TEXTURE3      = 19,
//	D3DTS_TEXTURE4      = 20,
//	D3DTS_TEXTURE5      = 21,
//	D3DTS_TEXTURE6      = 22,
//	D3DTS_TEXTURE7      = 23,
//	D3DTS_FORCE_DWORD   = 0x7fffffff 
//} D3DTRANSFORMSTATETYPE, *LPD3DTRANSFORMSTATETYPE;
typedef D3DTRANSFORMSTATETYPE T_TRASFORM;

struct CustomVertex3ColorUV {
	D3DXVECTOR3 xyz;
	DWORD color;
	DWORD u,v;
};

#define CUSTOMVERTEX3COLORUVFORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct CustomVertex3Color {
	D3DXVECTOR3 xyz;
	DWORD color;
};

#define CUSTOMVERTEX3COLORFORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE)

class DirectX9 {

public:
	DirectX9();
	~DirectX9();

	// releases DirectX interfaces
	void release();

	// initialized DirectX
	// TODO options for window
	void init(HWND hWindow);

	// TODO toggleFullscreen();
	
	// get pointer to IDirect3DDevice interface
	inline LPDIRECT3DDEVICE9 getD3D9Device() const;
	
	// render states
	void setRenderState(T_RENDERSTATE rederState, const DWORD value);
	
	// transforms
	void setTransform(T_TRASFORM transformState, const D3DMATRIX* pMatrix);

	// vertex buffer functions
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(const DWORD numberOfVertices, const DWORD FVF, std::string tag);
	// the customVertex FVF has to be exactly as defined in the associated buffer 
	void setVertexBufferData(std::string tag, void* customVertices);
	
	// this function returns the raw video memory pointer given by
	// buffer->Lock(0, 0, (void**)&pVoid, 0); return pVoid;
	// u must not forget to call unlockRawVideoMemoryPtr to tell DirectX9 the memory region locked can be unlocked again
	void* getRawVideoMemoryPtr(std::string vertexBuffer);
	void unlockRawVideoMemoryPtr(std::string vertexBuffer);

	// render given vertex buffer 
	// important: it is assumed that you provide the correct information about the size of the customVertexStruct and their amount
	void render(T_PRIMITIVE type, LPDIRECT3DVERTEXBUFFER9 bufferToRender, const DWORD sizeOfCustomVertex, const DWORD numberOfVertices);
	
	// http://www.mvps.org/directx/articles/d3dxmesh.htm
	// TODO LPD3DXMESH createMesh();
	
private:

	// calculates the amount of primitives that have to be drawn within render method
	inline int calcPrimitiveCount(T_PRIMITIVE renderType, const DWORD numberOfVertices);

	LPDIRECT3DDEVICE9 m_pDevice; 
	LPDIRECT3D9 m_pD3D;

	std::map<std::string,LPDIRECT3DVERTEXBUFFER9> vertexBuffers;
	
	App* m_runningApp;
};

#endif