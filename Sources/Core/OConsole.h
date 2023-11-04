#pragma once
#include "Object.h"

class OConsole : public Object
{
public:
	OConsole();
	OConsole(const OConsole&)									= default;
	OConsole& operator=(const OConsole&)						= default;
	OConsole(OConsole&&) noexcept								= default;
	OConsole& operator=(OConsole&&) noexcept					= default;
	~OConsole() override;

	static OConsole&											GetInstance();

	EHandleResultType											Initialize() override;
	void														Release() override;

	static void													Log(const std::wstring& Log);
	static void													Log(const std::string& Log);
	static void													LogWarning(const std::wstring& Log);
	static void													LogWarning(const std::string& Log);
	static void													LogError(const std::wstring& Log);
	static void													LogError(const std::string& Log);

	const HWND&													GetHWnd() const;

protected:

private:
	HWND														HWnd;
};