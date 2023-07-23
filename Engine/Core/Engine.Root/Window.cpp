#include "pch.h"
#include "Window.h"

#include <utility>


Window::Window(WNDPROC windowProc_, std::string name_, std::string title_, HINSTANCE hInstance_)
	: _WindowProc(windowProc_), _Name(std::move(name_)), _Title(std::move(title_)), _HInstance(hInstance_), _Hwnd(NULL)
{
}

void Window::Initialize()
{
	// Register the window class.
	{
		WNDCLASS wc = { };

		wc.lpfnWndProc = _WindowProc;
		wc.hInstance = _HInstance;
		wc.lpszClassName = _Name.c_str();

		RegisterClass(&wc);
	}

	// Create the window.z
	_Hwnd = createHWND();
	if (_Hwnd == NULL)
	{
		LogManager::LogError("Window Creation Failure.");
		return;
	}
}

void Window::Shutdown()
{
	DestroyWindow(_Hwnd);
	_Hwnd = NULL;

	UnregisterClass(_Name.c_str(), _HInstance);
	_HInstance = NULL;
}

void Window::ShowWindow() const
{
	::ShowWindow(_Hwnd, 1);
}

HWND Window::createHWND()
{
	_Hwnd = CreateWindowEx(
		0,																					// Optional window styles.
		_Name.c_str(),																		// Window class
		_Title.c_str(),																		// Window text
		WS_OVERLAPPEDWINDOW,																// Window style
		CW_USEDEFAULT, CW_USEDEFAULT,
		Config::GET_ENGINE_SETTING_WINDOW_WIDTH(),
		Config::GET_ENGINE_SETTING_WINDOW_HEIGHT(),
		NULL,																				// Parent window    
		NULL,																				// Menu
		_HInstance,																			// Instance handle
		NULL																				// Additional application data
	);

	return _Hwnd;
}