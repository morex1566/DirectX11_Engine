#pragma once

#include "Window.h"

class WindowManager : public ISingleton<WindowManager>, public IManager<Window>
{
public:
	WindowManager();
	~WindowManager() override;

	void SetAppWindow(const std::shared_ptr<Window>& window_);
	// TODO : Return Window* is quite dangerous. 
	std::shared_ptr<Window> GetAppWindow();

private:
	std::shared_ptr<Window> _appWindow;
};