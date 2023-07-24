#include "pch.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"

Scene::Scene(std::string name_)
	: _Name(std::move(name_)), _isDestroyed(false), _isActivated(true)
{
	SceneManager::Get()._scenes.emplace_back(this);
}

Scene::~Scene()
{
	Destroy();
}

void Scene::Start()
{
}

void Scene::Update()
{
}

void Scene::Render()
{
	for (const auto& gameObject : _gameObjects)
	{
		for (const auto& material : gameObject->GetComponents<Material>())
		{
			material->Render();
		}
	}
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

	for (const auto& child : gameObject_->GetChildren())
	{
		_gameObjects.emplace_back(child);
	}

	return _gameObjects.back();
}