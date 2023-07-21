#pragma once

class Window
{
public:
	Window(WNDPROC  windowProc_, std::wstring name_, std::wstring title_, HINSTANCE hInstance_);
	~Window() = default;

public:
	void ShowWindow() const;

private:
	HWND createHWND();

public:
	WNDPROC 											_WindowProc;
	std::wstring										_Name;
	std::wstring									    _Title;
	HINSTANCE											_HInstance;
	HWND												_Hwnd;

};