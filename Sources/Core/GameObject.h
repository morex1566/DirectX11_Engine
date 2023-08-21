#pragma once

class Component;

class GameObject : public IManageable
{
public:
	GameObject();
	GameObject(const GameObject& gameObject_);
	GameObject(const GameObject&& gameObject_) noexcept;
	~GameObject() override;

	void Destroy() override;

	Component*					AttachComponent(Component* component_);
	void						DetachComponent(const Component* component_);
	template <class T, class = IsBaseOf<T, Component>>
	T*							GetComponent();
	template <class T, class = IsBaseOf<T, Component>>
	std::vector<T*>				GetComponents();

private:
	std::vector<std::shared_ptr<Component>> _components;
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