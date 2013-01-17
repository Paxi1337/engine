#include "../includes/shader.h"


void Shader::loadEffectFile(const char* effectFileName) {
	if(mDev) {
		LPD3DXBUFFER errorLog;
		HRESULT hr = D3DXCreateEffectFromFileA(mDev, effectFileName, 0, 0, 0, 0, &mEffect, &errorLog);

		if(hr != D3D_OK) {
			OutputDebugStringA(reinterpret_cast<LPCSTR>(errorLog->GetBufferPointer()));
			__debugbreak();
		}
	}
	else {
		OutputDebugStringA("no valid device, you may forgot to initialize device?\n");
		__debugbreak();
	}
}
