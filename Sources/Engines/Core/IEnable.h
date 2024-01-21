#pragma once

class IEnable
{
public:
	IEnable()		   = default;
	virtual ~IEnable() = default;

public:
	bool IsEnable = true;
};