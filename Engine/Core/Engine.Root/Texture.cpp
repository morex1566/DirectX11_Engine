#include "pch.h"
#include "Texture.h"

Texture::Texture()
	: Resource(),
	  _textureView(nullptr)
{
}

Texture::Texture(const std::string& texturePath_)
	: Resource()
{
}

Texture::~Texture()
{
	Texture::Destroy();
}

void Texture::Destroy()
{
	_isDestroyed = true;
	_isActivated = false;
}

ID3D11ShaderResourceView* Texture::GetTextureView()
{
	return _textureView.Get();
}