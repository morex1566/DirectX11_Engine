#pragma once

class IEnable
{
public:
	IEnable()
	{
		IsEnable = true;
	}

	virtual ~IEnable() = default;

public:
	bool IsEnable;
};