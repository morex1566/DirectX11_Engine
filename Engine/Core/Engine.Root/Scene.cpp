#include "pch.h"
#include "Scene.h"

#include "GameObject.h"
#include "SceneManager.h"

Scene::Scene(std::string name_)
	: _Name(std::move(name_))
{
	SceneManager::Get()._scenes.emplace_back(this);
}

Scene::~Scene()
{
	Destroy();
}

void Scene::Update()
{
}

void Scene::Render()
{
}

void Scene::Destroy()
{
	_isDestroyed = true;
	_isActivated = false;

	for (const auto& gameObject : _gameObjects)
	{
		gameObject->Destroy();
	}
}

bool Scene::IsDestroyed()
{
	return _isDestroyed;
}

bool Scene::IsActivated()
{
	return _isActivated;
}

GameObject* Scene::AddHierarchy(GameObject* gameObject_)
{
	_gameObjects.emplace_back(gameObject_);

	return _gameObjects.back();
}
