#pragma once
#include <string>

class IName
{
public:
	IName()									= default;
	IName(const IName&)						= default;
	IName& operator=(const IName&)			= default;
	IName(IName&&) noexcept					= default;
	IName& operator=(IName&&) noexcept		= default;
	virtual ~IName()						= default;


public:
	std::wstring Name = L"";


};