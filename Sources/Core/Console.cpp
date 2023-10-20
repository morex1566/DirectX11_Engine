#include "PCH.h"
#include "Console.h"

Console::~Console()
{
	DestroyWindow(_hWnd);
}


bool Console::Initialize()
{
	_hWnd = GetConsoleWindow();
	if (!_hWnd)
	{
		AllocConsole();
		_hWnd = GetConsoleWindow();
	}

	return true;
}


void Console::Log(const std::wstring& log_)
{
	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]"
		<< L"[Log] " << log_ << std::endl;
}

void Console::Log(const std::string& log_)
{
	std::cout << "[" << GetCurrentTimeAsString() << "]"
		<< "[Log] " << log_ << std::endl;
}

// TODO : 1. Change color to orange.
void Console::LogWarning(const std::wstring& log_)
{
	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]"
		<< L"[LogWarning] " << log_ << std::endl;
}

// TODO : 1. Change color to orange.
void Console::LogWarning(const std::string& log_)
{
	std::cout << "[" << GetCurrentTimeAsString() << "]"
		<< "[LogWarning] " << log_ << std::endl;
}

// TODO : 1. Block compile and save.
// TODO : 2. Change color to red.
void Console::LogError(const std::wstring& log_)
{
	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]"
		<< L"[LogError] " << log_ << std::endl;
}

// TODO : 1. Block compile and save
// TODO : 2. Change color to red.
void Console::LogError(const std::string& log_)
{
	std::cout << "[" << GetCurrentTimeAsString() << "]"
		<< "[LogError] " << log_ << std::endl;
}

// TODO : 1. Change color to green.
void Console::LogSuccess(const std::wstring& log_)
{
	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]"
		<< L"[LogSuccess] " << log_ << std::endl;
}

// TODO : 1. Change color to green.
void Console::LogSuccess(const std::string& log_)
{
	std::cout << "[" << GetCurrentTimeAsString() << "]"
		<< "[LogSuccess] " << log_ << std::endl;
}


HWND Console::GetHWnd()
{
	return _hWnd;
}
