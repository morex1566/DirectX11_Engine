#pragma once

#include "ComponentManager.h"

class GameObject
{
	friend class GameObjectManager;
	template <class T> using IsComponent = std::enable_if_t<std::is_base_of<Component, T>::value, T>;

public:
	GameObject();
	virtual ~GameObject();

	Component*					FindComponent(const std::string& name_);
	std::vector<GameObject*>&	GetChildren();
	std::vector<Component*>&	GetComponents();
	template <class T, class = IsComponent<T>> std::vector<T*> GetComponents();

	void						AttachChild(GameObject* gameObject_);
	void						DetachChild(const GameObject* gameObject_);
	void						SetParent(GameObject* gameObject_);
	void						AttachComponent(Component* component_);
	void						DetachComponent(const Component* component_);
	void Destroy();
	bool IsDestroyed();
	bool IsActivated();

public:
	std::string				_Name;

private:
	GameObject*					_parent;
	std::vector<GameObject*>	_children;
	std::vector<Component*>		_components;
	bool						_isDestroyed;
	bool						_isActivated;

};

template <class T, class> std::vector<T*> GameObject::GetComponents()
{
	std::vector<T*> components;

	for (auto& component : _components)
	{
		if (typeid(component) == typeid(T))
		{
			T* t = dynamic_cast<T*>(component);
			if(t != nullptr)
			{
				components.emplace_back(t);
			}
		}
	}

	return components;
}