#pragma once

#include "Component.h"

class GameObject : public IEnable, public ITag, public IName
{
public:
	GameObject();
	GameObject(const GameObject&)						= default;
	GameObject& operator=(const GameObject&)			= default;
	GameObject(GameObject&&) noexcept					= default;
	GameObject& operator=(GameObject&&) noexcept		= default;
	~GameObject() override  							= default;


public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;
	Component* AttachComponent(Component* attachedComponent);
	bool DetachComponent(Component* detachedComponent);
	template <typename T> T* FindComponent();
	

protected:
	std::vector<Component*> components;


};

// 해당 타입의 컴포넌트를 찾아서 반환합니다.
template<typename T>
inline T* GameObject::FindComponent()
{
	for (Component* component : components)
	{
		T* typedComponent = dynamic_cast<T*>(component);
		if (typedComponent)
		{
			return typedComponent;
		}
	}

	Console::LogWarning(L"component, '" + Utls::ToWString(typeid(T).name()) + L"' is not found."
						, __FILE__, __LINE__);
	return nullptr;
}
