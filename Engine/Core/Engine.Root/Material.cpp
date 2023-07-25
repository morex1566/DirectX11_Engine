// ReSharper disable CppClangTidyModernizeUseEqualsDefault

#include "pch.h"
#include "Material.h"
// TODO : needs to sort
#include "Mesh.h"
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
	if(_mesh != nullptr)
	{
		_mesh->Render();
	}

	if(_shader != nullptr)
	{
		_shader->Render();
	}
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