#pragma once
#include "Windows.h"

class Window : public IGameLoop
{
public:
	Window()									= default;
	Window(const Window&)						= default;
	Window& operator=(const Window&)			= default;
	Window(Window&&) noexcept					= default;
	Window& operator=(Window&&) noexcept		= default;
	~Window() override 							= default;

public:
	static LRESULT WINAPI EventHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam);

	void Init() override;
	void Update() override;
	void Shutdown() override;

private:
	HWND hWindow;
	WNDCLASSEX windowClass;
	int screenWidth;
	int screenHeight;
	int windowWidth;
	int windowHeight;
};

