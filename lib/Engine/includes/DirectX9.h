#ifndef __DIRECTX9_H
#define __DIRECTX9_H

#include "RenderEngine.h"
#include <direct.h>

class DirectX9 : public RenderEngine {
public:
	DirectX9();
	~DirectX9();

	void release(); 
	void onCreateDevice(const HWND hWindow); 
	void onResetDevice(); 
	void onLostDevice(); 
	void onDestroyDevice(); 
	void onUpdateFrame(); 
	void onRenderFrame(); 
	void onKeyDown();

private:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3D9 m_pD3D;
	HWND m_hWindow;

};


#endif