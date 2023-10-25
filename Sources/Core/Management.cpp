#include "PCH.h"
#include "Management.h"

Management::Management()
{
	_isEnabled			= true;
	_isActivated		= true;
}

Management::~Management()
{
	_isEnabled			= false;
	_isActivated		= false;
}

bool Management::CheckIsEnabled() const
{
	return _isEnabled;
}

bool Management::CheckIsActivated() const
{
	return _isActivated;
}

void Management::SetIsActivated(bool toggle_)
{
	_isActivated = toggle_;
}

void Management::SetIsEabled(bool toggle_)
{
	_isEnabled = toggle_;
}
