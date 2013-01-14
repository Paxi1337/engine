#include "../includes/algoApp.h" 
#include "../includes/freeCamera.h"
#include "../includes/xManager.h"
#include "../includes/structdeclarations.h"
#include "../includes/scenemanager.h"

#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

#define D3DDEVICE mWindow->getRenderDevice()->getD3D9Device()
#define SAVEDELETE(x) { if(x) delete x; }

const float kCameraMovementSpeed=0.4f;
const float kCameraRotationSpeed=0.01f;
const float lightRotationSpeed=0.1f;

CustomTriangle<CustomVertex3Normal>* triData;

ID3DXMesh* box;

TestApp::TestApp(Window* window) : mWindow(window), 
								   mCube(NULL),
								   mGround(NULL),
								   mTimeSinceElapsedTimeReset(0.0f), 
								   mSceneCamera(new FreeCamera(D3DXVECTOR3(0.0f, 5.0f, -50.0f))),
								   mLightCamera(new FreeCamera(D3DXVECTOR3(100.0f, 30.0f, 0.0f))),
								   mWireframeMode(false),
								   mBoxEntity(0),
								   mBuffer(0),
								   mSelectedTriangle(0)
{
	mMSAAModeIterator = mWindow->getRenderDevice()->getMSAAModes().begin();
}

TestApp::~TestApp() {
	SAVEDELETE(mShadowMap);
	SAVEDELETE(mSceneCamera);
	SAVEDELETE(mLightCamera);
	
	mWhiteTexture->Release();
	mNormalTextureBricks->Release();
	mNormalTextureFloor->Release();
	
	delete mBoxEntity;
}

void TestApp::onCreateDevice() {

	// set vertex delaration	
	mWindow->getRenderDevice()->setVertexDeclaration(CustomVertex3Normal::decl);

	// load shader
	mWindow->getRenderDevice()->loadEffectFromFile("./shader/algo.fx");

	// load textures
	HR(D3DXCreateTextureFromFileA(D3DDEVICE, "./texture/stone.jpg", &mWhiteTexture));
	HR(D3DXCreateTextureFromFileA(D3DDEVICE, "./texture/white.jpg", &mNormalTextureBricks));
	HR(D3DXCreateTextureFromFileA(D3DDEVICE, "./texture/floor_nmap.bmp", &mNormalTextureFloor));


	D3DXCreateBox(D3DDEVICE, 3,3,3,&box,NULL);

	std::vector<Material>* mtrls;
	std::vector<IDirect3DTexture9*>* texs;

	mBoxEntity = new Entity<CustomVertex3Normal>(1, box); 
	
	triData = mBoxEntity->getTriangles();

	

	

	char buffer[55];

	for(int i = 0; i < mBoxEntity->getTriangleCount(); ++i) {
		mSceneTriangles.push_back(triData[i]);
		
		/*if(i > mSceneEntity->getTriangleCount() / 2) {
			 triData[i].mP1->pos.x = triData[i].mP1->pos.y = triData[i].mP1->pos.x = 0;
		}*/
	}

	
	
	KdTree<CustomVertex3Normal> tree(mSceneTriangles);

	std::vector<D3DXVECTOR3>& bb = tree.getBoundingBoxLines();

	CustomVertex3Normal* data = new CustomVertex3Normal[bb.size()];
	
	for(int i = 0; i < bb.size(); ++i) {
		data[i].pos = bb.at(i);
	}

	mBuffer = mWindow->getRenderDevice()->createVertexBuffer(bb.size(),CustomVertex3Normal::format, "debuglines");
	mWindow->getRenderDevice()->setVertexBufferData("debuglines", data);
	
	CustomVertex3Normal tri[3];
	tri[0].pos = triData[1].mP1->pos*150;
	tri[1].pos = triData[1].mP2->pos*150;
	tri[2].pos = triData[1].mP3->pos*150;

	//tri[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//tri[1].pos = D3DXVECTOR3(5.0f, 5.0f, 0.0f);
	//tri[2].pos = D3DXVECTOR3(10.0f, 0.0f, 0.0f);

	mSelectedTriangle = mWindow->getRenderDevice()->createVertexBuffer(3,CustomVertex3Normal::format, "st");
	mWindow->getRenderDevice()->setVertexBufferData("st", tri);

	// init light
	initLight();
	// texture for drawing the shadowmap
	D3DVIEWPORT9 vp = {0, 0, 1024, 1024, 0.0f, 1.0f};
	mShadowMap = new DrawableTexture2D(D3DDEVICE, 1024, 1024, 1, D3DFMT_R32F, true, D3DFMT_D24X8, vp, false);

	// init shader handles
	initShaderHandles();
	
}

void TestApp::onResetDevice() {
	onCreateDevice();
}

