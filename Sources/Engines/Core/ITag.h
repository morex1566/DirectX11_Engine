#pragma once
#include <string>

class ITag
{
public:
	ITag()									= default;
	ITag(const ITag&)						= default;
	ITag& operator=(const ITag&)			= default;
	ITag(ITag&&) noexcept					= default;
	ITag& operator=(ITag&&) noexcept		= default;
	virtual ~ITag()							= default;


public:
	std::wstring Tag = L"";


};