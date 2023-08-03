#include "pch.h"
#include "Component.h"
#include "ComponentManager.h"

Component::Component()
	: _isDestroyed(false), _isActivated(true)
{
	ComponentManager::Get()._components.emplace_back(this);
}

Component::Component(GameObject* owner_)
	: _isDestroyed(false), _isActivated(true)
{
	_owner = owner_;
	ComponentManager::Get()._components.emplace_back(this);
}

Component::~Component()
{
	Component::Destroy();
}

bool Component::IsDestroyed()
{
	return _isDestroyed;
}

bool Component::IsActivated()
{
	return _isActivated;
}

GameObject* Component::GetOwner()
{
	return _owner;
}

void Component::Update()
{
}

void Component::Destroy()
{
	_isDestroyed = true;
	_isActivated = false;
}