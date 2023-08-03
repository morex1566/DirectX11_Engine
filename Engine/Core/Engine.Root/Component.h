#pragma once

class GameObject;

class Component
{
	friend class GameObject;

public:
	Component();
	Component(GameObject* owner_);

	virtual ~Component();

	virtual void Update();
	virtual void Destroy();

	bool IsDestroyed();
	bool IsActivated();

	GameObject* GetOwner();

public:
	std::string _Name;
	
private:
	GameObject*									_owner;
	bool										_isDestroyed;
	bool										_isActivated;

};