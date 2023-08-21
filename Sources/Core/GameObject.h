#pragma once

class Component;

class GameObject : public IManageable
{
public:
	GameObject()							= default;
	GameObject(const GameObject&)			= default;
	~GameObject() override					= default;

	void Destroy() override;

	Component*					AttachComponent(Component* component_);
	void						DetachComponent(const Component* component_);
	template <class T, class = IsBaseOf<T, Component>>
	T*							GetComponent();
	template <class T, class = IsBaseOf<T, Component>>
	std::vector<T*>				GetComponents();
	std::string					GetName();
	void						SetName(const std::string& name_);
private:
	std::string									_name;
	std::vector<std::shared_ptr<Component>>		_components;
};

template <class T, class>
T* GameObject::GetComponent()
{
	for (auto& component : _components)
	{
		if (T* rc = dynamic_cast<T*>(component.get()))
		{
			return rc;
		}
	}

	return nullptr;
}

template <class T, class>
std::vector<T*> GameObject::GetComponents()
{
	std::vector<T*> rcv;

	for (auto& component : _components)
	{
		if (T* rc = dynamic_cast<T*>(component.get()))
		{
			rcv.push_back(rc);
		}
	}

	return rcv;
}