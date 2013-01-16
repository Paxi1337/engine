#ifndef SAMPLESHADER_H
#define SAMPLESHADER_H

#include "../includes/shader.h"

class SampleShader : public Shader {
public:
	SampleShader(LPDIRECT3DDEVICE9 dev);
	~SampleShader();

	void initShaderHandles();
	
	
	/*
	D3DXHANDLE mTechniqueHandle;
	D3DXHANDLE mTechniqueWireFrameHandle;
	D3DXHANDLE mShadowMapTechniqueHandle;

	D3DXHANDLE mWorldHandle;
	D3DXHANDLE mViewProjectionHandle;

	D3DXHANDLE mLightViewProjectionHandle;

	D3DXHANDLE mCameraPosHandle;

	D3DXHANDLE mLightHandle;
	D3DXHANDLE mMaterialHandle;

	D3DXHANDLE mTextureHandle;
	D3DXHANDLE mShadowTextureHandle;
	D3DXHANDLE mNormalmapHandle;*/
};

#endif