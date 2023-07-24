#include "pch.h"
#include "Texture.h"

Texture::Texture(const std::string& texturePath_)
{
}

Texture::~Texture()
{
}

void Texture::Destroy()
{
	_resource->Release();
	_resource = nullptr;
}

ID3D11ShaderResourceView* Texture::GetResource()
{
	return _resource;
}
