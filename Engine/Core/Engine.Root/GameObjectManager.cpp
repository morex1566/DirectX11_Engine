#include "pch.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "ComponentManager.h"


GameObjectManager& GameObjectManager::Get()
{
	static GameObjectManager instance;
	return instance;
}

void GameObjectManager::Initialize()
{

}

void GameObjectManager::Shutdown()
{
	for (const auto& gameObject : _gameObjects)
	{
		gameObject->Destroy();
	}
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