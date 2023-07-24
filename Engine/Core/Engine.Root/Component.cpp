#include "pch.h"
#include "Component.h"
#include "ComponentManager.h"

Component::Component()
	: _isDestroyed(false), _isActivated(true)
{
	ComponentManager::Get()._components.emplace_back(this);
}

Component::~Component()
{
	Destroy();
}

bool Component::IsDestroyed()
{
	return _isDestroyed;
}

bool Component::IsActivated()
{
	return _isActivated;
}

void Component::Update()
{
}

void Component::Destroy()
{
	_isDestroyed = true;
	_isActivated = false;
}