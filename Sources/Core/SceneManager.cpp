#include "pch.h"
#include "SceneManager.h"

void SceneManager::Initialize()
{
	IManager<Scene>::Initialize();
}

void SceneManager::Update()
{
	IManager<Scene>::Update();
}

void SceneManager::Dispose()
{
	IManager<Scene>::Dispose();
}

void SceneManager::Clear()
{
	IManager<Scene>::Clear();
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return _currScene;
}

std::shared_ptr<Scene> SceneManager::LoadScene(const std::string& name_)
{
	std::shared_ptr<Scene> loadedScene = FindScene(name_);

	if (loadedScene)
	{
		if (_currScene != nullptr)
		{
			_currScene->Destroy();
		}
		_currScene = loadedScene;
	}

	return _currScene;
}

std::shared_ptr<Scene> SceneManager::FindScene(const std::string& name_)
{
	for (auto& scene : _objects)
	{
		if (scene->GetName() == name_)
		{
			return scene;
		}
	}

	return nullptr;
}