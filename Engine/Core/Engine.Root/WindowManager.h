#pragma once
#include "Window.h"

class WindowManager
{
public:
	WindowManager(const WindowManager&)				= delete;
	WindowManager(const WindowManager&&)			= delete;
	WindowManager& operator=(const WindowManager&)	= delete;

	static WindowManager& Get();

	void Initialize(WNDPROC  winProc_, const std::string& name_, const std::string& title_, HINSTANCE hInstance_);
	void Shutdown();

	void ShowWindows();

	Window& GetMainWindow();

private:
	WindowManager() = default;

private:
	std::vector<std::shared_ptr<Window>>		_windows;
	std::shared_ptr<Window>						_mainWindow;
};