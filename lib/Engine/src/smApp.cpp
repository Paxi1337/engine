#include "../includes/smApp.h" 
#include "../includes/freeCamera.h"
#include "../includes/xManager.h"

#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

#define D3DDEVICE mWindow->getRenderDevice()->getD3D9Device()
#define SAVEDELETE(x) { if(x) delete x; }

const float kCameraMovementSpeed=0.4f;
const float kCameraRotationSpeed=0.01f;
const float lightRotationSpeed=0.1f;

ID3DXMesh* teapot;

CustomVertex3NormalUVTangent verticesCube[] = {
	// -Z face
	{ D3DXVECTOR3(-1.0f,  1.0f, -1.0f),       D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f,  1.0f, -1.0f),       D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f, -1.0f, -1.0f),       D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f, -1.0f, -1.0f),      D3DXVECTOR3(0.0f, 0.0f, -1.0f),  1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f),       D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f,  1.0f, -1.0f),       D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f , D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)},

	// +Z face
	{ D3DXVECTOR3(1.0f,  1.0f,  1.0f),     D3DXVECTOR3(0.0f, 0.0f, 1.0f),  0.0f, 0.0f , D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)},
	{ D3DXVECTOR3(-1.0f,  1.0f,  1.0f),     D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f , D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)},
	{ D3DXVECTOR3(-1.0f, -1.0f,  1.0f),     D3DXVECTOR3(0.0f, 0.0f, 1.0f),  1.0f, 1.0f , D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)},
	{ D3DXVECTOR3(-1.0f, -1.0f,  1.0f),      D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f, -1.0f,  1.0f),     D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f , D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)},
	{ D3DXVECTOR3(1.0f,  1.0f,  1.0f),    D3DXVECTOR3(0.0f, 0.0f, 1.0f),  0.0f, 0.0f , D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f)},

	// -Y face
	{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f),     D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f, -1.0f, -1.0f),    D3DXVECTOR3(0.0f, -1.0f, 0.0f),  1.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f, -1.0f,  1.0f),      D3DXVECTOR3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f, -1.0f,  1.0f),    D3DXVECTOR3(0.0f, -1.0f, 0.0f),  1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f, -1.0f,  1.0f),     D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f),    D3DXVECTOR3(0.0f, -1.0f, 0.0f),  0.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },

	// +Y face
	{ D3DXVECTOR3(-1.0f,  1.0f,  1.0f),      D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f,  1.0f,  1.0f),     D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f,  1.0f, -1.0f),      D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f,  1.0f, -1.0f),      D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f,  1.0f, -1.0f),     D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f,  1.0f,  1.0f),      D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },

	// -X face
	{ D3DXVECTOR3(-1.0f,  1.0f,  1.0f),      D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f,  1.0f, -1.0f),      D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f),      D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f),      D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f, -1.0f,  1.0f),     D3DXVECTOR3(-1.0f, 0.0f, 0.0f),  0.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-1.0f,  1.0f,  1.0f),      D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },

	// +X face
	{ D3DXVECTOR3(1.0f,  1.0f, -1.0f),      D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f,  1.0f,  1.0f),      D3DXVECTOR3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f, -1.0f,  1.0f),      D3DXVECTOR3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f, -1.0f,  1.0f),      D3DXVECTOR3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f, -1.0f, -1.0f),     D3DXVECTOR3(1.0f, 0.0f, 0.0f),  0.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(1.0f,  1.0f, -1.0f),      D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) }
};

