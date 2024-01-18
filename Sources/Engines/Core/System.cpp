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

void System::Init()
{
	window = std::make_unique<Window>(hInstance);
	{
		window->Init();
	}
}

void System::Start()
{
	window->Start();
}

void System::Update()
{
	window->Update();
}

void System::Shutdown()
{
	window->Shutdown();
}