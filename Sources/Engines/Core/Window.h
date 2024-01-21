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


public:
	void Init();
	void ShowWindow();
	void UpdateWindow();
	void Shutdown();

	FORCEINLINE int GetWindowWidth() const { return windowWidth; }
	FORCEINLINE int GetWindowHeight() const { return windowHeight; }
	FORCEINLINE bool GetFullScreenEnabled() const { return isFullScreenEnabled; }
	FORCEINLINE HWND GetHWND() const { return hWindow; }

private:
	static bool isFullScreenEnabled;


private:
	HINSTANCE hInstance;
	std::wstring name;
	int windowWidth;
	int windowHeight;

	HWND hWindow;
	WNDCLASSEX windowClass;


};