#include "pch.h"
#include "Application.h"
#include "Window.h"

LRESULT Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			Application::Get()._isExited = true;
			return 0;

		case WM_PAINT:
			PAINTSTRUCT ps;
			{
				const HDC hdc = BeginPaint(hwnd, &ps);
				const HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));

				FillRect(hdc, &ps.rcPaint, hBrush);
				EndPaint(hwnd, &ps);
			}

		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Application& Application::Get()
{
	if(_instance == nullptr)
	{
		_instance = new Application;

		if (!_instance->initialize())
		{
			Logger::Log(L"Application initialize failure.");
			throw std::exception("Application initialize failure.");
		}
	}

	return *_instance;
}

void Application::Update()
{
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool Application::IsExited() const
{
	return _isExited;
}

Application::Application()
	: _HInstance(NULL), _isExited(false)
{
}

//std::unique_ptr<Window>& Application::OpenWindow()
//{
//	return 
//}

bool Application::initialize()
{
	// 1. generate main window
	_windows.emplace_back(std::make_unique<Window>(
		WindowProc,
		"Main",
		"Engine",
		_HInstance
	));

	// 2. show window
	_windows.back()->ShowWindow();

	return true;
}

std::vector<std::unique_ptr<Window>> Application::_windows;
Application* Application::_instance;