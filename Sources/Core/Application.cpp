// ReSharper disable CppClangTidyBugproneBranchClone

#include "pch.h"
#include "Application.h"
#include "WindowManager.h"
#include "D3D11Manager.h"

Application::~Application()
{
	ClearMemory();
}

LRESULT Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		Application::GetInstance().Shutdown();
		return 0;

	case WM_CLOSE:
		Application::GetInstance().Shutdown();
		return 0;

	case WM_QUIT:
		Application::GetInstance().Shutdown();
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
	return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Application::Initialize(const HINSTANCE& hInstance_)
{
	// ClearMemory the app before when we initialized.
	ClearMemory();

	// Initialize member variables.
	{
		_hInstance = hInstance_;
		_isShutdowned = false;
	}

	// Create the application's window.
	WindowManager& windowManager = WindowManager::GetInstance();
	{
		Window* appWindow = windowManager.Create(
			_hInstance,
			"Sample Window Class",
			"Engine.exe",
			WindowProc,
			(Utils::GetMonitorResX() / 2) - (Config::Setting::Window::GetWidth() / 2),
			(Utils::GetMonitorResY() / 2) - (Config::Setting::Window::GetHeight() / 2),
			Config::Setting::Window::GetWidth(),
			Config::Setting::Window::GetHeight()
		);

		windowManager.SetAppWindow(appWindow);
	}

	// Create the D3D11Manager.
	D3D11Manager& d3d11Manager = D3D11Manager::GetInstance();
	{
		d3d11Manager.Initialize(
			Config::Setting::Window::GetWidth(),
			Config::Setting::Window::GetHeight(),
			Config::Setting::Window::GetVsyncEnabled(),
			windowManager.GetAppWindow()->GetHWnd(),
			Config::Setting::Window::GetFullScreenEnabled(),
			1000,
			0.3f
		);
	}
}

void Application::Update()
{
	MSG msg;
	while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void Application::ClearMemory()
{
	WindowManager::GetInstance().ClearMemory();
}

void Application::Shutdown()
{
	_isShutdowned = true;
}

bool Application::IsShutdowned()
{
	return _isShutdowned;
}

HINSTANCE Application::GetHInstance()
{
	return _hInstance;
}