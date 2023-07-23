#pragma once

class Window
{
public:
	Window(WNDPROC windowProc_, std::string name_, std::string title_, HINSTANCE hInstance_);

	void Initialize();
	void Shutdown();

	void ShowWindow() const;

private:
	HWND createHWND();

public:
	WNDPROC 											_WindowProc;
	std::string											_Name;
	std::string											_Title;
	HINSTANCE											_HInstance;
	HWND												_Hwnd;
};