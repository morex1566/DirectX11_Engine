#pragma once

#include "Window.h"

class WindowManager : public IManager<Window>
{
public:
	WindowManager()				= default;
	~WindowManager() override	= default;

	void Initialize() override;
	void Update() override;
	void Dispose() override;
	void Clear() override;

	void SetAppWindow(const std::shared_ptr<Window>& window_);
	std::shared_ptr<Window> GetAppWindow();

private:
	std::shared_ptr<Window> _appWindow;
};