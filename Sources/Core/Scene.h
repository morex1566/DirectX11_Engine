#pragma once

#include "GameObjectManager.h"

class Scene : public IManageable
{
public:
	Scene();
	~Scene() override;

	void Destroy() override;

	std::string									GetName();
	void										SetName(const std::string& name_);
	void										AddHierarchy(const std::shared_ptr<GameObject>& gameObject_);
	std::vector<std::shared_ptr<GameObject>>	GetHierarchy();
	std::shared_ptr<Light>						GetEnviromentLight();
	void										SetEnviromentLight(const std::shared_ptr<Light>& light_);

private:
	std::string										_name;
	std::shared_ptr<Light>							_envLight;
	std::vector<std::shared_ptr<GameObject>>		_gameObjects;
};

