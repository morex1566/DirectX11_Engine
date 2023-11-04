#include "PCH.h"
#include "GCamera.h"
#include "OApplication.h"
#include "OConsole.h"
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
			OApplication::Quit();

			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			OApplication::Quit();

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

Object::EHandleResultType OWindow::Initialize()
{
	Object::Initialize();

	// Set Window Setting
	uint32 WindowScreenWidth, WindowScreenHeight;
	uint32 WindowStartPosX, WindowStartPosY;
	{
		WindowScreenWidth = 1600;
		WindowScreenHeight = 900;

		WindowStartPosX = (GetSystemMetrics(SM_CXSCREEN) - WindowScreenWidth) / 2;
		WindowStartPosY = (GetSystemMetrics(SM_CYSCREEN) - WindowScreenHeight) / 2;
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
			OConsole::LogError(L"CreateWindowW() is failed.");
			return EHandleResultType::Failed;
		}
	}

	// Get current window size.
	Resize();

	return EHandleResultType::Success;
}

void OWindow::Release()
{
	Object::Release();

	::DestroyWindow(HWnd);
	::UnregisterClassW(WCEX.lpszClassName, WCEX.hInstance);
}

void OWindow::Start()
{
	Object::Start();

	::ShowWindow(HWnd, SW_SHOWDEFAULT);
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
