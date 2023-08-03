#include "pch.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"

Scene::Scene(std::string name_)
	: _Name(std::move(name_)), _isDestroyed(false), _isActivated(true)
{
	SceneManager::Get()._scenes.emplace_back(this);
}

Scene::~Scene()
{
	Destroy();
}

void Scene::Start()
{
	// Create the viewport camera.
	{
		Camera* camera = new Camera;
		camera->GetTransform()->SetPosition(0, 0, -5);

		_gameObjects.emplace_back(camera);
		_viewportCamera = camera;
	}

	// Create the cube.
	{
		GameObject* cube = new GameObject;
		{
			Material* material = new Material;

			material->SetMesh(FROM_RESOURCE_PATH_TO("box.fbx"));
			material->SetTexture(FROM_RESOURCE_PATH_TO("box.jpg"));
			material->SetShader(FROM_RESOURCE_PATH_TO("texture.vs"), FROM_RESOURCE_PATH_TO("texture.ps"));

			cube->AttachComponent(material);
		}

		_gameObjects.emplace_back(cube);
		
	}
}

void Scene::Update()
{
	for (const auto& gameObject : _gameObjects)
	{
		for (const auto& component : gameObject->GetComponents())
		{
			component->Update();
		}
	}
}

void Scene::Render()
{
	_viewportCamera->Render();

	for (const auto& gameObject : _gameObjects)
	{
		for (const auto& material : gameObject->GetComponents<Material>())
		{
			material->Render();
		}
	}
}

void Scene::Destroy()
{
	_isDestroyed = true;
	_isActivated = false;

	for (const auto& gameObject : _gameObjects)
	{
		gameObject->Destroy();
	}
}

bool Scene::IsDestroyed()
{
	return _isDestroyed;
}

bool Scene::IsActivated()
{
	return _isActivated;
}

Camera* Scene::GetViewportCamera()
{
	return _viewportCamera;
}

GameObject* Scene::AddHierarchy(GameObject* gameObject_)
{
	_gameObjects.emplace_back(gameObject_);

	for (const auto& child : gameObject_->GetChildren())
	{
		_gameObjects.emplace_back(child);
	}

	return _gameObjects.back();
}
