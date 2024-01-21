#pragma once

#include <iostream>

class Console
{
public:
	Console()									= default;
	Console(const Console&)						= delete;
	Console& operator=(const Console&)			= delete;
	Console(Console&&) noexcept					= delete;
	Console& operator=(Console&&) noexcept		= delete;
	~Console()									= default;


public:
	static void WINAPI MsgHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam);
	static void	Log(const std::wstring& Log);
	static void	LogSuccess(const std::wstring& Log);
	static void	LogError(const std::wstring& Log, const std::string& inFIleName, int inLine);


public:
	void Init();
	void Shutdown();


private:
	static bool isConsoleEnabled;


private:
	HWND			hWindow;

};

