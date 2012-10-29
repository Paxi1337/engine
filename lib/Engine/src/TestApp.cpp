#include "../includes/TestApp.h" 
#include "../includes/Camera.h"
#include "../includes/FPCamera.h"
#include "../includes/Timer.h"

#include <vector>
#include <algorithm>
#include <cmath>


int count = 1;
Timer t;

const float kCameraMovementSpeed=0.4f;
const float kCameraRotationSpeed=0.01f;


TestApp::TestApp() : m_window(NULL), m_buffer(NULL), m_timeSinceElapsedTimeReset(0.0f), mNewCamera(new CFPCamera(D3DXVECTOR3(0.0f,0.0f,-10.0f))) {

}

TestApp::TestApp(Window* window) : m_window(window), m_buffer(NULL), m_timeSinceElapsedTimeReset(0.0f), mNewCamera(new CFPCamera(D3DXVECTOR3(0.0f,0.0f,-10.0f))) {

}

TestApp::~TestApp() {

}

void TestApp::onCreateDevice() {
	
	mNewCamera->setPosition(D3DXVECTOR3(0.0f,0.0f,-25.0f));
	//m_camera->setLookAt(D3DXVECTOR3(0.0f,0.0f,-15.0f));
	//m_camera->updateCamera();

	CustomVertex3Color vertices[] =
	{
		{ D3DXVECTOR3(-3.0f, 0.0f, 0.0f), D3DCOLOR_XRGB(0, 0, 255) },
		{ D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DCOLOR_XRGB(0, 255, 0) },
		{ D3DXVECTOR3(3.0f, 0.0f, 0.0f), D3DCOLOR_XRGB(255, 0, 0) }
	};

	

	if(m_window) {
		m_buffer = m_window->getRenderDevice()->createVertexBuffer(3, CUSTOMVERTEX3COLORFORMAT, std::string("test"));
		
		m_buffer3 = m_window->getRenderDevice()->createVertexBuffer(3, CUSTOMVERTEX3COLORFORMAT, std::string("test3"));
		m_buffer4 = m_window->getRenderDevice()->createVertexBuffer(3, CUSTOMVERTEX3COLORFORMAT, std::string("test4"));

		
		m_window->getRenderDevice()->setVertexBufferData(std::string("test"), vertices);
		m_window->getRenderDevice()->setVertexBufferData(std::string("test3"), vertices);
		m_window->getRenderDevice()->setVertexBufferData(std::string("test4"), vertices);

	}
}

void TestApp::onResetDevice() {

}

void TestApp::onReleaseDevice() {

}

void TestApp::onUpdate() {

}

//float elapsedTime = 0.0f;

void TestApp::onRender() {
	t.start();

	D3DXMATRIX worldTransform;
	D3DXMatrixIdentity(&worldTransform);
	
	
	D3DXMATRIX translatedRight, rot;
	D3DXMatrixIdentity(&rot);
	D3DXMatrixIdentity(&translatedRight);
	D3DXMatrixTranslation(&translatedRight,10.0f,0.0f,0.0f);
	D3DXMatrixRotationY(&rot,D3DXToRadian(90.0f));

	D3DXMATRIX translatedLeft;
	D3DXMatrixIdentity(&translatedLeft);
	D3DXMatrixTranslation(&translatedLeft,-25.0f,0.0f,12.0f);

	if(m_window) {

		D3DXMATRIX view;
		mNewCamera->calculateViewMatrix(view);
		m_window->getRenderDevice()->setTransform(D3DTS_VIEW, &view);
		
		D3DXMATRIX projection;
		mNewCamera->calculateProjectionMatrix(projection);
		m_window->getRenderDevice()->setTransform(D3DTS_PROJECTION, &projection);

		m_window->getRenderDevice()->renderVertexbuffer(D3DPT_TRIANGLELIST, std::string("test"), rot*worldTransform);
		m_window->getRenderDevice()->renderVertexbuffer(D3DPT_LINESTRIP, std::string("test2"), worldTransform);
		m_window->getRenderDevice()->renderVertexbuffer(D3DPT_TRIANGLELIST, std::string("test3"), translatedRight);
		m_window->getRenderDevice()->renderVertexbuffer(D3DPT_TRIANGLELIST, std::string("test4"), rot*translatedLeft);
		m_window->getRenderDevice()->renderVertexbuffer(D3DPT_LINESTRIP, std::string("test5"), worldTransform);

		t.stop();
		float elapsedTime = t.getElapsedTimeInMilliSec();
		m_timeSinceElapsedTimeReset = m_timeSinceElapsedTimeReset + elapsedTime;
	}

	
}

void TestApp::onLostDevice() {

}

void TestApp::setWindow(Window* window) {
	m_window = window;
}

void TestApp::onKeyPressed(WPARAM keyCode) {
	//D3DXVECTOR3 currentPos = m_camera->getPosition();

	switch(keyCode) {	
		case 'W':
			mNewCamera->moveForward(kCameraMovementSpeed);
			break;
		case 'S':
			mNewCamera->moveForward(-kCameraMovementSpeed);
			break;
		case 'A':
			mNewCamera->moveRight(-kCameraMovementSpeed);
			break;
		case 'D':
			mNewCamera->moveRight(kCameraMovementSpeed);
			break;
		case VK_UP:
			mNewCamera->pitch(kCameraRotationSpeed);
			break;
		case VK_DOWN:
			mNewCamera->yaw(kCameraRotationSpeed);
			break;
		case 'O':
			//m_camera->head(D3DXToRadian(5.0f));
			break;
		default: break;

	}

	//m_camera->updateCamera();
	//m_camera->setPosition(currentPos);
}

void TestApp::onRawMouseInputReceived(RAWINPUT const& rawMouseInput) {
	static BOOL mouseDown;

	if(rawMouseInput.data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN)
		mouseDown = true;
	if(rawMouseInput.data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP)
		mouseDown = false;

	if(mouseDown) {
		//char buffer[30];
		//ZeroMemory(buffer,30);

		float lastX = rawMouseInput.data.mouse.lLastX;
		float lastY = rawMouseInput.data.mouse.lLastY;
		//_snprintf(buffer,30,"%08f\n",lastX);

		mNewCamera->pitch(kCameraRotationSpeed*lastY);
		mNewCamera->yaw(kCameraRotationSpeed*lastX);

		//OutputDebugStringA(buffer);
	}
}