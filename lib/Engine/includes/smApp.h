#ifndef __SHADOWAPP_H
#define __SHADOWAPP_H

#include "Window.h"
#include "timer.h"
#include "FreeCamera.h"
#include "drawable_tex2D.h"

#include <vector>

struct Entity;

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

	// render techniques
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
	IDirect3DTexture9* mNormalTextureFloor;
	IDirect3DTexture9* mNormalTextureBricks;

	DrawableTexture2D* mShadowMap;
	
	D3DXCOLOR mAmbient;

	bool mWireframeMode;

	double mTimeSinceElapsedTimeReset;

	Timer mT;


	// transforms
	D3DXMATRIX mWorld;
	D3DXMATRIX mViewScene;
	D3DXMATRIX mProjectionScene;

	D3DXMATRIX mViewLight;
	D3DXMATRIX mProjectionLight;

	/*ID3DXMesh* mSceneMesh;
	std::vector<Material> mSceneMaterials;
	std::vector<IDirect3DTexture9*> mSceneTextures;
*/

	Entity* mSceneEntity;

	std::vector<std::pair<std::string, DWORD*>>::const_iterator mMSAAModeIterator;
};

#endif