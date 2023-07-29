// ReSharper disable CppUnusedIncludeDirective
#pragma once

#include "Resource.h"
#include "Texture.h"
#include "Mesh.h"

class ResourceManager : public IManager<ResourceManager>,
						public ISingleton<ResourceManager>,
						public IObjectPool<Resource>
{
	friend class Resource;

public:
	ResourceManager(const ResourceManager&)					= delete;
	ResourceManager(const ResourceManager&&)				= delete;
	ResourceManager& operator=(const ResourceManager&)		= delete;
	~ResourceManager() override								= default;

	static ResourceManager& Get();

	void Initialize();
	/**
	 * \brief Destroy all of Resource.
	 */
	void Shutdown();
	/**
	 * \brief Check Resource's destroy trigger, if true, destroy it.
	 */
	void Dispose();

private:
	ResourceManager();

private:
	std::vector<std::shared_ptr<Resource>> _resources;
};