#ifndef __DIRECTX9_H
#define __DIRECTX9_H

#include "RenderEngine.h"
#include <d3d9.h>


class DirectX9 : public RenderEngine {
public:
	DirectX9();
	~DirectX9();

	void release(); 
	void init(HWND hWindow);
	inline void* getRenderDevice() const;
	
	inline LPDIRECT3DDEVICE9 getD3D9Device() const;
	void setRenderState(const int,const int);

	
	// alpha blending
	static const int BLEND_SRC = D3DRS_SRCBLEND;
	static const int BLED_DEST = D3DRS_DESTBLEND;
	static const int BLEND_OP = D3DRS_OPBLEND;
	static const int BLENDING = D3DRS_ALPHABLENDENABLE;

	static const int LIGHTING = D3DRS_LIGHTING;
	static const int ZBUFFER = D3DRS_ZENABLE;
    static const int FILLMODE = D3DRS_FILLMODE;
    static const int SHADEMODE = D3DRS_SHADEMODE;
	
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3D9 m_pD3D;
	HWND m_hWindow;    

};


#endif