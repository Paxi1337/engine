#ifndef __SHADOWAPP_H
#define __SHADOWAPP_H

#include "Window.h"
#include "timer.h"
#include "FreeCamera.h"
#include "drawable_tex2D.h"
#include "entity.h"

#include <vector>


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
	
	void setShaderData();
	void initShaderHandles();

	void createShadowMap();
	void changeDeviceInfo();

private:
	Window* mWindow;

	VertexbufferInfo* mCube;
	VertexbufferInfo* mGround;

	// shader handles
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
	D3DXHANDLE mNormalmapHandle;

	FreeCamera* mSceneCamera;
	FreeCamera* mLightCamera;

	SpotLight mSpotLight;
	Material mMaterial;

	IDirect3DTexture9* mWhiteTexture;
	IDirect3DTexture9* mYellowTextureBricks;
	IDirect3DTexture9* mNormalTextureFloor;
	IDirect3DTexture9* mNormalTextureBricks;
	
	DrawableTexture2D* mShadowMap;
	
	D3DXCOLOR mAmbient;

	bool mWireframeMode;

	
	Timer mT;
	double mTimeSinceElapsedTimeReset;

	// transforms
	D3DXMATRIX mWorld;
	D3DXMATRIX mViewScene;
	D3DXMATRIX mProjectionScene;

	D3DXMATRIX mViewLight;
	D3DXMATRIX mProjectionLight;

	Entity<CustomVertex3NormalUVTangentBinormal>* mSceneEntity;
	Entity<CustomVertex3NormalUVTangentBinormal>* mLightEntity;
	Entity<CustomVertex3NormalUVTangentBinormal>* mWatcherEntity;
	Entity<CustomVertex3NormalUVTangentBinormal>* mStatueEntity;
	

	std::vector<std::pair<std::string, DWORD*>>::const_iterator mMSAAModeIterator;

	VertexbufferInfo* mBuffer;
	VertexbufferInfo* mSelectedTriangle;

	std::vector<CustomTriangle<CustomVertex3NormalUVTangentBinormal>> mSceneTriangles;

	//void getMousePosition();
};

#endif