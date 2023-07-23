// ReSharper disable CppClangTidyCertErr33C

#include "pch.h"
#include "LogManager.h"
#include "Application.h"

LogManager& LogManager::Get()
{
	static LogManager instance;
	return instance;
}

void LogManager::Log(const std::wstring& msg_)
{
	std::wcout << msg_;
}

void LogManager::LogError(const std::wstring& msg_)
{
	std::wcout << msg_;
	Application::Get().Shutdown();
}

void LogManager::LogError(const std::string& msg_)
{
	std::cout << msg_;
	Application::Get().Shutdown();
}

void LogManager::Initialize()
{
	::AllocConsole();

	::freopen_s(&_pFile, "CONOUT$", "w", stdout);
	// Prevent save string at _pFile.
	::freopen_s(&_pFile, "CON", "w", stdout);

}

void LogManager::Shutdown()
{
	::FreeConsole();
}