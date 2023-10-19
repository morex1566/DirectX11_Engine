#include "PCH.h"
#include "Object.h"

Object::Object()
{
	_isEnabled			= false;
	_isActivated		= false;
}

Object::~Object()
{
	_isEnabled			= false;
	_isActivated		= false;
}

bool Object::CheckIsEnabled() const
{
	return _isEnabled;
}

bool Object::CheckIsActivated() const
{
	return _isActivated;
}

void Object::SetActive(bool toggle_)
{
	_isActivated = toggle_;
}
