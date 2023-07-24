#pragma once

#include "Component.h"

class Mesh : public Component
{
public:
	Mesh();
	~Mesh() override;

	void Render();

private:

};