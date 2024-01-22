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

	Cube* cube = new Cube(directX11->GetDevice(), directX11->GetDeviceContext());
	gameObjects.push_back(cube);
}

void System::Start()
{
	// 윈도우를 화면에 띄웁니다.
	window->ShowWindow();


	// 객체들의 post-init 동작입니다.
	for (GameObject* gameObject : gameObjects)
	{
		if (gameObject->IsEnable)
		{
			gameObject->Start();
		}
	}
}

void System::Update()
{
	// 윈도우의 이벤트들을 받아줍니다.
	window->UpdateWindow();


	// 객체들을 업데이트합니다.
	for (GameObject* gameObject : gameObjects)
	{
		if (gameObject->IsEnable)
		{
			gameObject->Update();
		}
	}


	// DirectX11을 통해서 화면을 그려줍니다.
	directX11->ClearRenderTargetView();
	directX11->ClearDepthStencilView();
	directX11->Draw();
}

void System::Shutdown()
{
	// gameObject의 내부를 지워줍니다.
	for (GameObject* gameObject : gameObjects)
	{
		gameObject->Shutdown();
	}

	// gameObjects 메모리 해제
	{
		for (GameObject* gameObject : gameObjects) {
			delete gameObject;
		}

		gameObjects.clear();
	}
	
	directX11->Shutdown();
	window->Shutdown();
}