#include "pch.h"
#include "ComponentManager.h"

#include "Component.h"

ComponentManager& ComponentManager::Get()
{
	static ComponentManager instance;
	return instance;
}

void ComponentManager::Initialize()
{
}

void ComponentManager::Shutdown()
{
}

void ComponentManager::Dispose()
{
	for (auto it = _components.begin(); it != _components.end();)
	{
		if (it->get()->IsDestroyed())
		{
			it->reset();
			it = _components.erase(it);
		}
		else
		{
			++it;
		}
	}
}