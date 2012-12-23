#include "../includes/Window.h"
#include "../includes/DirectX9.h"

Window::Window(const HINSTANCE hInstance, int nCmdShow) : m_hInstance(hInstance),
														  m_windowCallBackFunction(messageCallback),		
														  m_nCmdShow(nCmdShow),
														  m_runningApp(NULL),
														  m_vsLogger(new VSLogger),
														  m_renderDevice(new DirectX9) {
	memset(&m_windowClassEx, '\0', sizeof(WNDCLASSEX));

	m_vsLogger->verbosity(debuglib::logger::DEBUG);
	m_vsLogger->outputter(new VSOutputter);
}

Window::~Window() {

}

bool Window::initWindowClass(UINT style, LPCSTR className) {
	
	// check for correct string..
	m_className = className;

	m_windowClassEx.cbSize = sizeof(WNDCLASSEX);
	m_windowClassEx.style = style;
	m_windowClassEx.lpfnWndProc = m_windowCallBackFunction;
	m_windowClassEx.hInstance = m_hInstance;
	m_windowClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_windowClassEx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
	m_windowClassEx.lpszClassName = m_className;

	if(RegisterClassEx(&m_windowClassEx)) {
		#ifdef USE_DEBUGLIB
			m_vsLogger->log("Window was succesfully initialized", debuglib::logger::DEBUG);
		#endif
		return true;	
	}

	// debuginfo here
	// omitting return value from GetLastError() for now
	GetLastError();

	return false;
}

bool Window::initMouse() {
	RAWINPUTDEVICE mouse;
	mouse.usUsage = 0x02;    // register mouse
	mouse.usUsagePage = 0x01;    // top-level mouse
	mouse.dwFlags = NULL;    // flags
	mouse.hwndTarget = m_hWindow;    // handle to a window

	RegisterRawInputDevices(&mouse, 1, sizeof(RAWINPUTDEVICE));    // register the device
	
	return true;
}

bool Window::createWindowInitDirectX(LPCSTR title, int x, int y, int width, int height) {
	
	// passing in this pointer as optional param (lpParam) for using it later to access internal method in static message callback function
	m_hWindow = CreateWindowExA(NULL, m_className, title, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, m_hInstance, this);
	
	if(m_hWindow) {
		ShowWindow(m_hWindow, m_nCmdShow);
		m_renderDevice->createDevice(m_hWindow);
		
		return true;
	}

	// debuginfo here
	// omitting return value from GetLastError() for now
	GetLastError();
	return false;
}


int Window::run() {
	// this struct holds Windows event messages
    MSG msg;

	// the render device (DirectX9* m_renderDevice) should be set at this time
	if(m_renderDevice) {
		// as the running app ..
		if(m_runningApp) {
			// sets the app for DirectX9* m_renderDevice
			m_renderDevice->attachApp(m_runningApp);
			// custom initalize function
			m_renderDevice->onCreateDevice();
		}
	}

    // start message loop
    while(1)
    {
        // look for messages in the queue
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function (messageCallback)
            DispatchMessage(&msg);
        }

        // if message is WM_QUIT, exit loop
        if(msg.message == WM_QUIT)
            break;

        // render code
		if(m_renderDevice)
			m_renderDevice->renderFrame();

    }

	return msg.wParam;
}

void Window::attachApp(App* currentApp) {
	if(currentApp) 
		m_runningApp = currentApp;
}

DirectX9* Window::getRenderDevice() {
	return m_renderDevice;
}

LRESULT Window::messageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	// see http://web.archive.org/web/20051125022758/www.rpi.edu/~pudeyo/articles/wndproc/
	if(message == WM_CREATE ) { 
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)((CREATESTRUCT *)lParam)->lpCreateParams); 
	}

	Window* tWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if(tWindow)
		return tWindow->messageCallbackInternal(hWnd, message, wParam, lParam);
	
	// called the first time this method is called
	return DefWindowProc (hWnd, message, wParam, lParam);
}

LRESULT Window::messageCallbackInternal(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	RAWINPUT inputData;

	UINT dataSize = sizeof(RAWINPUT);

    // handle the given message
    switch(message)
    {
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			break;
		}
        // case WM_DESTROY when window gets closed
        case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			m_renderDevice->dispatchKeyMessage(wParam);
			break;
		}
		case WM_INPUT:
		{
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), 
				RID_INPUT,
				&inputData,
				&dataSize,
				sizeof(RAWINPUTHEADER));

			m_renderDevice->dispatchRawMouseInput(inputData);
		}
		break;
		default: return DefWindowProc (hWnd, message, wParam, lParam);
    }

	return 0;
}