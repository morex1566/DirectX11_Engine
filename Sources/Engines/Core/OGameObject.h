#pragma once
#include "Object.h"
#include "OComponent.h"

class CTransform;
class OWorld;

class OGameObject : public Object, public IName, public IEnable, public ITag
{
public:
	// T가 Component를 상속받는지 확인
	template <class T> using IsComponent = std::enable_if_t<std::is_base_of<OComponent, T>::value>;


public:
	OGameObject();
	OGameObject(const OGameObject&)							= default;
	OGameObject& operator=(const OGameObject&)				= default;
	OGameObject(OGameObject&&) noexcept						= default;
	OGameObject& operator=(OGameObject&&) noexcept			= default;
	virtual ~OGameObject() override;


public:
	virtual void					Init() override;
	virtual void					Shutdown() override;
	virtual void					Start() override;
	virtual void					Tick() override;
	virtual void					End() override;


public:
	template <typename T, typename ...Args>
	T*								TAddComponent(Args&&... InConstructorArgs);
	template <typename T>
	void							TDeleteComponent();
	template <typename T>
	T*								TFindComponent() const;
	template <typename T>
	std::vector<T*>					TFindComponents() const;
	FORCEINLINE CTransform*			GetTransform() const { return Transform; }
	FORCEINLINE OWorld*				GetWorld() const { return World; }
	FORCEINLINE void				SetWorld(OWorld* InWorld) { World = InWorld; }


protected:
	CTransform*									Transform;
	const OGameObject*							Parent;
	OWorld*										World;
	std::vector<std::shared_ptr<OComponent>>	Components;
	std::vector<std::shared_ptr<OGameObject>>	Children;


};

template <typename T, typename ...Args>
T* OGameObject::TAddComponent(Args&&... InConstructorArgs)
{
	// T(Args...)
	Components.emplace_back(std::make_shared<T>(std::move(this), std::forward<Args>(InConstructorArgs)...));

	return static_cast<T*>(Components.back().get());
}

template <typename T>
void OGameObject::TDeleteComponent()
{
	auto it = Components.begin();
	while (it != Components.end())
	{
		if (T* TComponent = dynamic_cast<T*>(it->get()))
		{
			it->reset();
			it = Components.erase(it);

			break;
		}
		else
		{
			++it;
		}
	}
}

template <typename T>
T* OGameObject::TFindComponent() const
{
	for (auto& Component : Components)
	{
		if (T* TComponent = dynamic_cast<T*>(Component.get()))
		{
			return TComponent;
		}
	}

	return nullptr;
}

template <class T>
std::vector<T*> OGameObject::TFindComponents() const
{
	std::vector<T*> TComponents;

	for (auto& Component : Components)
	{
		if (T* TComponent = dynamic_cast<T*>(Component.get()))
		{
			TComponents.emplace_back(TComponent);
		}
	}

	return TComponents;
}