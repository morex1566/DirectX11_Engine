// ReSharper disable CppLocalVariableMayBeConst

#include "pch.h"
#include "WindowManager.h"

WindowManager& WindowManager::Get()
{
	static WindowManager instance;
	return instance;
}

void WindowManager::Initialize(WNDPROC  winProc_, const std::string& name_, const std::string& title_, HINSTANCE hInstance_)
{
	// Add the default engine window
	std::shared_ptr<Window> window = std::make_shared<Window>(winProc_, name_, title_, hInstance_);
	{
		window->Initialize();
	}

	_windows.push_back(window);
	_mainWindow = _windows.back();
}

void WindowManager::Shutdown()
{
	for (auto& window : _windows)
	{
		window->Shutdown();
	}
}

void WindowManager::ShowWindows()
{
	for (auto& window : _windows)
	{
		window->ShowWindow();
	}
}

Window& WindowManager::GetMainWindow()
{
	return *_mainWindow;
}
