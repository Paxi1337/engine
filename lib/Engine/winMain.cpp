#include "includes/smApp.h"
#include "includes/engine.h"

// TODO:
// there is a bug that since initMouse() and the mouse is used the window closes but the application still runs

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	{
		Window window(hInstance, nCmdShow);

		window.initWindowClass(CS_HREDRAW | CS_VREDRAW, "WindowClass1");
		window.createWindowInitDirectX("EZG Final Project", 300, 300, 800, 640);
		window.initMouse();

		App* testApp = new TestApp(&window);
		window.attachApp(testApp);

		return window.run();
	}
}