CustomVertex3NormalUVTangent verticesGround[] = {
	{ D3DXVECTOR3(-50.0f, -5.0f, 50.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(50.0f, -5.0f, 50.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(-50.0f, -5.0f, -50.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) },
	{ D3DXVECTOR3(50.0f, -5.0f, -50.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f) }
};

TestApp::TestApp(Window* window) : mWindow(window), 
								   mCube(NULL),
								   mGround(NULL),
								   mTimeSinceElapsedTimeReset(0.0f), 
								   mSceneCamera(new FreeCamera(D3DXVECTOR3(0.0f, 5.0f, -50.0f))),
								   mLightCamera(new FreeCamera(D3DXVECTOR3(100.0f, 30.0f, 0.0f))),
								   mWireframeMode(false)
{
	mMSAAModeIterator = mWindow->getRenderDevice()->getMSAAModes().begin();
}

TestApp::~TestApp() {
	SAVEDELETE(mShadowMap);
	SAVEDELETE(mSceneCamera);
	SAVEDELETE(mLightCamera);
}

void TestApp::onCreateDevice() {

	// set vertex delaration	
	mWindow->getRenderDevice()->setVertexDeclaration(CustomVertex3NormalUVTangentBinormal::decl);

	// load shader
	mWindow->getRenderDevice()->loadEffectFromFile("./shader/shadow.fx");

	// load textures
	HR(D3DXCreateTextureFromFileA(D3DDEVICE, "./texture/stone.jpg", &mWhiteTexture));
	HR(D3DXCreateTextureFromFileA(D3DDEVICE, "./texture/stone_normal_map.jpg", &mNormalTextureBricks));
	HR(D3DXCreateTextureFromFileA(D3DDEVICE, "./texture/floor_nmap.bmp", &mNormalTextureFloor));

	ID3DXMesh* tempMesh = 0;

	// load scene Mesh
	XManager::loadXFile(D3DDEVICE, "./model/testscene/BasicColumnScene.x", &tempMesh, mSceneMaterials, mSceneTextures);

	// Get the vertex declaration for the NMapVertex.
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;
	HR(CustomVertex3NormalUVTangentBinormal::decl->GetDeclaration(elems, &numElems));

	// Clone the mesh to the NMapVertex format.
	ID3DXMesh* clonedTempMesh = 0;
	HR(tempMesh->CloneMesh(D3DXMESH_MANAGED, elems, D3DDEVICE, &clonedTempMesh));

	// Now use D3DXComputeTangentFrameEx to build the TNB-basis for each vertex
	// in the mesh.  
	
	HR(D3DXComputeTangentFrameEx(
	  clonedTempMesh, // Input mesh
	  D3DDECLUSAGE_TEXCOORD, 0, // Vertex element of input tex-coords.  
      D3DDECLUSAGE_BINORMAL, 0, // Vertex element to output binormal.
	  D3DDECLUSAGE_TANGENT, 0,  // Vertex element to output tangent.
      D3DDECLUSAGE_NORMAL, 0,   // Vertex element to output normal.
      0, // Options
      0, // Adjacency
	  0.01f, 0.25f, 0.01f, // Thresholds for handling errors
	  &mSceneMesh, // Output mesh
	  0));         // Vertex Remapping


	tempMesh->Release();
	clonedTempMesh->Release();
	
	UINT16* indices;
	CustomVertex3NormalUVTangentBinormal* vertices;

	mSceneMesh->LockIndexBuffer(D3DLOCK_READONLY, reinterpret_cast<void**>(&indices));
	mSceneMesh->LockVertexBuffer(D3DLOCK_READONLY, reinterpret_cast<void**>(&vertices));
	
	for(int i = 0; i < mSceneMesh->GetNumFaces(); ++i) {
		CustomVertex3NormalUVTangentBinormal v1, v2, v3;

		v1 = vertices[indices[i*3]];
		v2 = vertices[indices[i*3+1]];
		v3 = vertices[indices[i*3+2]];
	}
	
	mSceneMesh->UnlockIndexBuffer();
	mSceneMesh->UnlockVertexBuffer();


	// init light
	initLight();
	// texture for drawing the shadowmap
	D3DVIEWPORT9 vp = {0, 0, 1024, 1024, 0.0f, 1.0f};
	mShadowMap = new DrawableTexture2D(D3DDEVICE, 1024, 1024, 1, D3DFMT_R32F, true, D3DFMT_D24X8, vp, false);

	// create and init cube
	//mCube = mWindow->getRenderDevice()->createVertexBuffer(36, CUSTOMVERTEX3NORMALUVTANGENT, std::string("cube"));
	//mWindow->getRenderDevice()->setVertexBufferData(std::string("cube"), verticesCube);

	// create and init ground
	//mGround = mWindow->getRenderDevice()->createVertexBuffer(4, CUSTOMVERTEX3NORMALUVTANGENT, std::string("ground"));
	//mWindow->getRenderDevice()->setVertexBufferData(std::string("ground"), verticesGround);

	// init shader handles
	initShaderHandles();
	
	//mLightCamera->pitch(D3DXToRadian(90.0f));
	D3DXCreateSphere(D3DDEVICE, 5.0f, 10, 10, &teapot, NULL);


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
		
		//mWindow->getRenderDevice()->renderVertexbuffer(D3DPT_TRIANGLELIST, std::string("cube"));
		//mWindow->getRenderDevice()->renderVertexbuffer(D3DPT_TRIANGLESTRIP, std::string("ground"));

		for(UINT i = 0; i < mSceneMaterials.size(); ++i) {
			HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mMaterialHandle, &mSceneMaterials.at(i), sizeof(Material)));
			//HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mSceneTextures.at(i)));
			HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());

			mSceneMesh->DrawSubset(i);
		}
		

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

		GetCursorPos(&mousePos);

		sprintf(buffer, "X: %d, Y: %d\n", mousePos.x, mousePos.y);
		OutputDebugStringA(buffer);
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
	
	//mWindow->getRenderDevice()->renderVertexbuffer(D3DPT_TRIANGLELIST, std::string("cube"));
	//mWindow->getRenderDevice()->renderVertexbuffer(D3DPT_TRIANGLESTRIP, std::string("ground"));

	for(UINT i = 0; i < mSceneMaterials.size(); ++i) {
			//HR(mWindow->getRenderDevice()->getCurrentEffect()->SetValue(mMaterialHandle, &mSceneMaterials.at(i), sizeof(Material)));
			//HR(mWindow->getRenderDevice()->getCurrentEffect()->SetTexture(mTextureHandle, mSceneTextures.at(i)));
			//HR(mWindow->getRenderDevice()->getCurrentEffect()->CommitChanges());
			mSceneMesh->DrawSubset(i);
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
	mSceneMesh->Release();
	delete mShadowMap;

	mWindow->getRenderDevice()->resetDevice(currentDevInfo);
}