void TestApp::onReleaseDevice() {
	//delete
}

void TestApp::onUpdate() {
	if(mWindow) {
		setShaderData();
	}
}

void TestApp::onRender() {
	if(mWindow) {
		mT.start();

		mWindow->getRenderDevice()->getCurrentEffect()->Begin(NULL,NULL);
		mWindow->getRenderDevice()->getCurrentEffect()->BeginPass(0);
		
		mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mNormalTextureBricks);
		mWindow->getRenderDevice()->renderVertexbuffer(D3DPT_LINELIST, "debuglines");
		mWindow->getRenderDevice()->renderVertexbuffer(D3DPT_TRIANGLELIST, "st");

		//mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mWhiteTexture);
		
		box->DrawSubset(0);

		mWindow->getRenderDevice()->getCurrentEffect()->EndPass();
		mWindow->getRenderDevice()->getCurrentEffect()->End();

		mT.stop();
		float elapsedTime = mT.getElapsedTimeInMilliSec();
		mTimeSinceElapsedTimeReset = mTimeSinceElapsedTimeReset + elapsedTime;
	}
}

void TestApp::onLostDevice() {

}

void TestApp::setWindow(Window* window) {
	mWindow = window;
}

void TestApp::onKeyPressed(WPARAM keyCode) {
	D3DXVECTOR3 lightPos;

	switch(keyCode) {	
	case 'W':
		mSceneCamera->moveForward(kCameraMovementSpeed);
		break;
	case 'S':
		mSceneCamera->moveForward(-kCameraMovementSpeed);
		break;
	case 'A':
		mSceneCamera->moveRight(-kCameraMovementSpeed);
		break;
	case 'D':
		mSceneCamera->moveRight(kCameraMovementSpeed);
		break;
	case 'Q':
		mWireframeMode ? mWireframeMode = false : mWireframeMode = true;
		break;
	case VK_UP:
		lightPos = mLightCamera->getPosition();
		lightPos.x += kCameraMovementSpeed;
		mLightCamera->setPosition(lightPos);
		break;
	case VK_SHIFT & VK_UP:
		lightPos = mLightCamera->getPosition();
		lightPos.x -= kCameraMovementSpeed;
		mLightCamera->setPosition(lightPos);
		break;
	case VK_DOWN:
		changeDeviceInfo();
		break;
	case VK_LEFT:
		
		break;
	case VK_RIGHT:
		
		break;

	default: break;

	}
}

void TestApp::onRawMouseInputReceived(RAWINPUT const& rawMouseInput) {
	static BOOL mouseDown;
	char buffer[25];
	POINT mousePos;

	if(rawMouseInput.data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN)
		mouseDown = true;
	if(rawMouseInput.data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP)
		mouseDown = false;

	if(mouseDown) {
		float lastX = rawMouseInput.data.mouse.lLastX;
		float lastY = rawMouseInput.data.mouse.lLastY;

		mSceneCamera->pitch(kCameraRotationSpeed*lastY);
		mSceneCamera->yaw(kCameraRotationSpeed*lastX);

		getMousePosition();

		
	}
}

void TestApp::initLight() {
	// only do this now because no of no init list
	//memset(&mDirLight,'\0', sizeof(DirectionalLight));
	mSpotLight.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	mSpotLight.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mSpotLight.spec = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	mSpotLight.spotPower = 0.0f;
	//mSpotLight.posW = D3DXVECTOR3(2.0f, 2.0f, 2.0f);

	// only do this now because no of no init list
	//memset(&mMaterial,'\0', sizeof(Material));
	mMaterial.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.spec = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.specPower = 20.0f;

	mAmbient.r = 0.2f;
	mAmbient.g = 0.2f;
	mAmbient.b = 0.2f;
	mAmbient.a = 1.0f;
}


void TestApp::onCustomUserFunction() {
	D3DXMatrixIdentity(&mWorld);
	mLightCamera->calculateViewMatrix(mViewLight);
	
	D3DXVECTOR3 lookAt(0.0f,0.0f,0.0f);
	D3DXVECTOR3 up(0.0f,1.0f,0.0f);

	D3DXMatrixLookAtLH(&mViewLight, &(mLightCamera->getPosition()), &lookAt, &up);
	
	float lightFov = D3DX_PI*0.25f;
	// aspect = 1 because aspect is defined via surface height/width = 1
	mLightCamera->calculateProjectionMatrixCustom(mProjectionLight, lightFov, 1, 1, 200);

	mWindow->getRenderDevice()->getCurrentEffect()->SetTechnique(mShadowMapTechniqueHandle);
	mWindow->getRenderDevice()->getCurrentEffect()->SetMatrix(mWorldHandle, &mWorld);
	mWindow->getRenderDevice()->getCurrentEffect()->SetMatrix(mLightViewProjectionHandle, &(mViewLight*mProjectionLight));

	createShadowMap();
}


