#include "pch.h"
#include "WindowManager.h"

void WindowManager::Initialize()
{
	IManager<Window>::Initialize();
}

void WindowManager::Update()
{
	IManager<Window>::Update();

	MSG msg;
	while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void WindowManager::Dispose()
{
	IManager<Window>::Dispose();
}

void WindowManager::Clear()
{
	IManager<Window>::Clear();
}

void WindowManager::SetAppWindow(const std::shared_ptr<Window>& window_)
{
	_appWindow = window_;
}

std::shared_ptr<Window> WindowManager::GetAppWindow()
{
	return _appWindow;
}