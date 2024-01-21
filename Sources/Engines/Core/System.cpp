#include "PCH.h"
#include "System.h"

bool System::isLooping;

System::System(HINSTANCE hInstance)
	: hInstance(hInstance)
{
	isLooping = true;
}

void System::MsgHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
	// 시스템 종료
	if (Msg == WM_QUIT || Msg == WM_DESTROY)
	{
		isLooping = false;
	}
}

void System::Quit()
{
	isLooping = false;
}

void System::Init()
{
	// 주의 : 콘솔은 가장 먼저 Init()
	console = std::make_unique<Console>();
	{
		console->Init();
		Console::Log(L"Init Console OK.");
	}

	window = std::make_unique<Window>(hInstance);
	{
		window->Init();
		Console::Log(L"Init Window OK.");
	}

	directX11 = std::make_unique<DirectX11>(hInstance);
	{
		directX11->Init(window->GetWindowWidth(), window->GetWindowHeight(),
						window->GetHWND(), window->GetFullScreenEnabled());
		Console::Log(L"Init DirectX11 OK.");
	}
}

void System::Start()
{
	window->ShowWindow();
}

void System::Update()
{
	window->UpdateWindow();

	directX11->ClearRenderTargetView();
	directX11->ClearDepthStencilView();
	directX11->Draw();
}

void System::Shutdown()
{
	directX11->Shutdown();
	window->Shutdown();
	
	// 주의 : 콘솔은 항상 마지막에 Shutdown()
	console->Shutdown();
}