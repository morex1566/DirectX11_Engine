#pragma once

#include "GameObjectManager.h"

class Application : public ISingleton<Application>
{
public:
	Application() = default;
	~Application() override;

	void Initialize(const HINSTANCE& hInstance_);
	void Update();
	void ClearMemory();
	void Dispose();

	void Shutdown();
	bool IsShutdowned();

	HINSTANCE GetHInstance();

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	std::shared_ptr<Camera>		_camera;
	HINSTANCE					_hInstance;
	bool						_isShutdowned;
};