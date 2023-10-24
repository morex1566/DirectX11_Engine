#pragma once
#include "Component.h"

class GameObject : public Management	
{
public:
	GameObject()										= default;
	GameObject(const GameObject&)						= default;
	GameObject& operator=(const GameObject&)			= default;
	GameObject(GameObject&&) noexcept					= default;
	GameObject& operator=(GameObject&&) noexcept		= default;
	virtual ~GameObject() override;

	virtual void Start();
	virtual void Update();

	template <class T>
	T&													AddComponent();
	void												DeleteComponent();

	template <class T>
	std::vector<Component&>								FindComponents();

private:
	std::vector<Component>								_components;
	std::vector<GameObject>								_children;
	GameObject*											_parent;
};

template <class T>
T& GameObject::AddComponent()
{
	return _components.emplace_back(T());
}

template <class T>
std::vector<Component&> GameObject::FindComponents()
{
	return std::vector<Component&>();
}
