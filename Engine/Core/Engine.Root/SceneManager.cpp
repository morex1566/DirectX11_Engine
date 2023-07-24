#include "pch.h"
#include "SceneManager.h"

SceneManager& SceneManager::Get()
{
	static SceneManager instance;
	return instance;
}

void SceneManager::Initialize()
{
	Scene* scene1 = new Scene("Untitled1");

	_currentScene = LoadScene("Untitled1");
}

void SceneManager::Shutdown()
{
	for (const auto& scene : _scenes)
	{
		scene->Destroy();
	}
}

void SceneManager::Dispose()
{
	for (auto it = _scenes.begin(); it != _scenes.end();)
	{
		if (it->get()->IsDestroyed())
		{
			it->reset();
			it = _scenes.erase(it);
		}
		else
		{
			++it;
		}
	}
}

Scene* SceneManager::LoadScene(const std::string& name_)
{
	for (const auto& scene : _scenes)
	{
		if(scene->_Name == name_)
		{
			if(_currentScene != nullptr)
			{
				_currentScene->Destroy();
			}

			return scene.get();
		}
	}

	return nullptr;
}

Scene* SceneManager::GetCurrentScene()
{
	return _currentScene;
}