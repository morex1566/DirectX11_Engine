#pragma once

class GameObject;

class Scene : public IManageable
{
public:
	Scene();
	~Scene() override;

	void Destroy() override;

	std::string		GetName();
	void			SetName(const std::string& name_);
	void			AddHierarchy(GameObject* gameObject_);

private:
	std::string						_name;
	std::vector<GameObject*>		_gameObjects;
};

