#pragma once

class GameObject;

class Component
{
public:
	Component(const GameObject* owner_);
	Component(const Component&)						= delete;
	Component& operator=(const Component&)			= delete;
	Component(Component&&) noexcept					= delete;
	Component& operator=(Component&&) noexcept		= delete;
	virtual ~Component();

	virtual void Start();
	virtual void Update();

	const GameObject*								GetOwner() const;

private:
	const GameObject* const							_owner;
};