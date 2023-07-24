#pragma once

class Component;

class GameObject
{
	friend class GameObjectManager;

public:
	GameObject();
	virtual ~GameObject();

	Component*					FindComponent(const std::string& name_);
	std::vector<GameObject*>&	GetChildren();
	std::vector<Component*>&	GetComponents();
	void						AttachChild(GameObject* gameObject_);
	void						DetachChild(const GameObject* gameObject_);
	void						SetParent(GameObject* gameObject_);
	void						AttachComponent(Component* component_);
	void						DetachComponent(const Component* component_);
	void Destroy();
	bool IsDestroyed();
	bool IsActivated();

public:
	std::string				_Name;

private:
	GameObject*					_parent;
	std::vector<GameObject*>	_children;
	std::vector<Component*>		_components;
	bool						_isDestroyed;
	bool						_isActivated;

};