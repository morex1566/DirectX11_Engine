#pragma once

class GameObject;

class Scene
{
	friend class SceneManager;

public:
	Scene(std::string name_);
	~Scene();

	void Start();
	void Update();
	void Render();
	void Destroy();

	bool IsDestroyed();
	bool IsActivated();

	GameObject* AddHierarchy(GameObject* gameObject_);

public:
	std::string					_Name;

private:
	std::vector<GameObject*>					_gameObjects;
	bool										_isDestroyed;
	bool										_isActivated;

};