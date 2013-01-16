#include "../includes/smApp.h" 
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

float kCameraMovementSpeed=0.4f;
const float kCameraRotationSpeed=0.01f;
const float lightRotationSpeed=0.1f;

CustomTriangle<CustomVertex3NormalUVTangentBinormal>* triData;



static bool cubeInit = false;

TestApp::TestApp(Window* window) : mWindow(window), 
								   mCube(NULL),
								   mGround(NULL),
								   mTimeSinceElapsedTimeReset(0.0f), 
								   mSceneCamera(new FreeCamera(D3DXVECTOR3(0.0f, 10.0f, 0.0f))),
								   mLightCamera(new FreeCamera(D3DXVECTOR3(0.0f, 60.0f, 130.0f))),
								   mWireframeMode(false),
								   mSceneEntity(0),
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
	
	delete mSceneEntity;
	delete mLightEntity;
	delete mWatcherEntity;
	delete mStatueEntity;
}

void TestApp::onCreateDevice() {

	// set vertex delaration	
	mWindow->getRenderDevice()->setVertexDeclaration(CustomVertex3NormalUVTangentBinormal::decl);

	// load shader
	mWindow->getRenderDevice()->loadEffectFromFile("./shader/shadow.fx");
	
	// init shader handles
	initShaderHandles();
	
	// load textures
	HR(D3DXCreateTextureFromFileA(D3DDEVICE, "./texture/stone.jpg", &mWhiteTexture));
	HR(D3DXCreateTextureFromFileA(D3DDEVICE, "./texture/floor_nmap.bmp", &mNormalTextureBricks));
	HR(D3DXCreateTextureFromFileA(D3DDEVICE, "./texture/Watcher_N.tga", &mNormalTextureFloor));
	HR(D3DXCreateTextureFromFileA(D3DDEVICE, "./texture/yellow.jpg", &mYellowTextureBricks));
	// init meshes
	ID3DXMesh* box;
	ID3DXMesh* tempMesh = 0;
	ID3DXMesh* sceneMesh = 0;
	D3DXCreateSphere(D3DDEVICE, 3, 30, 30, &tempMesh, NULL);

	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;
	HR(CustomVertex3NormalUVTangentBinormal::decl->GetDeclaration(elems, &numElems));

	ID3DXMesh* clonedTempMesh = 0;
	HR(tempMesh->CloneMesh(D3DXMESH_MANAGED, elems, D3DDEVICE, &clonedTempMesh));
	HR(D3DXComputeTangentFrameEx(
	  clonedTempMesh, // Input mesh
	  D3DDECLUSAGE_TEXCOORD, 0, // Vertex element of input tex-coords.  
      D3DDECLUSAGE_BINORMAL, 0, // Vertex element to output binormal.
	  D3DDECLUSAGE_TANGENT, 0,  // Vertex element to output tangent.
      D3DDECLUSAGE_NORMAL, 0,   // Vertex element to output normal.
      0, // Options
      0, // Adjacency
	  0.01f, 0.25f, 0.01f, // Thresholds for handling errors
	  &box, // Output mesh
	  0));         // Vertex Remapping

	tempMesh->Release();
	clonedTempMesh->Release();

	// lock the vertex buffer
	CustomVertex3NormalUVTangentBinormal* pVerts;
    HR(box->LockVertexBuffer(0,(void**) &pVerts));
	
	D3DXVECTOR3* pVertices = new D3DXVECTOR3[box->GetNumVertices()];
	for (int i=0;i<box->GetNumVertices();i++) {
		pVertices[i] = pVerts[i].pos;
	}
	// determine extents
	D3DXVECTOR3 vMin,vMax;
	D3DXComputeBoundingBox(pVertices,box->GetNumVertices(),0,&vMin,&vMax);
	delete[] pVertices;

	// calculate center
	D3DXVECTOR3 vCent;
	vCent=(vMax+vMin)*0.5f;

	// loop through the vertices
	for (int i=0;i<box->GetNumVertices();i++) {

		// calculate normalized offset from center
		D3DXVECTOR3 v;
		v=pVerts->pos-vCent;
		D3DXVec3Normalize(&v,&v);

		// calculate texture coordinates
		pVerts->u=asinf(v.x)/D3DX_PI+0.5f;
		pVerts->v=asinf(v.y)/D3DX_PI+0.5f;

		// go to next vertex
		pVerts++;
	}

    HR(box->UnlockVertexBuffer());
    

	tempMesh = 0;
	sceneMesh = 0;
	std::vector<Material>* sceneMaterials = new std::vector<Material>();
	std::vector<IDirect3DTexture9*>* sceneTextures = new std::vector<IDirect3DTexture9*>();

	// load and convert scene mesh
	XManager::loadXFile(D3DDEVICE, "./model/testscene/BasicColumnScene.x", &tempMesh, sceneMaterials, sceneTextures);
	/*
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;
	HR(CustomVertex3NormalUVTangentBinormal::decl->GetDeclaration(elems, &numElems));*/

	clonedTempMesh = 0;
	HR(tempMesh->CloneMesh(D3DXMESH_MANAGED, elems, D3DDEVICE, &clonedTempMesh));
	
	HR(D3DXComputeTangentFrameEx(
	  clonedTempMesh, // Input mesh
	  D3DDECLUSAGE_TEXCOORD, 0, // Vertex element of input tex-coords.  
      D3DDECLUSAGE_BINORMAL, 0, // Vertex element to output binormal.
	  D3DDECLUSAGE_TANGENT, 0,  // Vertex element to output tangent.
      D3DDECLUSAGE_NORMAL, 0,   // Vertex element to output normal.
      0, // Options
      0, // Adjacency
	  0.01f, 0.25f, 0.01f, // Thresholds for handling errors
	  &sceneMesh, // Output mesh
	  0));         // Vertex Remapping

	tempMesh->Release();
	clonedTempMesh->Release();

	tempMesh = 0;
	std::vector<Material>* watcherMaterials = new std::vector<Material>();
	std::vector<IDirect3DTexture9*>* watcherTextures = new std::vector<IDirect3DTexture9*>();
	ID3DXMesh* watcher;
	XManager::loadXFile(D3DDEVICE, "./model/watcher/Watcher.x", &tempMesh, watcherMaterials, watcherTextures);


	
	numElems = 0;
	HR(CustomVertex3NormalUVTangentBinormal::decl->GetDeclaration(elems, &numElems));


	HR(tempMesh->CloneMesh(D3DXMESH_MANAGED, elems, D3DDEVICE, &watcher));

	std::vector<Material>* statueMaterials = new std::vector<Material>();
	std::vector<IDirect3DTexture9*>* statueTextures = new std::vector<IDirect3DTexture9*>();
	ID3DXMesh* statue;
	XManager::loadXFile(D3DDEVICE, "./model/statue/statueC.x", &statue, statueMaterials, statueTextures);

	mSceneEntity = new Entity<CustomVertex3NormalUVTangentBinormal>(1, sceneMesh, sceneMaterials, sceneTextures); 
	mLightEntity = new Entity<CustomVertex3NormalUVTangentBinormal>(2, box);
	mWatcherEntity = new Entity<CustomVertex3NormalUVTangentBinormal>(3, watcher, watcherMaterials, watcherTextures);
	mStatueEntity = new Entity<CustomVertex3NormalUVTangentBinormal>(4, statue, statueMaterials, statueTextures);
	triData = mSceneEntity->getTriangles();

	
	
	

	//char buffer[55];

	//for(int i = 0; i < mSceneEntity->getTriangleCount(); ++i) {
	//	mSceneTriangles.push_back(triData[i]);
	//}

	//KdTree<CustomVertex3NormalUVTangentBinormal> tree(mSceneTriangles);

	//std::vector<D3DXVECTOR3>& bb = tree.getBoundingBoxLines();

	//CustomVertex3NormalUVTangentBinormal* data = new CustomVertex3NormalUVTangentBinormal[bb.size()];
	//
	//for(int i = 0; i < bb.size(); ++i) {
	//	data[i].pos = bb.at(i);
	//}

	//mBuffer = mWindow->getRenderDevice()->createVertexBuffer(bb.size(),CustomVertex3NormalUVTangentBinormal::format, "debuglines");
	//mWindow->getRenderDevice()->setVertexBufferData("debuglines", data);
	//
	//CustomVertex3NormalUVTangentBinormal tri[3];
	//tri[0].pos = triData[1].mP1->pos*150;
	//tri[1].pos = triData[1].mP2->pos*150;
	//tri[2].pos = triData[1].mP3->pos*150;

	////tri[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////tri[1].pos = D3DXVECTOR3(5.0f, 5.0f, 0.0f);
	////tri[2].pos = D3DXVECTOR3(10.0f, 0.0f, 0.0f);

	//mSelectedTriangle = mWindow->getRenderDevice()->createVertexBuffer(3,CustomVertex3NormalUVTangentBinormal::format, "st");
	//mWindow->getRenderDevice()->setVertexBufferData("st", tri);

	// init light
	initLight();

	// texture for drawing the shadowmap
	D3DVIEWPORT9 vp = {0, 0, 1024, 1024, 0.0f, 1.0f};
	mShadowMap = new DrawableTexture2D(D3DDEVICE, 1024, 1024, 1, D3DFMT_R32F, true, D3DFMT_D24X8, vp, false);

	
	
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

		for(UINT i = 0; i < mSceneEntity->mMtrls->size()-1; ++i) {
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mMaterialHandle, &mSceneEntity->mMtrls->at(i), sizeof(Material)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mSceneEntity->mTexs->at(i)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());

			mSceneEntity->mMesh->DrawSubset(i);
		}


		


		D3DXMatrixScaling(&mWorld, 0.2,0.2,0.2);
		D3DXMATRIX rot;
		D3DXMATRIX trans;
		//D3DXMatrixRotationX(&rot, D3DXToRadian(90));
		
		D3DXMatrixRotationYawPitchRoll(&rot,0,D3DXToRadian(90),0);
		D3DXMatrixTranslation(&trans, 10, 0, 20);
		HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(rot*mWorld*trans), sizeof(D3DXMATRIX)));
		HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mNormalmapHandle, mNormalTextureFloor));
		for(UINT i = 0; i < mWatcherEntity->mMtrls->size(); ++i) {
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mMaterialHandle, &mWatcherEntity->mMtrls->at(i), sizeof(Material)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mWatcherEntity->mTexs->at(i)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());

			mWatcherEntity->mMesh->DrawSubset(i);
		}
		
		D3DXMatrixIdentity(&trans);
		D3DXMatrixTranslation(&trans, 45, 0, 45);
		D3DXMatrixRotationYawPitchRoll(&rot,0,D3DXToRadian(90),D3DXToRadian(90));

		HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
		for(UINT i = 0; i < mStatueEntity->mMtrls->size(); ++i) {
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mMaterialHandle, &mStatueEntity->mMtrls->at(i), sizeof(Material)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mStatueEntity->mTexs->at(i)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());

			mStatueEntity->mMesh->DrawSubset(i);
		}

		D3DXMatrixIdentity(&trans);
		D3DXMatrixTranslation(&trans, 45, 0, 45-20);

		HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
		for(UINT i = 0; i < mStatueEntity->mMtrls->size(); ++i) {
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mMaterialHandle, &mStatueEntity->mMtrls->at(i), sizeof(Material)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mStatueEntity->mTexs->at(i)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());

			mStatueEntity->mMesh->DrawSubset(i);
		}

		D3DXMatrixIdentity(&trans);
		D3DXMatrixTranslation(&trans, 45, 0, 45-40);

		HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
		for(UINT i = 0; i < mStatueEntity->mMtrls->size(); ++i) {
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mMaterialHandle, &mStatueEntity->mMtrls->at(i), sizeof(Material)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mStatueEntity->mTexs->at(i)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());

			mStatueEntity->mMesh->DrawSubset(i);
		}

		D3DXMatrixIdentity(&trans);
		D3DXMatrixTranslation(&trans, 45, 0, 45-60);

		HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
		for(UINT i = 0; i < mStatueEntity->mMtrls->size(); ++i) {
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mMaterialHandle, &mStatueEntity->mMtrls->at(i), sizeof(Material)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mStatueEntity->mTexs->at(i)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());

			mStatueEntity->mMesh->DrawSubset(i);
		}

		D3DXMatrixIdentity(&trans);
		D3DXMatrixTranslation(&trans, 45, 0, 45-80);

		HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
		for(UINT i = 0; i < mStatueEntity->mMtrls->size(); ++i) {
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mMaterialHandle, &mStatueEntity->mMtrls->at(i), sizeof(Material)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mStatueEntity->mTexs->at(i)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());

			mStatueEntity->mMesh->DrawSubset(i);
		}

		
		D3DXMATRIX translateLight;
		D3DXMatrixIdentity(&mWorld);
		
		D3DXMATRIX translate;
		const D3DXVECTOR3& lightPos = mLightCamera->getPosition();
		HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mYellowTextureBricks));
		D3DXMatrixTranslation(&mWorld, lightPos.x, lightPos.y, lightPos.z);
		HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, mWorld, sizeof(D3DXMATRIX)));
		HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());
		
		
		mLightEntity->mMesh->DrawSubset(0);

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
	// move light up
	case VK_NUMPAD7:
		lightPos = mLightCamera->getPosition();
		lightPos.y += kCameraMovementSpeed;
		mLightCamera->setPosition(lightPos);
		break;
	// move light down
	case VK_NUMPAD9:
		lightPos = mLightCamera->getPosition();
		lightPos.y -= kCameraMovementSpeed;
		mLightCamera->setPosition(lightPos);
		break;
	// move light forward
	case VK_NUMPAD8:
		lightPos = mLightCamera->getPosition();
		lightPos.z += kCameraMovementSpeed;
		mLightCamera->setPosition(lightPos);
		break;
	// move light back
	case VK_NUMPAD2:
		lightPos = mLightCamera->getPosition();
		lightPos.z -= kCameraMovementSpeed;
		mLightCamera->setPosition(lightPos);
		break;
	// move light left
	case VK_NUMPAD4:
		lightPos = mLightCamera->getPosition();
		lightPos.x -= kCameraMovementSpeed;
		mLightCamera->setPosition(lightPos);
		break;
	// move light right
	case VK_NUMPAD6:
		lightPos = mLightCamera->getPosition();
		lightPos.x += kCameraMovementSpeed;
		mLightCamera->setPosition(lightPos);
		break;

	case VK_ADD:
		kCameraMovementSpeed += kCameraMovementSpeed;

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
	mSceneCamera->calculateProjectionMatrix(mProjectionScene, 1.0f, 250.0f);

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

	for(UINT i = 0; i < mSceneEntity->mMtrls->size()-1; ++i) {
			mSceneEntity->mMesh->DrawSubset(i);
	}


	D3DXMatrixScaling(&mWorld, 0.2,0.2,0.2);
	D3DXMATRIX rot;
	D3DXMATRIX trans;
	D3DXMatrixRotationX(&rot, D3DXToRadian(90));
	D3DXMatrixTranslation(&trans, 10, -10, 20);
	HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
	HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());
	for(UINT i = 0; i < mWatcherEntity->mMtrls->size(); ++i) {
			mWatcherEntity->mMesh->DrawSubset(i);
	}
	
	D3DXMatrixIdentity(&trans);
	D3DXMatrixTranslation(&trans, 45, 0, 45);
	D3DXMatrixRotationYawPitchRoll(&rot,0,D3DXToRadian(90),D3DXToRadian(90));

	HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
	HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());
	for(UINT i = 0; i < mStatueEntity->mMtrls->size(); ++i) {
		mStatueEntity->mMesh->DrawSubset(i);
	}

	D3DXMatrixIdentity(&trans);
	D3DXMatrixTranslation(&trans, 45, 0, 45-20);

	HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
	HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());
	for(UINT i = 0; i < mStatueEntity->mMtrls->size(); ++i) {
		mStatueEntity->mMesh->DrawSubset(i);
	}

	D3DXMatrixIdentity(&trans);
	D3DXMatrixTranslation(&trans, 45, 0, 45-40);
	HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
	HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());
	for(UINT i = 0; i < mStatueEntity->mMtrls->size(); ++i) {
		mStatueEntity->mMesh->DrawSubset(i);
	}

	D3DXMatrixIdentity(&trans);
	D3DXMatrixTranslation(&trans, 45, 0, 45-60);
	HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
	HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());
	for(UINT i = 0; i < mStatueEntity->mMtrls->size(); ++i) {mStatueEntity->mMesh->DrawSubset(i);
		mStatueEntity->mMesh->DrawSubset(i);
	}

	D3DXMatrixIdentity(&trans);
	D3DXMatrixTranslation(&trans, 45, 0, 45-80);
	HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mWorldHandle, &(mWorld*rot*trans), sizeof(D3DXMATRIX)));
	HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());
	for(UINT i = 0; i < mStatueEntity->mMtrls->size(); ++i) {mStatueEntity->mMesh->DrawSubset(i);
		mStatueEntity->mMesh->DrawSubset(i);
	}
	

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
	
	delete mSceneEntity;
	mSceneEntity = 0;
	delete mShadowMap;
	mShadowMap = 0;

	mWindow->getRenderDevice()->resetDevice(currentDevInfo);
}

