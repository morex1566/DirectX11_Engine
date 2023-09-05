#include "pch.h"
#include "GameObjectManager.h"

void GameObjectManager::Initialize()
{
	IManager<GameObject>::Initialize();
}

void GameObjectManager::Update()
{
	IManager<GameObject>::Update();
}

void GameObjectManager::Dispose()
{
	IManager<GameObject>::Dispose();
}

void GameObjectManager::Clear()
{
	IManager<GameObject>::Clear();
}
