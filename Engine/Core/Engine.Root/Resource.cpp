#include "pch.h"
#include "Resource.h"
#include "ResourceManager.h"

Resource::Resource()
	: _isDestroyed(false), _isActivated(true)
{
	ResourceManager::Get()._resources.emplace_back(this);
}

Resource::~Resource()
{
}

bool Resource::IsDestroyed()
{
	return _isDestroyed;
}

bool Resource::IsActivated()
{
	return _isActivated;
}
