#include "PCH.h"
#include "System.h"
#include "Cube.h"

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
	window = std::make_unique<Window>(hInstance);
	{
		window->Init();
	}

	directX11 = std::make_unique<DirectX11>(hInstance);
	{
		directX11->Init(window->GetWindowWidth(), window->GetWindowHeight(),
						window->GetHWND(), window->GetFullScreenEnabled());
	}

	Cube* cube = new Cube;
	gameObjects.push_back(cube);
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
	// gameObjects 정리
	{
		for (GameObject* gameObject : gameObjects) {
			delete gameObject;
		}

		gameObjects.clear();
	}
	
	directX11->Shutdown();
	window->Shutdown();
}