#pragma once
#include "Object.h"
#include "OComponent.h"

class CTransform;
class GCamera;
class OWindow;
class ODirectX11;

class OGameObject : public Object
{
public:
	OGameObject();
	OGameObject(const OGameObject&)									= default;
	OGameObject& operator=(const OGameObject&)						= default;
	OGameObject(OGameObject&&) noexcept								= default;
	OGameObject& operator=(OGameObject&&) noexcept					= default;
	virtual ~OGameObject() override;

	virtual void													Initialize() override;
	virtual void													Release() override;

	/**
	 * \brief Called only once before entering the main loop.
	 */
	virtual void													Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	virtual void													Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	virtual void													End() override;

	template <typename T, typename ...Args>
	T*																TAddComponent(Args&&... InConstructorArgs);
	template <typename T>
	void															TDeleteComponent();
	template <typename T>
	T*																TFindComponent() const;
	template <typename T>
	std::vector<T*>													TFindComponents() const;

	CTransform*														GetTransform() const;

protected:
	CTransform*														Transform;
	const OGameObject*												Parent;
	std::vector<std::shared_ptr<OComponent>>						Components;
	std::vector<std::shared_ptr<OGameObject>>						Children;

private:
	
};

template <typename T, typename ...Args>
T* OGameObject::TAddComponent(Args&&... InConstructorArgs)
{
	// T(Args...)
	Components.emplace_back(std::make_shared<T>(std::move(InConstructorArgs)...));

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