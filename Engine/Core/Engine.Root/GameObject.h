#pragma once

class Component;
class Transform;

class GameObject
{
	friend class Component;
	template <class T> using IsComponent = std::enable_if_t<std::is_base_of<Component, T>::value, T>;

public:
	GameObject();
	virtual ~GameObject();

	Component*					FindComponent(const std::string& name_);
	std::vector<GameObject*>&	GetChildren();
	std::vector<Component*>&	GetComponents();
	template <class T, class = IsComponent<T>> std::vector<T*> GetComponents();
	template <class T, class = IsComponent<T>> T* GetComponent();

	void						AttachChild(GameObject* gameObject_);
	void						DetachChild(const GameObject* gameObject_);
	void						SetParent(GameObject* gameObject_);
	void						AttachComponent(Component* component_);
	void						DetachComponent(const Component* component_);
	Transform*					GetTransform();
	virtual void Destroy();
	bool IsDestroyed();
	bool IsActivated();

public:
	std::string					_Name;

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
		if (T* t = dynamic_cast<T*>(component))
		{
			components.emplace_back(t);
		}
	}

	return components;
}

template <class T, class> T* GameObject::GetComponent()
{
	for (auto& component: _components)
	{
		if (T* t = dynamic_cast<T*>(component))
		{
			return t;
		}
	}

	return nullptr;
}