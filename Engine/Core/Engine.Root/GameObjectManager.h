#pragma once

class GameObject;

class GameObjectManager
{
	friend class GameObject;
	friend class Scene;
	template <class T> using IsGameObject = std::enable_if_t<std::is_base_of<GameObject, T>::value, T>;

public:
	GameObjectManager(const GameObjectManager&)						= delete;
	GameObjectManager(const GameObjectManager&&)					= delete;
	GameObjectManager& operator=(const GameObjectManager&)			= delete;

	static GameObjectManager& Get();

	void Initialize();
	/**
	 * \brief Destroy all of GameObject.
	 */
	void Shutdown();
	/**
	 * \brief Check GameObject's destroy trigger, if true, destroy it.
	 */
	void Dispose();

private:
	GameObjectManager() = default;

private:
	std::vector<std::shared_ptr<GameObject>>		_gameObjects;
};
