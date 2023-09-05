#pragma once

#include "Component.h"
#include "Model.h"

class ComponentManager : public IManager<Component>
{
public:
	ComponentManager()					= default;
	~ComponentManager() override		= default;

	void Initialize() override;
	void Update() override;
	void Dispose() override;
	void Clear() override;
};