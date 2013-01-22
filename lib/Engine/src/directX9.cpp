#include "../includes/directX9.h"
#include "../includes/timer.h"
#include <string.h>

DirectX9::DirectX9() : mDevice(NULL), mD3D(NULL), mRunningApp(NULL), mCurrentEffect(NULL) {
	
}

DirectX9::DirectX9(App* currentApp) : mDevice(NULL), mD3D(NULL), mRunningApp(currentApp), mCurrentEffect(NULL) {

}

DirectX9::~DirectX9() {
	release();
}

void DirectX9::attachApp(App* currentApp) {
	mRunningApp = currentApp;
}

void DirectX9::createDevice(HWND window) {
	mD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// struct that holds informations provided to the video card
	

	memset(&mDevInfo,'\0', sizeof(mDevInfo));
	
	mDevInfo.BackBufferCount = 1; // 1 back buffer (double buffering)
	mDevInfo.SwapEffect = D3DSWAPEFFECT_DISCARD; // discard previous frame
	mDevInfo.MultiSampleType = D3DMULTISAMPLE_NONE;
	mDevInfo.MultiSampleQuality = 0; 
	mDevInfo.hDeviceWindow = window;
	mDevInfo.Flags = 0;
	mDevInfo.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	mDevInfo.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	mDevInfo.EnableAutoDepthStencil = true; // z-buffer
	mDevInfo.AutoDepthStencilFormat = D3DFMT_D24S8;
	mDevInfo.BackBufferFormat = D3DFMT_UNKNOWN;
#ifndef FULLSCREEN
	mDevInfo.Windowed = true;
	mDevInfo.BackBufferHeight = 0;
	mDevInfo.BackBufferWidth = 0;
#else
	mDevInfo.Windowed = false;
	mDevInfo.BackBufferWidth = WINDOW_WIDTH;
	mDevInfo.BackBufferHeight = WINDOW_HEIGHT;
#endif

	HRESULT result;

	result = mD3D->CreateDevice(D3DADAPTER_DEFAULT,
							   D3DDEVTYPE_HAL,
							   window,
							   D3DCREATE_HARDWARE_VERTEXPROCESSING,
							   &mDevInfo,
							   &mDevice);

	// init all vertex declarations
	VertexDeclarations::initVertexDeclarations(mDevice);


	DWORD* none = new DWORD[2];
	none[0] = D3DMULTISAMPLE_NONE;
	none[1] = 0;
	mSupportedMSAAModes.push_back(std::pair<std::string, DWORD*>("NONE", none));

	DWORD qualitylevels[10];
	ZeroMemory(qualitylevels, sizeof(qualitylevels));

	result = checkMSAAModeSupport(D3DMULTISAMPLE_2_SAMPLES, qualitylevels);
	if(result != D3D_OK) {
		__debugbreak();
	}
	else {
		DWORD* aa2 = new DWORD[2];
		aa2[0] = D3DMULTISAMPLE_2_SAMPLES;
		aa2[1] = 0;
		mSupportedMSAAModes.push_back(std::pair<std::string, DWORD*>("2x", aa2));
	}

	result = checkMSAAModeSupport(D3DMULTISAMPLE_4_SAMPLES, qualitylevels);
	if(result != D3D_OK) {
		__debugbreak();
	}
	else {
		DWORD* aa4 = new DWORD[2];
		aa4[0] = D3DMULTISAMPLE_4_SAMPLES;
		aa4[1] = 0;
		mSupportedMSAAModes.push_back(std::pair<std::string, DWORD*>("4x", aa4));
	}

	result = checkMSAAModeSupport(D3DMULTISAMPLE_8_SAMPLES, qualitylevels);
	if(result != D3D_OK) {
		__debugbreak();
	}
	else {
		DWORD* aa8 = new DWORD[2];
		aa8[0] = D3DMULTISAMPLE_8_SAMPLES;
		aa8[1] = 0;
		mSupportedMSAAModes.push_back(std::pair<std::string, DWORD*>("8x", aa8));
	}

	result = checkMSAAModeSupport(D3DMULTISAMPLE_16_SAMPLES, qualitylevels);
	if(result != D3D_OK) {
		//__debugbreak();
	}
	else {
		DWORD* aa16 = new DWORD[2];
		aa16[0] = D3DMULTISAMPLE_16_SAMPLES;
		aa16[1] = 0;
		mSupportedMSAAModes.push_back(std::pair<std::string, DWORD*>("16x", aa16));
	}

	//// see http://www.nvidia.com/object/coverage-sampled-aa.html
	//

	//DWORD* csaa8x = new DWORD[2];
	//csaa8x[0] = D3DMULTISAMPLE_2_SAMPLES;
	//csaa8x[1] = 0;
	//
	//DWORD* csaa8xQ = new DWORD[2];
	//csaa8xQ[0] = D3DMULTISAMPLE_4_SAMPLES;
	//csaa8xQ[1] = 0;

	//DWORD* csaa16x = new DWORD[2];
	//csaa16x[0] = D3DMULTISAMPLE_8_SAMPLES;
	//csaa16x[1] = 0;

	//DWORD* csaa16xQ = new DWORD[2];
	//csaa16xQ[0] = D3DMULTISAMPLE_16_SAMPLES;
	//csaa16xQ[1] = 0;

	// supported MSAA modes are not checked so far (currenlty assuming the App runs under GTX680 & GTX650M)
	



	//mSupportedMSAAModes.push_back(std::pair<std::string, DWORD*>("CSAA8x", csaa8x));
	//mSupportedMSAAModes.push_back(std::pair<std::string, DWORD*>("CSAA8xQ", csaa8xQ));
	//mSupportedMSAAModes.push_back(std::pair<std::string, DWORD*>("CSAA16x", csaa16x));
	//mSupportedMSAAModes.push_back(std::pair<std::string, DWORD*>("CSAA16xQ", csaa16xQ));
}

