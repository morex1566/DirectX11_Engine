#pragma once

class GameObject;

class Component
{
public:
	Component(const GameObject* owner_);
	Component(const Component&)						= default;
	Component& operator=(const Component&)			= default;
	Component(Component&&) noexcept					= default;
	Component& operator=(Component&&) noexcept		= default;
	virtual ~Component();

	virtual void Start();
	virtual void Update();

	const GameObject*								GetOwner() const;

private:
	const GameObject* const							_owner;
};