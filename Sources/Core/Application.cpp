// ReSharper disable CppClangTidyBugproneBranchClone

#include "pch.h"
#include "Application.h"
#include "WindowManager.h"
#include "D3D11Manager.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"

Application::~Application()
{
	ClearMemory();
}

LRESULT Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		Application::GetInstance().Shutdown();
		return 0;

	case WM_CLOSE:
		Application::GetInstance().Shutdown();
		return 0;

	case WM_QUIT:
		Application::GetInstance().Shutdown();
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
	return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Application::Initialize(const HINSTANCE& hInstance_)
{
	// ClearMemory the app before when we initialized.
	ClearMemory();

	// Initialize member variables.
	{
		_hInstance = hInstance_;
		_isShutdowned = false;
	}

	// Create the application's window.
	WindowManager& windowManager = WindowManager::GetInstance();
	{
		windowManager.Initialize();

		std::shared_ptr<Window> appWindow = windowManager.Create(
			_hInstance,
			"Sample Window Class",
			"Engine.exe",
			WindowProc,
			(Utils::GetMonitorResX() / 2) - (Config::Setting::Window::GetWidth() / 2),
			(Utils::GetMonitorResY() / 2) - (Config::Setting::Window::GetHeight() / 2),
			Config::Setting::Window::GetWidth(),
			Config::Setting::Window::GetHeight()
		);

		windowManager.SetAppWindow(appWindow);
	}

	// Initialize the D3D11Manager.
	D3D11Manager& d3d11Manager = D3D11Manager::GetInstance();
	{
		d3d11Manager.Initialize(
			Config::Setting::Window::GetWidth(),
			Config::Setting::Window::GetHeight(),
			Config::Setting::Window::GetVsyncEnabled(),
			windowManager.GetAppWindow()->GetHWnd(),
			Config::Setting::Window::GetFullScreenEnabled(),
			1000,
			0.3f
		);
	}

	// Initialize the SceneManager.
	SceneManager& sceneManager = SceneManager::GetInstance();
	{
		sceneManager.Initialize();
		std::shared_ptr<Scene> defaultScene = sceneManager.Create();
		defaultScene->SetName("Untitled");
		sceneManager.LoadScene("Untitled");
	}

	// Initialize the GameobjectManager.
	GameObjectManager& gameObjectManager = GameObjectManager::GetInstance();
	{
		gameObjectManager.Initialize();
	}

	auto go1 = gameObjectManager.Create();
	go1->SetName("go1");
	auto go2 = gameObjectManager.Create();
	go2->SetName("go2");

	auto camera = gameObjectManager.Create<Camera>();
	{
		camera->SetPosition(0.0f, 0.0f, -15.0f);
		_camera = camera;
	}


	auto light1 = gameObjectManager.Create<Light>();
	{
		light1->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		light1->SetDirection(0.0f, 0.0f, 1.0f);
	}

	auto currScene = sceneManager.GetCurrentScene();
	{
		currScene->AddHierarchy(go1);
		currScene->AddHierarchy(go2);
		currScene->SetEnviromentLight(light1);
	}

	// Initialize the ComponentManager.
	ComponentManager& componentManager = ComponentManager::GetInstance();
	{
		componentManager.Initialize();
	}

	auto model1 = componentManager.Create<Model>();
	{
		model1->Initialize(
			d3d11Manager.GetDevice(),
			d3d11Manager.GetDeviceContext(),
			windowManager.GetAppWindow()->GetHWnd(),
			FROM_SOLUTION_PATH_TO("Resources/box.fbx"),
			FROM_SOLUTION_PATH_TO("Resources/stone.jpg"),
			FROM_SOLUTION_PATH_TO("Shaders/light.vs"),
			FROM_SOLUTION_PATH_TO("Shaders/light.ps")
		);
	}

	go1->AttachComponent(model1);
}

void Application::Update()
{
	// Event.
	{
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	// Rendering.
	D3D11Manager& d3d11Manager = D3D11Manager::GetInstance();
	{
		d3d11Manager.BeginScene(0.14f, 0.14f, 0.14f, 1.0f);

		_camera->Render();

		// Render objects in scene hierarchy.
		std::shared_ptr<Scene> currScene = SceneManager::GetInstance().GetCurrentScene();
		{
			for (auto& gameObject : currScene->GetHierarchy())
			{
				std::vector<std::shared_ptr<Model>> models = gameObject->GetComponents<Model>();
				for (auto& model : models)
				{
					model->Render(d3d11Manager.GetWorldMatrix(), _camera->GetViewMatrix(), d3d11Manager.GetProjectionMatrix(), 
								  currScene->GetEnviromentLight());
				}
			}
		}

		d3d11Manager.EndScene();
	}

	// Dispatch.
	Dispose();
}

void Application::ClearMemory()
{
	GameObjectManager::GetInstance().ClearMemory();
	SceneManager::GetInstance().ClearMemory();
	D3D11Manager::GetInstance().ClearMemory();
	WindowManager::GetInstance().ClearMemory();
}

void Application::Dispose()
{
	SceneManager& sceneManager = SceneManager::GetInstance();
	{
		sceneManager.Dispose();
	}

	GameObjectManager& gameObjectManager = GameObjectManager::GetInstance();
	{
		gameObjectManager.Dispose();
	}

	ComponentManager& componentManager = ComponentManager::GetInstance();
	{
		componentManager.Dispose();
	}
}

void Application::Shutdown()
{
	_isShutdowned = true;
}

bool Application::IsShutdowned()
{
	return _isShutdowned;
}

HINSTANCE Application::GetHInstance()
{
	return _hInstance;
}