void DirectX9::onCreateDevice() {
	if(mRunningApp)
		mRunningApp->onCreateDevice();
}

void DirectX9::release() {


	mRunningApp->onReleaseDevice();

	std::map<std::string,VertexbufferInfo*>::iterator iter;
	iter = m_vertexBuffers.begin();
	while(iter!=m_vertexBuffers.end()) {
		iter->second->buffer->Release();
		++iter;
	}

	m_vertexBuffers.clear();

	VertexDeclarations::releaseVertexDeclarations();
	mCurrentEffect->Release();

	mDevice->Release();
	mD3D->Release();

	std::vector<std::pair<std::string,DWORD*>>::iterator it;
	it = mSupportedMSAAModes.begin();
	while(it != mSupportedMSAAModes.end()) {
		delete(it->second);
		++it;
	}
	mSupportedMSAAModes.clear();
}

int DirectX9::calcPrimitiveCount(D3DPRIMITIVETYPE primitiveType, const DWORD numberOfVertices) {
	switch(primitiveType) {
		case D3DPT_POINTLIST:
			return numberOfVertices;
		case D3DPT_LINELIST:
			return ceil((double)numberOfVertices/2.0);
		case D3DPT_TRIANGLELIST:
			return ceil((double)numberOfVertices/3.0);
		case D3DPT_TRIANGLESTRIP:
			return numberOfVertices-2;
		case D3DPT_LINESTRIP:
			return numberOfVertices-1;
		default: return -1;
	}
}

int DirectX9::calcCustomStructSize(DWORD vertexType) {
	switch(vertexType) {
		case CUSTOMVERTEX3NORMAL:
			return sizeof(CustomVertex3Normal);
		case CUSTOMVERTEX3NORMALUV:
			return sizeof(CustomVertex3NormalUV);
		case CUSTOMVERTEX3NORMALUVTANGENT:
			return sizeof(CustomVertex3NormalUVTangent);
		case CUSTOMVERTEX3NORMALUVTANGENTBINORMAL:
			return sizeof(CustomVertex3NormalUVTangentBinormal);
		case CUSTOMVERTEX3COLOR:
			return sizeof(CustomVertex3Color);
		case CUSTOMVERTEX3UV:
			return sizeof(CustomVertex3UV);
		case CUSTOMVERTEXTRANSFORMEDCOLOR:
			return sizeof(CustomVertexTransformedColor);
		default: return -1;
	}
}

void DirectX9::userDefinedFunction() {
	if(mRunningApp) {
		mRunningApp->onCustomUserFunction();
	}
}

