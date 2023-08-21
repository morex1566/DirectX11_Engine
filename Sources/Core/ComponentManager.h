#pragma once

#include "Component.h"
#include "Model.h"

class ComponentManager : public ISingleton<ComponentManager>, public IManager<Component>
{
};