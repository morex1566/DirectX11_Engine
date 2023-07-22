#pragma once

#include "pch.h"

class Window;

class Application
{
public:
	~Application() = default;
	
	/**
	 * \brief Get application's singleton instance.
	 * \return application's singleton instance
	 */
	static Application& Get();
	/**
	 * \brief Create and Get Window class.
	 * \return Window class's smart pointer
	 */
	//std::unique_ptr<Window>& OpenWindow();
	/**
	 * \brief Application's Frame.
	 */
	void Update();
	/**
	 * \brief if true, application is terminated.
	 * \return _IsExited
	 */
	bool IsExited() const;
	

private:
	Application();
	/**
	 * \brief Processing below works
	 * 1. generate main window
	 * \return All of task is success, return true. else false.
	 */
	bool initialize();
	/**
	 * \brief Engine's main window procedure.
	 */
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/**
	 * \brief project's instance handle
	 */
	HINSTANCE _HInstance;

private:
	/**
	 * \brief current process windows
	 */
	static std::vector<std::unique_ptr<Window>>	_windows;
	/**
	 * \brief if true, application is terminated.
	 */
	bool _isExited;
	/**
	 * \brief  singleton instance
	 */
	static Application* _instance;
};