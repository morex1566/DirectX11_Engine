#pragma once

class GameObject;
class Camera;

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

	Camera* GetViewportCamera();
	GameObject* AddHierarchy(GameObject* gameObject_);

public:
	std::string					_Name;

private:
	std::vector<GameObject*>					_gameObjects;
	Camera*										_viewportCamera;
	bool										_isDestroyed;
	bool										_isActivated;
	
};