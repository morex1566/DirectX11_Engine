#include "pch.h"
#include "GameObject.h"
#include "Component.h"

GameObject::GameObject()
{
}

GameObject::GameObject(const GameObject& gameObject_)
	: IManageable(gameObject_)
{
}

GameObject::GameObject(const GameObject&& gameObject_) noexcept
	: IManageable(gameObject_)
{
}

GameObject::~GameObject()
{
}

void GameObject::Destroy()
{
	IManageable::Destroy();

	for (auto& component : _components)
	{
		component->Destroy();
	}
}

Component* GameObject::AttachComponent(Component* component_)
{
	_components.push_back(shared_ptr<Component>(component_));

	return _components.back().get();
}

void GameObject::DetachComponent(const Component* component_)
{
	for (auto it = _components.begin(); it != _components.end();)
	{
		if (it->get() == component_)
		{
			it->reset();
			it = _components.erase(it);
		}
		else
		{
			++it;
		}
	}
}