#include "PCH.h"
#include "Window.h"
#include "Config.h"

HWND				Window::_hWnd;
std::wstring		Window::_titleName;
unsigned int		Window::_windowWidth;
unsigned int		Window::_windowHeight;
unsigned int		Window::_clientWidth;
unsigned int		Window::_clientHeight;
bool				Window::_isFullScreenEnabled;

Window::~Window()
{
	Config::SetWindowWidth(_windowWidth);
	Config::SetWindowHeight(_windowHeight);
	Config::SetIsFullScreenEnabled(_isFullScreenEnabled);

	::DestroyWindow(_hWnd);
	::UnregisterClassW(_wc.lpszClassName, _wc.hInstance);
}

bool Window::Initialize(WNDPROC winProc_, const std::wstring& titleName_, int windowWidth_, int windowHeight_, bool isFullScreenEnabled_)
{
	// Initialize member variable
	{
		_winProc = winProc_;
		_titleName = titleName_;
		_windowWidth = windowWidth_;
		_windowHeight = windowHeight_;
		_isFullScreenEnabled = isFullScreenEnabled_;

		_wc = { sizeof(_wc), CS_CLASSDC, _winProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, _titleName.c_str(), nullptr };
		::RegisterClassExW(&_wc);
		_hWnd = ::CreateWindowW(_wc.lpszClassName, _titleName.c_str(), WS_OVERLAPPEDWINDOW, 100, 100, _windowWidth, _windowHeight, nullptr, nullptr, _wc.hInstance, nullptr);

		CalculateClientScreen();
	}

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
	CalculateClientScreen();

	::UpdateWindow(_hWnd);
}

void Window::CalculateClientScreen()
{
	RECT clientRect;
	GetClientRect(_hWnd, &clientRect);
	{
		_clientWidth = clientRect.right - clientRect.left;
		_clientHeight = clientRect.bottom - clientRect.top;
	}
}

void Window::CalculateWindowScreen()
{
	RECT windowRect;
	GetWindowRect(_hWnd, &windowRect);
	{
		_windowWidth = windowRect.right - windowRect.left;
		_windowHeight = windowRect.bottom - windowRect.top;
	}
}


HWND Window::GetHWND()
{
	return _hWnd;
}

std::wstring Window::GetTitleName()
{
	return _titleName;
}

unsigned Window::GetWindowWidth()
{
	return _windowWidth;
}

unsigned Window::GetWindowHeight()
{
	return _windowHeight;
}

unsigned Window::GetClientWidth()
{
	return _clientWidth;
}

unsigned Window::GetClientHeight()
{
	return _clientHeight;
}

bool Window::GetIsFullScreenEnabled()
{
	return _isFullScreenEnabled;
}

void Window::SetTitleName(const std::wstring& titleName_)
{
	_titleName = titleName_;
	SetWindowText(_hWnd, _titleName.c_str());
}

void Window::SetResolutionWidth(unsigned int width_)
{
	_windowWidth = width_;
}

void Window::SetResolutionHeight(unsigned int height_)
{
	_windowHeight = height_;
}

void Window::SetIsFullScreenEnabled(bool toggle_)
{
	_isFullScreenEnabled = toggle_;
}