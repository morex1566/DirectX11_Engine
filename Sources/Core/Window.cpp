#include "PCH.h"
#include "Window.h"

unsigned int Window::_width;
unsigned int Window::_height;
HWND Window::_hWnd;

Window::~Window()
{
	::DestroyWindow(_hWnd);
	::UnregisterClassW(_wc.lpszClassName, _wc.hInstance);
}

bool Window::Initialize(WNDPROC winProc_, const std::wstring& titleName_, int screenWidth_, int screenHeight_)
{
	_wc = { sizeof(_wc), CS_CLASSDC, winProc_, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, titleName_.c_str(), nullptr };
	::RegisterClassExW(&_wc);
	_hWnd = ::CreateWindowW(_wc.lpszClassName, titleName_.c_str(), WS_OVERLAPPEDWINDOW, 100, 100, screenWidth_, screenHeight_, nullptr, nullptr, _wc.hInstance, nullptr);

	// Setup the flag as true.
	{
		_isEnabled = true;
		_isActivated = true;
	}

	return true;
}

void Window::Show()
{
	::ShowWindow(_hWnd, SW_SHOWDEFAULT);
}

void Window::Update()
{
	RECT rect;
	GetClientRect(_hWnd, &rect);

	_width = rect.right - rect.left; 
	_height = rect.bottom - rect.top; 

	::UpdateWindow(_hWnd);
}

unsigned Window::GetWidth()
{
	return _width;
}

unsigned Window::GetHeight()
{
	return _height;
}

HWND Window::GetHWND() const
{
	return _hWnd;
}

std::wstring Window::GetTitleName() const
{
	return _titleName;
}
