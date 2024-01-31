#include "PCH.h"
#include "GCamera.h"
#include "SApplication.h"
#include "SConsole.h"
#include "OInput.h"
#include "OGUI.h"
#include "OWindow.h"

uint8					OWindow::bIsScreenSizeChanged;
						
OWindow::OWindow()
	: Object(),
	ClientScreenWidth(0), ClientScreenHeight(0), WindowScreenWidth(0), WindowScreenHeight(0),
	HWnd(nullptr), WCEX()
{
}

OWindow::~OWindow()
{
}

LRESULT WINAPI OWindow::WindowEventHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
	switch (Msg)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			SApplication::Quit();

			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			SApplication::Quit();

			return 0;
		}
		case WM_SIZE:
		{
			bIsScreenSizeChanged = 1;
			break;
		}
		case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			//const int dpi = HIWORD(wParam);
			//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = reinterpret_cast<RECT*>(LParam);
			::SetWindowPos(HWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);

			break;
		}
		default:
		{
			break;
		}
	}

	ODirectX11::MessageHandler(HWnd, Msg, WParam, LParam);
	OGUI::MessageHandler(HWnd, Msg, WParam, LParam);
	GCamera::MessageHandler(HWnd, Msg, WParam, LParam);

	return ::DefWindowProcW(HWnd, Msg, WParam, LParam);
}

void OWindow::Init()
{
	Object::Init();

	// Set Window Setting
	int WindowScreenWidth, WindowScreenHeight;
	int WindowStartPosX, WindowStartPosY;
	{
		SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

		WindowScreenWidth = 1600;
		WindowScreenHeight = 900;

		WindowStartPosX = GetGap(GetSystemMetrics(SM_CXSCREEN), WindowScreenWidth) / 2;
		WindowStartPosY = GetGap(GetSystemMetrics(SM_CYSCREEN), WindowScreenHeight) / 2;
	}
	
	// Create window class.
	{	
		WCEX.style = CS_CLASSDC;
		WCEX.lpfnWndProc = WindowEventHandler;
		WCEX.cbClsExtra = 0L;
		WCEX.cbWndExtra = 0L;
		WCEX.hInstance = GetModuleHandle(nullptr);
		WCEX.hIcon = nullptr;
		WCEX.hIconSm = nullptr;
		WCEX.hCursor = nullptr;
		WCEX.hbrBackground = nullptr;
		WCEX.lpszMenuName = nullptr;
		WCEX.lpszClassName = L"Engine";
		WCEX.cbSize = sizeof(WCEX);

		::RegisterClassExW(&WCEX);
	}

	// Create window handle.
	{
		HWnd = ::CreateWindowW(
		WCEX.lpszClassName,
		L"Engine",
		WS_OVERLAPPEDWINDOW, WindowStartPosX, WindowStartPosY, WindowScreenWidth, WindowScreenHeight,
		nullptr,
		nullptr,
		WCEX.hInstance,
		nullptr);

		if (HWnd == nullptr)
		{
			SConsole::LogError(L"CreateWindowW() is failed.", __FILE__, __LINE__);
			Object::Init();
		}
	}

	// Get current window size.
	Resize();

	// 윈도우 창 화면에 출력하기 
	::ShowWindow(HWnd, SW_SHOWDEFAULT);	
}

void OWindow::Shutdown()
{
	Object::Shutdown();

	::DestroyWindow(HWnd);
	::UnregisterClassW(WCEX.lpszClassName, WCEX.hInstance);
}

void OWindow::Start()
{
	Object::Start();
}

void OWindow::Tick()
{
	Object::Tick();

	if (bIsScreenSizeChanged == 1)
	{
		Resize();
		bIsScreenSizeChanged = 0;
	}

	::UpdateWindow(HWnd);
}

const HWND& OWindow::GetHWnd() const
{
	return HWnd;
}

uint32 OWindow::GetClientScreenWidth() const
{
	return ClientScreenWidth;
}

uint32 OWindow::GetClientScreenHeight() const
{
	return ClientScreenHeight;
}

uint32 OWindow::GetWindowScreenWidth() const
{
	return WindowScreenWidth;
}

uint32 OWindow::GetWindowScreenHeight() const
{
	return WindowScreenHeight;
}

void OWindow::Resize()
{
	RECT ClientRect;
	GetClientRect(HWnd, &ClientRect);
	{
		ClientScreenWidth = ClientRect.right - ClientRect.left;
		ClientScreenHeight = ClientRect.bottom - ClientRect.top;
	}

	RECT WindowRect;
	GetWindowRect(HWnd, &WindowRect);
	{
		WindowScreenWidth = WindowRect.right - WindowRect.left;
		WindowScreenHeight = WindowRect.bottom - WindowRect.top;
	}
}
