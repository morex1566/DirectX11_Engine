#pragma once

class Component : public IEnable
{
public:
	Component();
	Component(const Component&)						= default;
	Component& operator=(const Component&)			= default;
	Component(Component&&) noexcept					= default;
	Component& operator=(Component&&) noexcept		= default;
	virtual ~Component() override  					= default;

};

