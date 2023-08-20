#include "pch.h"
#include "IManageable.h"

IManageable::IManageable()
{
	_isEnabled = true;
	_isDestroyed = false;
}

IManageable::~IManageable()
{
	IManageable::Destroy();
}

void IManageable::Destroy()
{
	_isEnabled = false;
	_isDestroyed = true;
}

void IManageable::Disable()
{
	_isEnabled = false;
}

bool IManageable::IsDestroyed()
{
	return _isDestroyed;
}

bool IManageable::IsDisabled()
{
	return !_isEnabled;
}