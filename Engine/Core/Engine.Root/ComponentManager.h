#pragma once

class Component;

class ComponentManager
{
	friend class Component;
	template <class T> using IsComponent = std::enable_if_t<std::is_base_of<Component, T>::value, T>;

public:
	ComponentManager(const ComponentManager&)					= delete;
	ComponentManager(const ComponentManager&&)					= delete;
	ComponentManager& operator=(const ComponentManager&)		= delete;
	~ComponentManager() = default;

	static ComponentManager& Get();

	void Initialize();
	/**
	 * \brief Destroy all of Component.
	 */
	void Shutdown();
	/**
	 * \brief Check Component's destroy trigger, if true, destroy it.
	 */
	void Dispose();

private:
	ComponentManager() = default;

private:
	std::vector<std::shared_ptr<Component>>			_components;
};