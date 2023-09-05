// ReSharper disable CppClangTidyBugproneBranchClone
// ReSharper disable CppClangTidyPerformanceNoIntToPtr
// ReSharper disable CppRedundantQualifier

#include "pch.h"
#include "Application.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Application::WindowProc(HWND hWnd_, UINT uMsg_, WPARAM wParam_, LPARAM lParam_)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd_, uMsg_, wParam_, lParam_))
		return true;

	switch (uMsg_)
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
		HDC hdc = BeginPaint(hWnd_, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.
		FillRect(hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>((COLOR_WINDOW + 1)));
		EndPaint(hWnd_, &ps);
	}
	return 0;
	}

	return DefWindowProc(hWnd_, uMsg_, wParam_, lParam_);
}

void Application::Initialize(const HINSTANCE& hInstance_)
{
	// Clear the app before when we initialized.
	Clear();

	// Initialize member variables.
	{
		_hInstance = hInstance_;
		_isShutdowned = false;
	}

	// Create the application's window.
	_windowManager = std::make_unique<WindowManager>();
	{
		_windowManager->Initialize();

		std::shared_ptr<Window> appWindow = _windowManager->Create(
			_hInstance,
			"Sample Window Class",
			"Engine.exe",
			WindowProc,
			(Utils::GetMonitorResX() / 2) - (Config::Setting::Window::GetWidth() / 2),
			(Utils::GetMonitorResY() / 2) - (Config::Setting::Window::GetHeight() / 2),
			Config::Setting::Window::GetWidth(),
			Config::Setting::Window::GetHeight()
		);

		_windowManager->SetAppWindow(appWindow);
	}

	// Initialize the RenderManager.
	_renderManager = std::make_unique<RenderManager>();
	{
		_renderManager->Initialize(
			Config::Setting::Window::GetWidth(),
			Config::Setting::Window::GetHeight(),
			Config::Setting::Window::GetVsyncEnabled(),
			_windowManager->GetAppWindow()->GetHWnd(),
			Config::Setting::Window::GetFullScreenEnabled(),
			1000,
			0.3f
		);
	}

	// Initialize the UIManager.
	_uiManager = std::make_unique<UIManager>();
	{
		_uiManager->Initialize(_renderManager->GetDevice(), _renderManager->GetDeviceContext(), _windowManager->GetAppWindow()->GetHWnd());
	}


	// Initialize the SceneManager.
	_sceneManager = std::make_unique<SceneManager>();
	{
		_sceneManager->Initialize();
		std::shared_ptr<Scene> defaultScene = _sceneManager->Create();
		defaultScene->SetName("Untitled");
		_sceneManager->LoadScene("Untitled");
	}

	// Initialize the GameobjectManager.
	_gameObjectManager = std::make_unique<GameObjectManager>();
	{
		_gameObjectManager->Initialize();
	}

	auto go1 = _gameObjectManager->Create();
	go1->SetName("go1");
	auto go2 = _gameObjectManager->Create();
	go2->SetName("go2");

	// Initialize the viewport camera.
	_camera = _gameObjectManager->Create<Camera>();
	{
		_camera->SetPosition(0.0f, 0.0f, -15.0f);
	}

	auto light1 = _gameObjectManager->Create<Light>();
	{
		light1->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		light1->SetDirection(0.0f, 0.0f, 1.0f);
	}

	auto currScene = _sceneManager->GetCurrentScene();
	{
		currScene->AddHierarchy(go1);
		currScene->AddHierarchy(go2);
		currScene->SetEnviromentLight(light1);
	}

	// Initialize the ComponentManager.
	_componentManager = std::make_unique<ComponentManager>();
	{
		_componentManager->Initialize();
	}

	auto model1 = _componentManager->Create<Model>();
	{
		model1->Initialize(
			_renderManager->GetDevice(),
			_renderManager->GetDeviceContext(),
			_windowManager->GetAppWindow()->GetHWnd(),
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
	_windowManager->Update();

	// Rendering.
	{
		_renderManager->BeginScene(0.14f, 0.14f, 0.14f, 1.0f);

		_camera->Render();

		XMMATRIX worldRotation = _renderManager->GetWorldMatrix();

		rotation -= 0.0174532925f * 1.0f;
		worldRotation = XMMatrixRotationY(rotation);

		// Render objects in scene hierarchy.
		std::shared_ptr<Scene> currScene = _sceneManager->GetCurrentScene();
		{
			for (auto& gameObject : currScene->GetHierarchy())
			{
				std::vector<std::weak_ptr<Model>> models = gameObject->GetComponents<Model>();
				for (auto& model : models)
				{
					model.lock()->Render(worldRotation, _camera->GetViewMatrix(), _renderManager->GetProjectionMatrix(),
								  currScene->GetEnviromentLight());
				}
			}
		}

		_uiManager->Render();

		_renderManager->EndScene();
	}
}

void Application::Render()
{
}

void Application::Dispose()
{
	_componentManager->Dispose();
	_gameObjectManager->Dispose();
	_sceneManager->Dispose();
	_uiManager->Dispose();
	_renderManager->Dispose();
	_windowManager->Dispose();
}

void Application::Clear()
{
	if (_gameObjectManager)
	{
		_gameObjectManager->Clear();
		_gameObjectManager.reset();
	}

	if (_componentManager)
	{
		_componentManager->Clear();
		_componentManager.reset();
	}

	if (_sceneManager)
	{
		_sceneManager->Clear();
		_sceneManager.reset();
	}

	if (_uiManager)
	{
		_uiManager->Clear();
		_uiManager.reset();
	}

	if (_renderManager)
	{
		_renderManager->Clear();
		_renderManager.reset();
	}

	if (_windowManager)
	{
		_windowManager->Clear();
		_windowManager.reset();
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