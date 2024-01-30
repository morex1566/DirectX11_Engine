#pragma once
#pragma comment(lib, "Dbghelp.lib")

class SConsole
{
public:
	SConsole(const SConsole&)					= delete;
	SConsole& operator=(const SConsole&)		= delete;
	SConsole(SConsole&&) noexcept				= delete;
	SConsole& operator=(SConsole&&) noexcept	= delete;
	~SConsole()									= default;


public:
	static SConsole&	GetInstance();


public:
	void				Init();
	void				Shutdown();
	static void			Log(const std::wstring& Log);
	static void			Log(const std::string& Log);
	static void			LogWarning(const std::wstring& Log, const std::string& inFIleName, int inLine);
	static void			LogWarning(const std::string& Log, const std::string& inFIleName, int inLine);
	static void			LogError(const std::wstring& Log, const std::string& inFIleName, int inLine);
	static void			LogError(const std::string& Log, const std::string& inFIleName, int inLine);
	const HWND&			GetHWnd() const;


private:
	SConsole()			= default;
	static bool			IsInternalFunction(const std::string& functionName);
	static void			LogCallStack();


private:
	static uint8		bIsConsoleEnabled;

private:
	HWND				HWnd;
};