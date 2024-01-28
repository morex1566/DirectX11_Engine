#pragma once

class SConsole
{
public:
	SConsole(const SConsole&)					= delete;
	SConsole& operator=(const SConsole&)		= delete;
	SConsole(SConsole&&) noexcept				= delete;
	SConsole& operator=(SConsole&&) noexcept	= delete;
	~SConsole()									= default;

	static SConsole&							GetInstance();

	void										Init();
	void										Shutdown();

	static void									Log(const std::wstring& Log);
	static void									Log(const std::string& Log);
	static void									LogWarning(const std::wstring& Log);
	static void									LogWarning(const std::string& Log);
	static void									LogError(const std::wstring& Log);
	static void									LogError(const std::string& Log);

	const HWND&									GetHWnd() const;

private:
	SConsole()									= default;

	HWND										HWnd;
};