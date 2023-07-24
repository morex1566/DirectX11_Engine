// ReSharper disable CppClangTidyModernizeUseEqualsDefault

#include "pch.h"
#include "Material.h"
// TODO : needs to sort
#include "Shader.h"
#include "Texture.h"

Material::Material()
	: _shader(nullptr), _texture(nullptr)
{
}

Material::~Material()
{
	Material::Destroy();
}

void Material::Render()
{
}

void Material::Destroy()
{
	Component::Destroy();

	if(_shader != nullptr)
	{
		_shader->Destroy();
	}

	if(_texture != nullptr)
	{
		_texture->Destroy();
	}
}