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

void Scene::AddHierarchy(GameObject* gameObject_)
{
	_gameObjects.push_back(gameObject_);
}