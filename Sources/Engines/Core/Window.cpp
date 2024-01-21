#include "PCH.h"
#include "Window.h"
#include "System.h"

bool Window::isFullScreenEnabled = false;

Window::Window(HINSTANCE hInstance, std::wstring name, int windowWidth, int windowHeight)
    : hInstance(hInstance), name(name), windowWidth(windowWidth), windowHeight(windowHeight)
{
}

LRESULT __stdcall Window::MsgHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
	System::MsgHandler(HWnd, Msg, WParam, LParam);

	return ::DefWindowProcW(HWnd, Msg, WParam, LParam);
}

void Window::Init()
{
	// 윈도우 시작점 설정
	int WindowStartPosX, WindowStartPosY;
	{
		::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

		WindowStartPosX = Utls::GetGap(GetSystemMetrics(SM_CXSCREEN), windowWidth) / 2;
		WindowStartPosY = Utls::GetGap(GetSystemMetrics(SM_CYSCREEN), windowHeight) / 2;
	}

	// 윈도우값 설정
	{
		windowClass.style = CS_CLASSDC;
		windowClass.lpfnWndProc = MsgHandler;
		windowClass.cbClsExtra = 0L;
		windowClass.cbWndExtra = 0L;
		windowClass.hInstance = hInstance;
		windowClass.hIcon = nullptr;
		windowClass.hIconSm = nullptr;
		windowClass.hCursor = nullptr;
		windowClass.hbrBackground = nullptr;
		windowClass.lpszMenuName = nullptr;
		windowClass.lpszClassName = name.c_str();
		windowClass.cbSize = sizeof(windowClass);

		::RegisterClassExW(&windowClass);
	}

	// 윈도우핸들 생성
	{
		hWindow = ::CreateWindowW(
			windowClass.lpszClassName,
			name.c_str(),
			WS_OVERLAPPEDWINDOW, WindowStartPosX, WindowStartPosY, windowWidth, windowHeight,
			nullptr,
			nullptr,
			windowClass.hInstance,
			nullptr);
	}
}

void Window::ShowWindow()
{
	::ShowWindow(hWindow, SW_SHOWDEFAULT);
}

void Window::UpdateWindow()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	::UpdateWindow(hWindow);
}

void Window::Shutdown()
{
	::DestroyWindow(hWindow);
	::UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
}
