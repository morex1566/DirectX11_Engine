#pragma once

class Component : public IEnable
{
public:
	Component()										= default;
	Component(const Component&)						= default;
	Component& operator=(const Component&)			= default;
	Component(Component&&) noexcept					= default;
	Component& operator=(Component&&) noexcept		= default;
	~Component() override  							= default;

	bool operator==(const Component& other) const
	{
		return this == &other;
	}


public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;


};

