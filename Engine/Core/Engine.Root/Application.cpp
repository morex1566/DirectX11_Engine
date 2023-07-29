#include "pch.h"
#include "Application.h"
#include "D3DManager.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "WindowManager.h"
#include "ResourceManager.h"

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

	SceneManager::Get().GetCurrentScene()->Render();

	D3DManager::Get().EndScene();
}

void Application::onDispose()
{
	SceneManager::Get().Dispose();

	GameObjectManager::Get().Dispose();

	ComponentManager::Get().Dispose();

	ResourceManager::Get().Dispose();
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
		SceneManager::Get().GetCurrentScene()->Update();
		onRender();
		onDispose();
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
	ResourceManager::Get().Shutdown();

	ComponentManager::Get().Shutdown();

	GameObjectManager::Get().Shutdown();

	SceneManager::Get().Shutdown();

	D3DManager::Get().Shutdown();

	WindowManager::Get().Shutdown();

	LogManager::Get().Shutdown();

	onDispose();

	_isExited = true;
}

Application::Application()
	: _HInstance(NULL), _isExited(false), _isStarted(false)
{
}

bool Application::Initialize(HINSTANCE hInstance_)
{
	// CAUTION : Set as first.
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

	// CAUTION : Set as earlier than GameObjectManager.
	SceneManager& sceneManager = SceneManager::Get();
	{
		sceneManager.Initialize();
	}

	GameObjectManager& gameObjectManager = GameObjectManager::Get();
	{
		gameObjectManager.Initialize();
	}

	ComponentManager& componentManager = ComponentManager::Get();
	{
		componentManager.Initialize();
	}

	ResourceManager& resourceManager = ResourceManager::Get();
	{
		resourceManager.Initialize();
	}

	return true;
}