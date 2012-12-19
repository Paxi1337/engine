#ifndef __SHADOWAPP_H
#define __SHADOWAPP_H

#include "Window.h"
#include "FreeCamera.h"
#include "drawable_tex2D.h"

class TestApp : public App {
public:
	explicit TestApp(Window* window);
	~TestApp();

	void onCreateDevice();
	void onResetDevice();
	void onReleaseDevice();
	void onUpdate();
	void onRender();
	void onLostDevice();
	void onKeyPressed(WPARAM keyCode);
	void onRawMouseInputReceived(RAWINPUT const& rawMouseInput);
	void onCustomUserFunction();
	void setWindow(Window* window);

	void initLight();
	void initCube();
	void initVertexDeclaration();
	
	void setShaderData();
	void initShaderHandles();

private:
	Window* mWindow;

	VertexbufferInfo* mBuffer;
	
	D3DXHANDLE mTechniqueHandle;
	D3DXHANDLE mTechniqueWireFrameHandle;

	D3DXHANDLE mWorldHandle;
	D3DXHANDLE mInverseTransposeHandle;
	D3DXHANDLE mViewProjectionHandle;

	D3DXHANDLE mCameraPosHandle;
	D3DXHANDLE mAmbientHandle;

	D3DXHANDLE mLightHandle;
	D3DXHANDLE mMaterialHandle;

	D3DXHANDLE mTextureHandle;

	FreeCamera* mSceneCamera;
	PointLight mDirLight;
	Material mMaterial;

	IDirect3DTexture9* mWhiteTexture;
	
	D3DXCOLOR mAmbient;

	bool mWireframeMode;

	double mTimeSinceElapsedTimeReset;
};

#endif