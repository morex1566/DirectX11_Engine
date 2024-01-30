#pragma once
#include <stdint.h>

class IEnable
{
public:
	IEnable()									= default;
	IEnable(const IEnable&)						= default;
	IEnable& operator=(const IEnable&)			= default;
	IEnable(IEnable&&) noexcept					= default;
	IEnable& operator=(IEnable&&) noexcept		= default;
	virtual ~IEnable()							= default;


public:
	int8_t	IsEnable = 1;


};