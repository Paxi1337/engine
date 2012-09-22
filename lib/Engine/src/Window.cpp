#include "../includes/Window.h"

Window::Window(const HINSTANCE hInstance, int nCmdShow) : m_hInstance(hInstance), m_windowCallBackFunction(messageCallback), m_nCmdShow(nCmdShow) {
	memset(&m_windowClassEx, '\0', sizeof(WNDCLASSEX));
}

Window::~Window() {

}

bool Window::initWindowClass(UINT style, LPCWSTR className) {
	// check for correct string..
	m_className = className;

	m_windowClassEx.cbSize = sizeof(WNDCLASSEX);
	m_windowClassEx.style = style;
	m_windowClassEx.lpfnWndProc = Window::m_windowCallBackFunction;
	m_windowClassEx.hInstance = m_hInstance;
	m_windowClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_windowClassEx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
	m_windowClassEx.lpszClassName = m_className;

	if(RegisterClassEx(&m_windowClassEx)) {
		return true;	
	}

	// debuginfo here
	// omitting return value from GetLastError() for now
	GetLastError();

	return false;
}

bool Window::createWindow(LPCWSTR title, int x, int y, int width, int height) {
	// passing in this pointer as optional param (lpParam) for using it later to access internal method in static message callback function
	m_hWindow = CreateWindowEx(NULL, m_className, title, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, m_hInstance, this);

	if(m_hWindow) {
		
		if(ShowWindow(m_hWindow, m_nCmdShow)) {
			return true;
		}
		else {
			// debuginfo here
			// omitting return value from GetLastError() for now
			GetLastError();
			return false;
		}
	}

	// debuginfo here
	// omitting return value from GetLastError() for now
	GetLastError();
	return false;
}


int Window::run() {
	// this struct holds Windows event messages
    MSG msg;

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


    }

	return msg.wParam;
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
    // handle the given message
    switch(message)
    {
        // case WM_DESTROY when window gets closed
        case WM_DESTROY:
            {
                // closes the application entirely
                PostQuitMessage(0);
                return 0;
            }

		default: return DefWindowProc (hWnd, message, wParam, lParam);
    }
}