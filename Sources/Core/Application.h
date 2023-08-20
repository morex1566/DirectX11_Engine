#pragma once

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
	HINSTANCE					_hInstance;
	bool						_isShutdowned;
};