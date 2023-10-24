#include "PCH.h"
#include "Component.h"

Component::Component(const GameObject* owner_)
	: _owner(owner_)
{
}

Component::~Component()
{
}

void Component::Start()
{
}

void Component::Update()
{
}

const GameObject* Component::GetOwner() const
{
	return _owner;
}
