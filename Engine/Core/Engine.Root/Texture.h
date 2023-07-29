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

	ID3D11ShaderResourceView*	GetTextureView();

private:
	ComPtr<ID3D11ShaderResourceView>	_textureView;

};