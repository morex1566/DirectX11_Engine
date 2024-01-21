#pragma once

class Component;

class GameObject : public IEnable, public ITag, public IName
{
public:
	GameObject();
	GameObject(const GameObject&)						= default;
	GameObject& operator=(const GameObject&)			= default;
	GameObject(GameObject&&) noexcept					= default;
	GameObject& operator=(GameObject&&) noexcept		= default;
	virtual ~GameObject() override  					= default;


public:
	void Start();
	void Update();
	void Shutdown();
	void AttachComponent(Component* attachedComponent);
	bool DetachComponent(Component* detachedComponent);
	template <typename T> T* FindComponent();
	

private:
	std::vector<Component> components;
};

template<typename T>
inline T* GameObject::FindComponent()
{

	return nullptr;
}
