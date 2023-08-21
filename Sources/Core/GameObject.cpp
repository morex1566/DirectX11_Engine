#include "pch.h"
#include "GameObject.h"
#include "Component.h"

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
	_components.push_back(std::make_shared<Component>(*component_));

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

std::string GameObject::GetName()
{
	return _name;
}

void GameObject::SetName(const std::string& name_)
{
	_name = name_;
}
