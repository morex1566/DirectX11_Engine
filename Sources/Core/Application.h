#pragma once

#include "GameObjectManager.h"

class Application : public ISingleton<Application>
{
public:
	Application() = default;
	~Application() override;

	void Initialize(const HINSTANCE& hInstance_);
	void Update();
	void Destroy();
	void ClearMemory();
	void Dispose();

	void Shutdown();
	bool IsShutdowned();

	HINSTANCE GetHInstance();

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd_, UINT uMsg_, WPARAM wParam_, LPARAM lParam_);

private:
	std::shared_ptr<Camera>		_camera;
	HINSTANCE					_hInstance;
	bool						_isShutdowned;
	float rotation = 0.0f;
};