void TestApp::initShaderHandles() {
	// get handle for rendering scene
	mTechniqueHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetTechniqueByName("LightShadowTech");
	ASSERT(mTechniqueHandle != 0, "scene technique returns NULL pointer");

	/*mTechniqueWireFrameHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetTechniqueByName("NormalMappingDirectionalLightingWireframe");
	ASSERT(mTechniqueWireFrameHandle != 0, "NormalMappingDirectionalLightingWireframe technique returns NULL pointer");*/

	mShadowMapTechniqueHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetTechniqueByName("BuildShadowMapTech");
	ASSERT(mShadowMapTechniqueHandle != 0, "shadow map technique == NULL");

	// initializing shader handles for transforms
	mWorldHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetParameterByName(0, "gWorld");
	ASSERT(mWorldHandle != 0, "worldMatrix == NULL");

	mLightViewProjectionHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetParameterByName(0, "gLightVP");
	ASSERT(mLightViewProjectionHandle != 0, "mLightViewProjectionHandle == NULL");

	mViewProjectionHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetParameterByName(0, "gVP");
	ASSERT(mViewProjectionHandle != 0, "projMatrix == NULL");
	
	// initializing shader handle to camera pos
	mCameraPosHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetParameterByName(0, "gEyePosW");
	ASSERT(mCameraPosHandle != 0, "mCameraPosHandle == NULL");

	// light
	mLightHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetParameterByName(0, "gLight");
	ASSERT(mLightHandle != 0, "mLightHandle == NULL");
	
	// material
	mMaterialHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetParameterByName(0, "gMaterial");
	ASSERT(mMaterialHandle != 0, "mMaterialHandle == NULL");
	
	// texture
	mTextureHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetParameterByName(0, "gTex");
	ASSERT(mTextureHandle != 0, "mTextureHandle == NULL");

	// shadow texture
	mShadowTextureHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetParameterByName(0, "gShadowMap");
	ASSERT(mShadowTextureHandle != 0, "mShadowTextureHandle == NULL");

	// normal mapping
	mNormalmapHandle = mWindow->getRenderDevice()->getCurrentEffect()->GetParameterByName(0, "gNormalMap");
	ASSERT(mNormalmapHandle != 0, "mShadowTextureHandle == NULL");

}

void TestApp::setShaderData() {
	mSceneCamera->calculateViewMatrix(mViewScene);
	mSceneCamera->calculateProjectionMatrix(mProjectionScene);

	D3DXMatrixIdentity(&mWorld);
	//D3DXMatrixScaling(&mWorld, 0.1f, 0.1f, 0.1f);
	mWindow->getRenderDevice()->getCurrentEffect()->SetMatrix(mWorldHandle, &mWorld);

	D3DXVECTOR3 lightDirW = mLightCamera->getLookAt() - mLightCamera->getPosition();
	D3DXVec3Normalize(&lightDirW, &lightDirW);
	mSpotLight.posW = mLightCamera->getPosition();
	mSpotLight.dirW = lightDirW;

	// set technique
	if(!mWireframeMode)
		mWindow->getRenderDevice()->getCurrentEffect()->SetTechnique(mTechniqueHandle);
	else
		mWindow->getRenderDevice()->getCurrentEffect()->SetTechnique(mTechniqueWireFrameHandle);

	// Set the matrices for the shader.
	mWindow->getRenderDevice()->getCurrentEffect()->SetMatrix(mWorldHandle, &mWorld);

	mWindow->getRenderDevice()->getCurrentEffect()->SetMatrix(mViewProjectionHandle, &(mViewScene * mProjectionScene));

	// Set the camera position.
	mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mCameraPosHandle, &mSceneCamera->getPosition(), sizeof(D3DXVECTOR3));

	//// Set the scene global ambient term.
	//mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mAmbientHandle, &mAmbient, sizeof(D3DXCOLOR));

	// Set the lighting parameters for the shader.
	mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mLightHandle, &mSpotLight, sizeof(SpotLight));

	// Set the material parameters for the shader.
	mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mMaterialHandle, &mMaterial, sizeof(Material));

	// Bind the textures to the shader.
	mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mWhiteTexture);
	mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mShadowTextureHandle, mShadowMap->d3dTex());
	mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mNormalmapHandle, mNormalTextureBricks);
}


