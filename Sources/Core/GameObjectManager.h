#pragma once

#include "GameObject.h"

class GameObjectManager : public ISingleton<GameObjectManager>, public IManager<GameObject>
{
};
