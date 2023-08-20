#pragma once

#include "Window.h"

class WindowManager : public ISingleton<WindowManager>, public IManager<Window>
{
public:
	WindowManager();
	~WindowManager() override;

	void SetAppWindow(Window* window_);
	// TODO : Return Window* is quite dangerous. 
	Window* GetAppWindow();

private:
	Window* _appWindow;
};