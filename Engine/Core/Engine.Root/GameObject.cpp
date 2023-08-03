#include "pch.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"

GameObject::GameObject()
	: _Name(std::string()), _parent(nullptr), _isDestroyed(false), _isActivated(true)
{
	GameObjectManager::Get()._gameObjects.emplace_back(this);

	AttachComponent(new Transform);
}

GameObject::~GameObject()
{
}

Component* GameObject::FindComponent(const std::string& name_)
{
	for (const auto& component : _components)
	{
		if(component->_Name == name_)
		{
			return component;
		}
	}

	return nullptr;
}

std::vector<GameObject*>& GameObject::GetChildren()
{
	return _children;
}

std::vector<Component*>& GameObject::GetComponents()
{
	return _components;
}

void GameObject::AttachChild(GameObject* gameObject_)
{
	// Check is the 'gameObject_' already child.
	for (const auto& child : _children)
	{
		if(child == gameObject_)
		{
			LogManager::LogWarning("GameObject is already chlid.");
			return;
		}
	}

	// Attach 'gameObject_' to _children. 
	_children.emplace_back(gameObject_);
}

void GameObject::DetachChild(const GameObject* gameObject_)
{
	for(auto it = _children.begin(); it != _children.end();)
	{
		if(*it == gameObject_)
		{
			it = _children.erase(it);
			return;
		}
		else
		{
			++it;
		}
	}
}

void GameObject::SetParent(GameObject* gameObject_)
{
	_parent = gameObject_;
}

void GameObject::AttachComponent(Component* component_)
{
	component_->_owner = this;
	_components.emplace_back(component_);
}

void GameObject::DetachComponent(const Component* component_)
{
	for (auto it = _components.begin(); it != _components.end();)
	{
		if (*it == component_)
		{
			it = _components.erase(it);
			return;
		}
		else
		{
			++it;
		}
	}
}

Transform* GameObject::GetTransform()
{
 	return GetComponent<Transform>();
}

void GameObject::Destroy()
{
	_isDestroyed = true;
	_isActivated = false;

	for (const auto& child : _children)
	{
		child->Destroy();
	}

	for (const auto& component : _components)
	{
		component->Destroy();
	}
}

bool GameObject::IsDestroyed()
{
	return _isDestroyed;
}

bool GameObject::IsActivated()
{
	return _isActivated;
}
