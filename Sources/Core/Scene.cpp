#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	Scene::Destroy();
}

void Scene::Destroy()
{
	IManageable::Destroy();

	for (auto& gameObject : _gameObjects)
	{
		gameObject->Destroy();
	}
}

std::string Scene::GetName()
{
	return _name;
}

void Scene::SetName(const std::string& name_)
{
	_name = name_;
}

void Scene::AddHierarchy(const std::shared_ptr<GameObject>& gameObject_)
{
	_gameObjects.push_back(gameObject_);
}

std::vector<std::shared_ptr<GameObject>> Scene::GetHierarchy()
{
	return _gameObjects;
}

std::shared_ptr<Light> Scene::GetEnviromentLight()
{
	return _envLight;
}

void Scene::SetEnviromentLight(const std::shared_ptr<Light>& light_)
{
	AddHierarchy(light_);
	_envLight = light_;
}
