#ifndef __DIRECTX9_H
#define __DIRECTX9_H

#include <d3d9.h>
#include <d3dx9.h>


class Matrix3;

typedef const int STATE;

class DirectX9 {
	friend class Window;

public:
	DirectX9();
	~DirectX9();

	void release(); 
	void init();
	
	inline LPDIRECT3DDEVICE9 getD3D9Device() const;
	void setRenderState(DWORD, const int);

	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(int sizeOfCustomVertexStruct);
	LPDIRECT3DINDEXBUFFER9 createIndexBuffer();
	LPD3DXMESH createMesh();
	
	void worldTransform(Matrix3*);
	void viewTransform();
	void perspectiveProjection();
	void orthoProjection();	
	void setFVF();	

	void render();

	
	// states see http://msdn.microsoft.com/en-us/library/windows/desktop/bb172599(v=vs.85).aspx

	// alpha blending
	static STATE BLEND_SRC = D3DRS_SRCBLEND;
	static STATE BLED_DEST = D3DRS_DESTBLEND;
	static STATE BLEND_OP = D3DRS_BLENDOP;
	static STATE BLENDING = D3DRS_ALPHABLENDENABLE;

	// zbuffer
	static STATE ZBUFFER = D3DRS_ZENABLE;
	
	// render modes
	static STATE FILLMODE = D3DRS_FILLMODE;
	static STATE SHADEMODE = D3DRS_SHADEMODE;

	// fog
	static STATE FOG = D3DRS_FOGENABLE;
	static STATE FOG_COLOR = D3DRS_FOGCOLOR;
	static STATE FOG_TABLEMODE = D3DRS_FOGTABLEMODE;
	static STATE FOG_STARTD3DRS_FOGSTART;
	static STATE FOG_ENDD3DRS_FOGEND;
	static STATE FOG_DENSITY = D3DRS_FOGDENSITY;
	
	// color lighting
	static STATE LIGHTING = D3DRS_LIGHTING;
	static STATE COLOR_EMISSIVE = D3DRS_EMISSIVEMATERIALSOURCE;
	static STATE COLOR_DIFFUSE = D3DRS_DIFFUSEMATERIALSOURCE;
	static STATE COLOR_SPECULAR = D3DRS_SPECULARMATERIALSOURCE;
	static STATE COLOR_AMBIENT = D3DRS_AMBIENTMATERIALSOURCE;
	
private:

	inline int calcPrimitiveCount();

	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3D9 m_pD3D;
	HWND m_hWindow;
	
	int m_sizeOfVertex;
	int m_vertexCount;
	int m_primitiveCount;

	/**
		typedef enum D3DPRIMITIVETYPE { 
		  D3DPT_POINTLIST       = 1,
		  D3DPT_LINELIST        = 2,
		  D3DPT_LINESTRIP       = 3,
		  D3DPT_TRIANGLELIST    = 4,
		  D3DPT_TRIANGLESTRIP   = 5,
		  D3DPT_TRIANGLEFAN     = 6,
		  D3DPT_FORCE_DWORD     = 0x7fffffff 
		} D3DPRIMITIVETYPE, *LPD3DPRIMITIVETYPE;
	*/
	D3DPRIMITIVETYPE m_primitiveType;

	
	D3DFVF m_FVF;    

};


#endif