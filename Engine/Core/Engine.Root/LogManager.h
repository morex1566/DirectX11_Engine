#pragma once

class LogManager
{
public:
	LogManager(const LogManager&)				= delete;
	LogManager(const LogManager&&)				= delete;
	LogManager& operator=(const LogManager&)	= delete;

	static LogManager& Get();

	static void Log(const std::wstring& msg_);
	static void LogError(const std::wstring& msg_);
	static void LogError(const std::string& msg_);

	void Initialize();
	void Shutdown();

private:
	LogManager() = default;

private:
	FILE* _pFile;
};