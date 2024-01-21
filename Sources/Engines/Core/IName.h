#pragma once

class IName
{
public:
	IName()			 = default;
	virtual ~IName() = default;

public:
	std::wstring Name = L"";
};