//void TestApp::getMousePosition() {
//	//char buffer[255];
//	
//	D3DXMATRIX matInv;
//	
//
//	D3DXMatrixInverse(&matInv, NULL, &(mWorld * mViewScene));
//
//	POINT mousePos;
//	GetCursorPos(&mousePos);
//
//	//sprintf(buffer, "x: %d y: %d\n", mousePos.x, mousePos.y);
//
//	HWND windowHandle = mWindow->getWindowHandle();
//
//	ScreenToClient(windowHandle, &mousePos);
//
//	//sprintf(buffer, "x: %d y: %d\n", mousePos.x, mousePos.y);
//
//	//OutputDebugStringA(buffer);
//
//	D3DXVECTOR3 worldSpaceCoordinates;
//	D3DXVECTOR3 inputVec1(mousePos.x, mousePos.y, 0.0f);
//	D3DXVECTOR3 inputVec2(mousePos.x, mousePos.y, 1.0f);
//
//	/*D3DVIEWPORT9 viewPort;
//	mWindow->getRenderDevice()->getD3D9Device()->GetViewport(&viewPort);
//
//	D3DXVec3Unproject(&worldSpaceCoordinates, &inputVec1, &viewPort, &mProjectionScene, &mViewScene, &mWorld);
//	D3DXVec3Unproject(&worldSpaceCoordinates, &inputVec2, &viewPort, &mProjectionScene, &mViewScene, &mWorld);*/
//
//	float xAngle = (((2.0f * mousePos.x) / 800) - 1.0f) / mProjectionScene(0, 0);
//	float yAngle = (((-2.0f * mousePos.y) / 600) + 1.0f) / mProjectionScene(1, 1);
//
//
//	//D3DXVECTOR3 origin(mousePos.x, mousePos.y, 0.0f);
//	D3DXVECTOR3 origin(mSceneCamera->getPosition());
//	D3DXVec3Normalize(&origin, &origin);
//	D3DXVECTOR3 direction(xAngle, yAngle, 1.0f);
//
//	D3DXVec3TransformCoord(&origin, &origin, &matInv);
//	D3DXVec3TransformNormal(&direction, &direction, &matInv);
//	D3DXVec3Normalize(&direction, &direction);
//
//	bool hit = false;
//
//	for (int i = 0; i < mSceneTriangles.size(); ++i) {
//		hit = D3DXIntersectTri(&mSceneTriangles[i].mP1->pos, &mSceneTriangles[i].mP2->pos, &mSceneTriangles[i].mP3->pos, &origin, &direction, NULL, NULL, NULL);
//		if(hit) {
//			OutputDebugStringA("HIT!!!\n");
//
//			CustomVertex3NormalUVTangentBinormal tri[3];
//			tri[0].pos = triData[i].mP1->pos;
//			tri[1].pos = triData[i].mP2->pos;
//			tri[2].pos = triData[i].mP3->pos;
//
//			//tri[0].pos.z--;
//			//tri[1].pos.z--;
//			//tri[2].pos.z--;
//
//			mWindow->getRenderDevice()->setVertexBufferData("st", tri);
//
//			//triData[i].mP1->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//			////triData[i].mP1->v = 0.0f;
//
//			//triData[i].mP2->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//			////triData[i].mP2->v = 0.0f;
//
//			//triData[i].mP3->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//			////triData[i].mP3->v = 0.0f;
//		}
//	}
//}