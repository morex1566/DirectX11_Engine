#include "PCH.h"
#include "OApplication.h"
#include "OConsole.h"

OConsole::OConsole()
	: Object(), HWnd(nullptr)
{
}

OConsole::~OConsole()
{
}

OConsole& OConsole::GetInstance()
{
	static OConsole Console;

	return Console;
}

Object::EHandleResultType OConsole::Initialize()
{
	AllocConsole();

	HWnd = GetConsoleWindow();
	if (HWnd == nullptr)
	{
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

void OConsole::Release()
{
	DestroyWindow(HWnd);
}

void OConsole::Log(const std::wstring& Log)
{
	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]"
		<< L"[Log] " << Log << std::endl;
}

void OConsole::Log(const std::string& Log)
{
	std::cout << "[" << GetCurrentTimeAsString() << "]"
		<< "[Log] " << Log << std::endl;
}

// TODO : 1. Change color to orange.
void OConsole::LogWarning(const std::wstring& Log)
{
	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]"
		<< L"[LogWarning] " << Log << std::endl;
}

// TODO : 1. Change color to orange.
void OConsole::LogWarning(const std::string& Log)
{
	std::cout << "[" << GetCurrentTimeAsString() << "]"
		<< "[LogWarning] " << Log << std::endl;
}

// TODO : 1. Block compile and save.
// TODO : 2. Change color to red.
void OConsole::LogError(const std::wstring& Log)
{
	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]"
		<< L"[LogError] " << Log << std::endl;

	OApplication::Quit();
}

// TODO : 1. Block compile and save
// TODO : 2. Change color to red.
void OConsole::LogError(const std::string& Log)
{
	std::cout << "[" << GetCurrentTimeAsString() << "]"
		<< "[LogError] " << Log << std::endl;

	OApplication::Quit();
}

const HWND& OConsole::GetHWnd() const
{
	return HWnd;
}