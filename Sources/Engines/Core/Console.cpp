#include "PCH.h"
#include "Console.h"
#include "System.h"

bool		Console::isConsoleEnabled;

Console::Console()
{
	isConsoleEnabled = Config::GetConfig()["Console"]["IsConsoleEnabled"];
}

void __stdcall Console::MsgHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
	if (!isConsoleEnabled)
	{
		return;
	}

}

void Console::Init()
{
	if (!isConsoleEnabled)
	{
		return;
	}

	FILE* Stream;
	errno_t		Error;

	AllocConsole();

	hWindow = GetConsoleWindow();
	if (hWindow == nullptr)
	{
		throw std::exception();
	}

	Error = freopen_s(&Stream, "CONOUT$", "w", stdout);
	if (Error)
	{
		throw std::exception();
	}
}

void Console::Shutdown()
{
	if (!isConsoleEnabled)
	{
		return;
	}

	DestroyWindow(hWindow);
}

void Console::Log(const std::wstring& Log)
{
	if (!isConsoleEnabled)
	{
		return;
	}

	// 흰색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 7);
	}

	std::wcout << L"[" << Utls::ToWString(Utls::GetTime()) << L"]"
		<< L"[Log] " << Log << std::endl;
}

void Console::LogSuccess(const std::wstring& Log)
{
	if (!isConsoleEnabled)
	{
		return;
	}

	// 흰색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 10);
	}

	std::wcout << L"[" << Utls::ToWString(Utls::GetTime()) << L"]"
		<< L"[LogSuccess] " << Log << std::endl;
}

void Console::LogError(const std::wstring& Log, const std::string& inFIleName, int inLine)
{
	if (!isConsoleEnabled)
	{
		return;
	}

	// 붉은색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
	}

	std::wcout << L"[" << Utls::ToWString(Utls::GetTime()) << L"]"
		<< L"[LogError]" << 
		L"[" << Utls::ToWString(inFIleName) << L"]" <<
		L"[" << Utls::ToWString(std::to_string(inLine)) << L"] " <<
		Log 
		<< std::endl;

}
