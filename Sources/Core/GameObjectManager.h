#pragma once

#include "GameObject.h"
#include "Light.h"

class GameObjectManager : public ISingleton<GameObjectManager>, public IManager<GameObject>
{
};
