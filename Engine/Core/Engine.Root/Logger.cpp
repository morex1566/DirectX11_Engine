// ReSharper disable CppClangTidyCertErr33C

#include "pch.h"
#include "Logger.h"

Logger::~Logger()
{
	::FreeConsole();
}

Logger& Logger::Get()
{
	if(_instance == nullptr)
	{
		_instance = new Logger;
		_instance->initialize();
	}

	return *_instance;
}

void Logger::Log(const std::wstring& msg_)
{
	std::wcout << msg_;
}

void Logger::initialize()
{
	::AllocConsole();

	::freopen_s(&_pFile, "CONOUT$", "w", stdout);
	// Prevent save string at _pFile.
	::freopen_s(&_pFile, "CON", "w", stdout);
}

Logger* Logger::_instance;