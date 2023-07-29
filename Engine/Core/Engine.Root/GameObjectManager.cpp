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
	Scene* currentScene = SceneManager::Get().GetCurrentScene();
	{
		GameObject* go1 = new GameObject;
		GameObject* go2 = new GameObject;
		GameObject* go3 = new GameObject;
		GameObject* go4 = new GameObject;

		go1->AttachChild(go2);
		go1->AttachChild(go3);
		go1->AttachChild(go3);
		go1->AttachChild(go4);

		currentScene->AddHierarchy(new Camera);
		currentScene->AddHierarchy(go1);

		Material* material = new Material;
		{
			material->SetMesh(FILE_PATH("box.fbx"));
		}
		go1->AttachComponent(new Material);
		
	}
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