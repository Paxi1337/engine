#ifndef __WINDOWS_H
#define __WINDOWS_H

#include <cstdio>
#include <Windows.h>
#include "App.h"
#include "DirectX9.h"

typedef LRESULT (__stdcall *MSGCALLBACK) (HWND, UINT, WPARAM, LPARAM);

class Window {
	
public:
	Window(const HINSTANCE hInstance, int nCmdShow);
	~Window();

	bool initWindowClass(UINT style, LPCWSTR className);
	bool createWindowInitDirectX(LPCWSTR title, int x, int y, int width, int height);
	
	int run();


	static LRESULT __stdcall messageCallback(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam);
	
	
	void attachApp(App* currentApp);
	
	
	DirectX9* getRenderDevice();
	

private:
	// handle to current instance passed in by WinMain
	// 32bit int auto generated by windows for identifying process object
	// HINSTANCE = HANDLE = INT32
	HINSTANCE m_hInstance;
	// window handle
	HWND m_hWindow;
	// information struct for window (cursor, background color ...)
	WNDCLASSEX m_windowClassEx;
	// holds window callback function passed into the windowclass info struct
	MSGCALLBACK m_windowCallBackFunction;
	// class name of the window
	LPCWSTR m_className;
	// controls how window has to be shown, passed in by WinMain
	int m_nCmdShow;
	// implementation of the messageCallback function
	LRESULT __stdcall messageCallbackInternal(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam);
	// used to "link" a custom app with the window implements App.h 
	App* m_runningApp;
	// used to communicate to DirectX9
	DirectX9* m_renderDevice;

};

#endif

