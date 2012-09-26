#include "includes/Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

	Window window(hInstance, nCmdShow);
	//DirectX9 graphicInterface;

	window.initWindowClass(CS_HREDRAW | CS_VREDRAW,L"WindowClass1");
	window.createWindow(L"Debug",300,300,500,400);
	
	//graphicInterface.onCreateDevice(window.handle());

	//window.setRenderEngine(&graphicInterface);

	

	return window.run();

}