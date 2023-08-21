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

std::shared_ptr<Component> GameObject::AttachComponent(const std::shared_ptr<Component>& component_)
{
	std::shared_ptr<Component> component = component_;
	_components.push_back(component);

	return component;
}

void GameObject::DetachComponent(const std::shared_ptr<Component>& component_)
{
	for (auto it = _components.begin(); it != _components.end();)
	{
		if (it->get() == component_.get())
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
