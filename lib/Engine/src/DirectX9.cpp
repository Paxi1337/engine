#include "../includes/DirectX9.h"

DirectX9::DirectX9() {
	m_vertexBuffers = new std::map<std::string,LPDIRECT3DVERTEXBUFFER9>();
}

DirectX9::~DirectX9() {

}

void DirectX9::init(HWND window) {
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// struct that holds informations provided to the video card
	D3DPRESENT_PARAMETERS dev_info;

	memset(&dev_info,'\0', sizeof(dev_info));
	
	dev_info.BackBufferCount = 1; // 1 back buffer (double buffering)
	dev_info.SwapEffect = D3DSWAPEFFECT_DISCARD; // discard previous frame
	dev_info.MultiSampleType = D3DMULTISAMPLE_NONE; // no multi sampling
	dev_info.MultiSampleQuality = 0; // no multi sampling
	dev_info.hDeviceWindow = window;
	dev_info.Flags = 0;
	dev_info.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	dev_info.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	dev_info.EnableAutoDepthStencil = true; // z-buffer
	dev_info.AutoDepthStencilFormat = D3DFMT_D16;
	dev_info.BackBufferFormat = D3DFMT_X8R8G8B8;
#ifndef FULLSCREEN
	dev_info.Windowed = true;
#else
	dev_info.Windowed = false;
	dev_info.BackBufferWidth = WINDOW_WIDTH;
	dev_info.BackBufferHeight = WINDOW_HEIGHT;
#endif

	HRESULT result;

	result = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
							   D3DDEVTYPE_HAL,
							   window,
							   D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							   &dev_info,
							   &m_pDevice);
}

void DirectX9::release() {
	std::map<std::string,LPDIRECT3DVERTEXBUFFER9>::iterator iter;
	iter = m_vertexBuffers.begin();
	while(iter!=m_vertexBuffers.end()) {
		iter->second->Release();
		++iter;
	}
	m_pDevice->Release();
	m_pD3D->Release();
}


LPDIRECT3DDEVICE9 DirectX9::getD3D9Device() const {
	return m_pDevice;
}

void DirectX9::setRenderState(D3DRENDERSTATETYPE renderState, const DWORD value) {
	m_pDevice->SetRenderState(renderState, value);
}