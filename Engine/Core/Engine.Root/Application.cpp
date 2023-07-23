#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "D3DManager.h"
#include "WindowManager.h"

LRESULT Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			Application::Get()._isExited = true;
			return 0;

		case WM_PAINT:
			return 0;

	case WM_SIZE:
			D3DManager::Get().Clear();
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Application::onRender()
{
	D3DManager::Get().BeginScene();

	D3DManager::Get().EndScene();
}

Application& Application::Get()
{
	static Application instance;
	return instance;
}

void Application::Update()
{
	MSG msg = { };

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (!_isExited)
	{
		onRender();
	}

}

void Application::Start()
{
	WindowManager::Get().ShowWindows();

	_isStarted = true;
}

bool Application::IsStarted() const
{
	return _isStarted;
}

bool Application::IsExited() const
{
	return _isExited;
}

void Application::Shutdown()
{
	D3DManager::Get().Shutdown();

	WindowManager::Get().Shutdown();

	LogManager::Get().Shutdown();

	_isExited = true;
}

Application::Application()
	: _HInstance(NULL), _isExited(false), _isStarted(false)
{
}

bool Application::Initialize(HINSTANCE hInstance_)
{
	LogManager& logManager = LogManager::Get();
	{
		logManager.Initialize();
	}

	WindowManager& windowManager = WindowManager::Get();
	{
		windowManager.Initialize(WindowProc, "Main", "Engine", hInstance_);
	}

	D3DManager& d3dManager = D3DManager::Get();
	{
		d3dManager.Initialize(windowManager.GetMainWindow()._Hwnd);
	}

	return true;
}