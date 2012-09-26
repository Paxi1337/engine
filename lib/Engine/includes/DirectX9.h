#ifndef __DIRECTX9_H
#define __DIRECTX9_H

#include <d3d9.h>
#include <d3dx9.h>

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
	void lockVertexBuffer();
	void unlockVertexBuffer();

	
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
	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3D9 m_pD3D;
	HWND m_hWindow;    

};


#endif