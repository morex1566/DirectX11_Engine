#pragma once

#include "Scene.h"

class SceneManager
{
	friend class Scene;

public:
	SceneManager(const SceneManager&)					= delete;
	SceneManager(const SceneManager&&)					= delete;
	SceneManager& operator=(const SceneManager&)		= delete;
	~SceneManager() = default;

	static SceneManager& Get();

	void Initialize();
	/**
	 * \brief Destroy all of Scene.
	 */
	void Shutdown();
	/**
	 * \brief Check Scene's destroy trigger, if true, destroy it.
	 */
	void Dispose();


	Scene* LoadScene(const std::string& name_);
	Scene* GetCurrentScene();

private:
	SceneManager() = default;

private:
	Scene*											_currentScene;
	std::vector<std::shared_ptr<Scene>>				_scenes;
};