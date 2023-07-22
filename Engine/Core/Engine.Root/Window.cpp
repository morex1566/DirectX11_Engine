#include "pch.h"
#include "Window.h"


Window::Window(WNDPROC windowProc_, std::string name_, std::string title_, HINSTANCE hInstance_)
	: _WindowProc(windowProc_), _Name(std::move(name_)), _Title(std::move(title_)), _HInstance(hInstance_)
{
	// Register the window class.
	{
		WNDCLASS wc = { };

		wc.lpfnWndProc = windowProc_;
		wc.hInstance = hInstance_;
		wc.lpszClassName = _Name.c_str();

		RegisterClass(&wc);
	}

	// Create the window.
	_Hwnd = createHWND();
	if(_Hwnd == NULL)
	{
		throw std::exception("Craete HWND failure.");
	}
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