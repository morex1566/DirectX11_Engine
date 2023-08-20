#pragma once

#include "Scene.h"

class SceneManager : public ISingleton<SceneManager>, public IManager<Scene>
{
public:
	SceneManager();
	~SceneManager() override;

	Scene* LoadScene(const std::string& name_);
	Scene* LoadScene(const Scene* scene_);
	Scene* FindScene(const std::string& name_);
	Scene* FindScene(const Scene* scene_);


private:
	Scene* _currScene;
};