void TestApp::createShadowMap() {
	D3DDEVICE->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(100, 149, 237), 1.0f, 0);
	mShadowMap->beginScene();
	
	D3DDEVICE->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	UINT passCount = 0;
	mWindow->getRenderDevice()->getCurrentEffect()->Begin(&passCount,NULL);
	mWindow->getRenderDevice()->getCurrentEffect()->BeginPass(0);

	
	//mBoxEntity->mMesh->DrawSubset(0);
	

	mWindow->getRenderDevice()->getCurrentEffect()->EndPass();
	mWindow->getRenderDevice()->getCurrentEffect()->End();

	mShadowMap->endScene();
	
	//mShadowMap->dumpToFileA("shadowmap.dds");
}


void TestApp::changeDeviceInfo() {
	D3DPRESENT_PARAMETERS* currentDevInfo = mWindow->getRenderDevice()->getDeviceInfo();
	
	if(mMSAAModeIterator == mWindow->getRenderDevice()->getMSAAModes().end() - 1) {
		mMSAAModeIterator = mWindow->getRenderDevice()->getMSAAModes().begin();
	}
	else {
		++mMSAAModeIterator;
	}

	DWORD* val = mMSAAModeIterator->second;

	currentDevInfo->MultiSampleType = static_cast<D3DMULTISAMPLE_TYPE>(val[0]);
	currentDevInfo->MultiSampleQuality = val[1];
	
	// release all ressources otherwise the device reset call with break with INVALIDCALL error
	mWindow->getRenderDevice()->getCurrentEffect()->Release();
	mWhiteTexture->Release();
	mNormalTextureBricks->Release();
	mNormalTextureFloor->Release();
	
	delete mBoxEntity;
	mBoxEntity = 0;
	delete mShadowMap;
	mShadowMap = 0;

	mWindow->getRenderDevice()->resetDevice(currentDevInfo);
}

void TestApp::getMousePosition() {
	//char buffer[255];
	
	D3DXMATRIX matInv;
	

	D3DXMatrixInverse(&matInv, NULL, &(mWorld * mViewScene));

	POINT mousePos;
	GetCursorPos(&mousePos);

	//sprintf(buffer, "x: %d y: %d\n", mousePos.x, mousePos.y);

	HWND windowHandle = mWindow->getWindowHandle();

	ScreenToClient(windowHandle, &mousePos);

	//sprintf(buffer, "x: %d y: %d\n", mousePos.x, mousePos.y);

	//OutputDebugStringA(buffer);

	D3DXVECTOR3 worldSpaceCoordinates;
	D3DXVECTOR3 inputVec1(mousePos.x, mousePos.y, 0.0f);
	D3DXVECTOR3 inputVec2(mousePos.x, mousePos.y, 1.0f);

	/*D3DVIEWPORT9 viewPort;
	mWindow->getRenderDevice()->getD3D9Device()->GetViewport(&viewPort);

	D3DXVec3Unproject(&worldSpaceCoordinates, &inputVec1, &viewPort, &mProjectionScene, &mViewScene, &mWorld);
	D3DXVec3Unproject(&worldSpaceCoordinates, &inputVec2, &viewPort, &mProjectionScene, &mViewScene, &mWorld);*/


	float xAngle = (((2.0f * mousePos.x) / 800) - 1.0f) / mProjectionScene(0, 0);
	float yAngle = (((-2.0f * mousePos.y) / 600) + 1.0f) / mProjectionScene(1, 1);


	//D3DXVECTOR3 origin(mousePos.x, mousePos.y, 0.0f);
	D3DXVECTOR3 origin(0.0f,0.0f,0.0f);
	D3DXVec3Normalize(&origin, &origin);
	D3DXVECTOR3 direction(xAngle, yAngle, 1.0f);

	D3DXVec3TransformCoord(&origin, &origin, &matInv);
	D3DXVec3TransformNormal(&direction, &direction, &matInv);
	D3DXVec3Normalize(&direction, &direction);

	bool hit = false;

	for (int i = 0; i < mSceneTriangles.size(); ++i) {
		hit = D3DXIntersectTri(&mSceneTriangles[i].mP1->pos, &mSceneTriangles[i].mP2->pos, &mSceneTriangles[i].mP3->pos, &origin, &direction, NULL, NULL, NULL);
		if(hit) {
			OutputDebugStringA("HIT!!!\n");

			CustomVertex3Normal tri[3];
			tri[0].pos = triData[i].mP1->pos;
			tri[1].pos = triData[i].mP2->pos;
			tri[2].pos = triData[i].mP3->pos;

			tri[0].pos.x++;
			tri[1].pos.x++;
			tri[2].pos.x++;

			mWindow->getRenderDevice()->setVertexBufferData("st", tri);

			//triData[i].mP1->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			////triData[i].mP1->v = 0.0f;

			//triData[i].mP2->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			////triData[i].mP2->v = 0.0f;

			//triData[i].mP3->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			////triData[i].mP3->v = 0.0f;
		}
	}
}