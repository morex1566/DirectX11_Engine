#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Light.h"

class GameObjectManager : public ISingleton<GameObjectManager>, public IManager<GameObject>
{
};
