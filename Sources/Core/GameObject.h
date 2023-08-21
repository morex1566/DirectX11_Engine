#pragma once

class Component;

class GameObject : public IManageable
{
public:
	GameObject()							= default;
	GameObject(const GameObject&)			= default;
	~GameObject() override					= default;

	void Destroy() override;

	std::shared_ptr<Component>		AttachComponent(const std::shared_ptr<Component>& component_);
	void							DetachComponent(const std::shared_ptr<Component>& component_);
	template <class T, class = IsBaseOf<T, Component>>
	std::shared_ptr<T>				GetComponent();
	template <class T, class = IsBaseOf<T, Component>>
	std::vector<std::shared_ptr<T>>	GetComponents();
	std::string						GetName();
	void							SetName(const std::string& name_);
private:
	std::string									_name;
	std::vector<std::shared_ptr<Component>>		_components;
};

template <class T, class>
std::shared_ptr<T> GameObject::GetComponent()
{
	for (auto& component : _components)
	{
		if (std::shared_ptr<T> rc = std::dynamic_pointer_cast<T>(component))
		{
			return rc;
		}
	}

	return nullptr;
}

template <class T, class>
std::vector<std::shared_ptr<T>> GameObject::GetComponents()
{
	std::vector<std::shared_ptr<T>> rcv;

	for (auto& component : _components)
	{
		if (std::shared_ptr<T> rc = std::dynamic_pointer_cast<T>(component))
		{
			rcv.push_back(rc);
		}
	}

	return rcv;
}