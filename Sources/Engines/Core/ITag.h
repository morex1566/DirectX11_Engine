#pragma once

class ITag
{
public:
	ITag()				= default;
	virtual ~ITag()		= default;

public:
	std::wstring Tag = L"";
};