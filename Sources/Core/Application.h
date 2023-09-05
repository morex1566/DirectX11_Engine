#pragma once

#include "WindowManager.h"
#include "RenderManager.h"
#include "UIManager.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"

class Application : public ISingleton<Application>
{
public:
	Application()					= default;
	~Application() override			= default;

	void Initialize(const HINSTANCE& hInstance_);
	void Update();
	void Render();
	void Dispose();
	void Clear();

	void Shutdown();
	bool IsShutdowned();

	HINSTANCE GetHInstance();

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd_, UINT uMsg_, WPARAM wParam_, LPARAM lParam_);

private:
	std::shared_ptr<Camera>					_camera;
	HINSTANCE								_hInstance;
	bool									_isShutdowned;
	float rotation = 0.0f;

	std::unique_ptr<WindowManager>			_windowManager;
	std::unique_ptr<RenderManager>			_renderManager;
	std::unique_ptr<UIManager>				_uiManager;
	std::unique_ptr<SceneManager>			_sceneManager;
	std::unique_ptr<ComponentManager>		_componentManager;
	std::unique_ptr<GameObjectManager>		_gameObjectManager;
};