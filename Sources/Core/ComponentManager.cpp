#include "pch.h"
#include "ComponentManager.h"

void ComponentManager::Initialize()
{
	IManager<Component>::Initialize();
}

void ComponentManager::Update()
{
	IManager<Component>::Update();
}

void ComponentManager::Dispose()
{
	IManager<Component>::Dispose();
}

void ComponentManager::Clear()
{
	IManager<Component>::Clear();
}
