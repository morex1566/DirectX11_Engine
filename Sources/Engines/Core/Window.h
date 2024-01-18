#pragma once
#include "Windows.h"

class Window : public IEnable
{
public:
	Window(HINSTANCE hInstance, std::wstring name = L"Minecraft", int windowWidth = 1600, int windowHeight = 900);
	Window(const Window&)						= default;
	Window& operator=(const Window&)			= default;
	Window(Window&&) noexcept					= default;
	Window& operator=(Window&&) noexcept		= default;
	~Window() override  						= default;

public:
	static LRESULT WINAPI MsgHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam);

	void Init();
	void Start();
	void Update();
	void Shutdown();

private:
	HINSTANCE hInstance;
	std::wstring name;
	int windowWidth;
	int windowHeight;

	HWND hWindow;
	WNDCLASSEX windowClass;
};