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

	bool operator==(const Component& other) const
	{
		return this == &other;
	}


};

