#pragma once

#include <d3d11.h>
#include "Resource.h"

class Texture : public Resource
{
public:
	Texture();
	Texture(const std::string& texturePath_);
	~Texture() override;

	void Destroy() override;
	void ReadFile(const std::string& filePath_);

	ID3D11ShaderResourceView*	GetTextureView();
	ID3D11Texture2D*			GetTexture();
private:
	ComPtr<ID3D11ShaderResourceView>	_textureView;
	ComPtr<ID3D11Texture2D>				_texture;
};