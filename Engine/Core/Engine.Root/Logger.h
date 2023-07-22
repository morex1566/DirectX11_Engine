#pragma once

class Logger
{
public:
	~Logger();

	static Logger& Get();

	static void Log(const std::wstring& msg_);

private:
	Logger() = default;

	void initialize();

private:
	static Logger* _instance;
	FILE* _pFile;
};