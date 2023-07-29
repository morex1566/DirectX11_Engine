#include "pch.h"
#include "ResourceManager.h"

ResourceManager& ResourceManager::Get()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::Initialize()
{
	
}

void ResourceManager::Shutdown()
{
	for (const auto& resource : _resources)
	{
		resource->Destroy();
	}
}

void ResourceManager::Dispose()
{
	for(auto it = _resources.begin(); it != _resources.end();)
	{
		if(it->get()->IsDestroyed())
		{
			it->reset();
			it = _resources.erase(it);
		}
		else
		{
			++it;
		}
	}
}

ResourceManager::ResourceManager()
	: IManager(),
	  ISingleton(),
	  IObjectPool()
{
}
