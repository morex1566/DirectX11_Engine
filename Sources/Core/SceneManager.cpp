#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager()
	: _currScene(nullptr)
{
}

SceneManager::~SceneManager()
{
	
}

Scene* SceneManager::LoadScene(const std::string& name_)
{
	Scene* loadedScene = FindScene(name_);

	if (loadedScene)
	{
		_currScene = loadedScene;
	}
	else
	{
		
	}

	return _currScene;
}

Scene* SceneManager::LoadScene(const Scene* scene_)
{
	Scene* loadedScene = FindScene(scene_);

	if (loadedScene)
	{
		_currScene = loadedScene;
	}
	else
	{

	}

	return _currScene;
}

Scene* SceneManager::FindScene(const std::string& name_)
{
	for (auto& scene : _objects)
	{
		if (scene->GetName() == name_)
		{
			return scene.get();
		}
	}

	return nullptr;
}

Scene* SceneManager::FindScene(const Scene* scene_)
{
	for (auto& scene : _objects)
	{
		if (scene.get() == scene_)
		{
			return scene.get();
		}
	}

	return nullptr;
}
