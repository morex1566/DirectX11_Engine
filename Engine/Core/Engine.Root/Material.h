#pragma once

#include "Component.h"

class Shader;
class Texture;

class Material : public Component
{
public:
	Material();
	~Material() override;

	void Render();
	void Destroy() override;

private:
	std::unique_ptr<Shader>			_shader;
	std::unique_ptr<Texture>		_texture;
};