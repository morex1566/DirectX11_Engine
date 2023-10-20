#pragma once
class Console
{
public:
	Console()									= default;
	Console(const Console&)						= default;
	Console& operator=(const Console&)			= default;
	Console(Console&&) noexcept					= default;
	Console& operator=(Console&&) noexcept		= default;
	~Console();

	bool Initialize();

	static void Log(const std::wstring& log_);
	static void Log(const std::string& log_);
	static void LogWarning(const std::wstring& log_);
	static void LogWarning(const std::string& log_);
	static void LogError(const std::wstring& log_);
	static void LogError(const std::string& log_);
	static void LogSuccess(const std::wstring& log_);
	static void LogSuccess(const std::string& log_);

private:
	
};