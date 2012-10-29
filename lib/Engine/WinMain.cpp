#include "includes/Engine.h"
#include "includes/TestApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

	Window window(hInstance, nCmdShow);

	window.initWindowClass(CS_HREDRAW | CS_VREDRAW,L"WindowClass1");
	window.createWindowInitDirectX(L"Debug",300,300,500,400);
	window.initMouse();

	App* testApp = new TestApp(&window);

	window.attachApp(testApp);
	return window.run();
}