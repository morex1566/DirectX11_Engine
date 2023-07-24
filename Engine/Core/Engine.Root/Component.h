#pragma once

class Component
{
	friend class ComponentManager;

public:
	Component();
	virtual ~Component();

	virtual void Update();
	void Destroy();

	bool IsDestroyed();
	bool IsActivated();

public:
	std::string _Name;

private:
	bool										_isDestroyed;
	bool										_isActivated;

};