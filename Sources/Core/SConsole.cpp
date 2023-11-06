#include "PCH.h"
#include "SApplication.h"
#include "SConsole.h"

SConsole& SConsole::GetInstance()
{
	static SConsole Instance;
	return Instance;
}

void SConsole::Initialize()
{
	AllocConsole();

	HWnd = GetConsoleWindow();
	if (HWnd == nullptr)
	{
		throw std::exception();
	}
}

void SConsole::Release()
{
	DestroyWindow(HWnd);
}

void SConsole::Log(const std::wstring& Log)
{
	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]"
		<< L"[Log] " << Log << std::endl;
}

void SConsole::Log(const std::string& Log)
{
	std::cout << "[" << GetCurrentTimeAsString() << "]"
		<< "[Log] " << Log << std::endl;
}

// TODO : 1. Change color to orange.
void SConsole::LogWarning(const std::wstring& Log)
{
	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]"
		<< L"[LogWarning] " << Log << std::endl;
}

// TODO : 1. Change color to orange.
void SConsole::LogWarning(const std::string& Log)
{
	std::cout << "[" << GetCurrentTimeAsString() << "]"
		<< "[LogWarning] " << Log << std::endl;
}

// TODO : 1. Block compile and save.
// TODO : 2. Change color to red.
void SConsole::LogError(const std::wstring& Log)
{
	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]"
		<< L"[LogError] " << Log << std::endl;

	SApplication::Quit();
}

// TODO : 1. Block compile and save
// TODO : 2. Change color to red.
void SConsole::LogError(const std::string& Log)
{
	std::cout << "[" << GetCurrentTimeAsString() << "]"
		<< "[LogError] " << Log << std::endl;

	SApplication::Quit();
}

const HWND& SConsole::GetHWnd() const
{
	return HWnd;
}