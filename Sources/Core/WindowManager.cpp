#include "pch.h"
#include "WindowManager.h"

WindowManager::WindowManager()
	: _appWindow(nullptr)
{
}

WindowManager::~WindowManager()
{
}

void WindowManager::SetAppWindow(const std::shared_ptr<Window>& window_)
{
	_appWindow = window_;
}

std::shared_ptr<Window> WindowManager::GetAppWindow()
{
	return _appWindow;
}