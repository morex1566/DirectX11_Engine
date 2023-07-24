#include "pch.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "Scene.h"

GameObjectManager& GameObjectManager::Get()
{
	static GameObjectManager instance;
	return instance;
}

void GameObjectManager::Initialize()
{
	Scene* currentScene = SceneManager::Get().GetCurrentScene();
	{
		currentScene->AddHierarchy(new Camera);
		currentScene->AddHierarchy(new GameObject);
	}
}

void GameObjectManager::Shutdown()
{
}

void GameObjectManager::Dispose()
{
	for (auto it = _gameObjects.begin(); it != _gameObjects.end();)
	{
		if (it->get()->IsDestroyed())
		{
			it->reset();
			it = _gameObjects.erase(it);
		}
		else
		{
			++it;
		}
	}
}