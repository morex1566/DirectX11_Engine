#include "pch.h"
#include "Window.h"

Window::Window(const HINSTANCE& hInstance_, const std::string& serialName_, const std::string& titleName_, WinProc* winProc_,
			   int posX_, int posY_, int width_, int height_)
{
	Window::Initialize(hInstance_, serialName_, titleName_, winProc_, 
					   posX_, posY_, width_, height_);
}

Window::~Window()
{
	Window::ClearMemory();
}

void Window::Initialize(const HINSTANCE& hInstance_, const std::string& serialName_, const std::string& titleName_, WinProc* winProc_, 
						int posX_, int posY_, int width_, int height_)
{
	// Clear the window before when we initialized.
	ClearMemory();

	_hInstance = hInstance_;
	_serialName = serialName_;
	_titleName = titleName_;
	_winProc = winProc_;


	// Register the window class.
	WNDCLASS wc = { };
	wc.lpfnWndProc = _winProc;
	wc.hInstance = _hInstance;
	wc.lpszClassName = _serialName.c_str();
	RegisterClass(&wc);


	// Create the window.
	_hWnd = CreateWindowEx(
		0,								// Optional window styles.
		_serialName.c_str(),			// Window class
		_titleName.c_str(),				// Window text
		WS_OVERLAPPEDWINDOW,			// Window style
		posX_,							// Window instancing x position
		posY_,							// Window instancing y position
		width_,							// Window x size
		height_,						// Window y size
		NULL,							// Parent window    
		NULL,							// Menu
		_hInstance,						// Instance handle
		NULL							// Additional application data
	);


	// Render the window.
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(_hWnd);
}

void Window::Update()
{
	UpdateWindow(_hWnd);
}

void Window::ClearMemory()
{
	if (_hWnd != nullptr)
	{
		DestroyWindow(_hWnd);
		UnregisterClassW(Utils::ToWString(_serialName).c_str(), _hInstance);
	}

	_hInstance = nullptr;
	_serialName = "";
	_titleName = "";
	_winProc = nullptr;
	_hWnd = nullptr;
}

HWND Window::GetHWnd()
{
	return _hWnd;
}
