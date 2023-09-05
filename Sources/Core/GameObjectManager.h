#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Light.h"

class GameObjectManager : public IManager<GameObject>
{
public:
	GameObjectManager()				= default;
	~GameObjectManager() override	= default;

	void Initialize() override;
	void Update() override;
	void Dispose() override;
	void Clear() override;
};