void DirectX9::renderFrame() {

	userDefinedFunction();

	mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(100, 149, 237), 1.0f, 0);
	mDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	mDevice->BeginScene();
	
	if(mRunningApp) {
		mRunningApp->onUpdate();
		mRunningApp->onRender();
	}

	mDevice->EndScene();
	
	mDevice->Present(NULL,NULL,NULL,NULL);
}

VertexbufferInfo* DirectX9::createVertexBuffer(const DWORD numberOfVertices, const DWORD FVF, const std::string tag) {
	int vertexStructSize = calcCustomStructSize(FVF);

	if(vertexStructSize == -1) 
		return NULL;

	VertexbufferInfo* info = new VertexbufferInfo;

	// create the vertex buffer before otherwise the call to CreateVertexBuffer will zero out the next DWORD in the VertexBufferInfo struct
	mDevice->CreateVertexBuffer(numberOfVertices*vertexStructSize, 0, 0, D3DPOOL_MANAGED, &info->buffer, NULL);

	info->FVF = FVF;
	info->vertexCount = numberOfVertices;
	m_vertexBuffers[tag] = info;
	return info;
}

void DirectX9::setVertexBufferData(std::string tag, void* customVertices) {
	
	VertexbufferInfo* vb = m_vertexBuffers[tag];
	int vertexStructSize = calcCustomStructSize(vb->FVF);
	
	if(vertexStructSize == -1) 
		return;
	
	void* vram;
	vb->buffer->Lock(0, vb->vertexCount*vertexStructSize, &vram, 0);
	memcpy(vram,customVertices,vb->vertexCount*vertexStructSize);
	vb->buffer->Unlock();
}


void DirectX9::renderVertexbuffer(D3DPRIMITIVETYPE type, std::string tag) {
	VertexbufferInfo* vb = m_vertexBuffers[tag];
	if(vb) {
		
		int vertexStructSize = calcCustomStructSize(vb->FVF);
		int primitiveCount = calcPrimitiveCount(type, vb->vertexCount);

		if(FAILED(mDevice->SetStreamSource(0, vb->buffer, 0, vertexStructSize))){
			return;
		}

		if(FAILED(mDevice->DrawPrimitive(type,0,primitiveCount))) {
			return;
		}
	}
}

void* DirectX9::getRawVideoMemoryPtr(std::string tag) {
	VertexbufferInfo* vb = m_vertexBuffers[tag];
	int vertexStructSize = calcCustomStructSize(vb->FVF);

	if(vb) {
		void* vram;
		vb->buffer->Lock(0,vb->vertexCount*vertexStructSize,&vram,0);
		return vram;
	}

	return NULL;
}

void DirectX9::unlockRawVideoMemoryPtr(std::string tag) {
	VertexbufferInfo* vb = m_vertexBuffers[tag];
	if(vb) {
		vb->buffer->Unlock();
	}
}

void DirectX9::dispatchKeyMessage(WPARAM keyCode) {
	if(mRunningApp) {
		mRunningApp->onKeyPressed(keyCode);
	}
}

void DirectX9::dispatchRawMouseInput(RAWINPUT const& rawMouseInput) {
	if(mRunningApp) {
		mRunningApp->onRawMouseInputReceived(rawMouseInput);
	}
}

void DirectX9::loadEffectFromFile(const char* effectFileName) {
	LPD3DXBUFFER errorLog;
	
	HRESULT hr = D3DXCreateEffectFromFileA(mDevice, effectFileName, 0, 0, 0, 0, &mCurrentEffect, &errorLog);

	if(hr != D3D_OK) {
		OutputDebugStringA(reinterpret_cast<LPCSTR>(errorLog->GetBufferPointer()));
		__debugbreak();
	}
}

void DirectX9::resetDevice(D3DPRESENT_PARAMETERS* newDevInfo) {
	HR(mDevice->Reset(newDevInfo));
	
	if(mRunningApp) {
		mRunningApp->onResetDevice();
	}
}

// space for quality must be reserved before
HRESULT DirectX9::checkMSAAModeSupport(D3DMULTISAMPLE_TYPE type, DWORD* quality) {
	return mD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mDevInfo.BackBufferFormat, TRUE, type, quality);
}

