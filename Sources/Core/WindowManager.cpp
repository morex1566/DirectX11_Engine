#include "pch.h"
#include "WindowManager.h"

WindowManager::WindowManager()
	: _appWindow(nullptr)
{
}

WindowManager::~WindowManager()
{
}

void WindowManager::SetAppWindow(Window* window_)
{
	_appWindow = window_;
}

Window* WindowManager::GetAppWindow()
{
	return _appWindow;
}