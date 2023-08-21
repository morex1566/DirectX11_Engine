#pragma once

#include "Scene.h"

class SceneManager : public ISingleton<SceneManager>, public IManager<Scene>
{
public:
	SceneManager();
	~SceneManager() override;

	std::shared_ptr<Scene> GetCurrentScene();
	std::shared_ptr<Scene> LoadScene(const std::string& name_);
	std::shared_ptr<Scene> FindScene(const std::string& name_);

private:
	std::shared_ptr<Scene> _currScene;
};

