#pragma once
#include <d3d11.h>

class Texture
{
public:
	Texture(const std::string& texturePath_);
	~Texture();

	void Destroy();
	ID3D11ShaderResourceView* GetResource();

private:
	ID3D11ShaderResourceView* _resource;	
};