#include "pch.h"
#include "ComponentManager.h"

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
	for (const auto& component : _components)
	{
		component->Destroy();
	}
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