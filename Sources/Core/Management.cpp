#include "PCH.h"
#include "Management.h"

Management::Management()
{
	_isEnabled			= false;
	_isActivated		= false;
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

void Management::SetActive(bool toggle_)
{
	_isActivated = toggle_;
}
