#pragma once

class Texture
{
public:
	Texture();
	Texture(const Texture&)			= default;
	~Texture()						= default;

	void Initialize(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, const std::string& filePath_);
	void ClearMemory();
	void ClearTexture();
	void Destroy();

	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	bool loadWICTexture(const std::string& filePath_);
	bool loadDDSTexture(const std::string& filePath_);

private:
	ID3D11Device*				_device;
	ID3D11DeviceContext*		_deviceContext;
	ID3D11Texture2D*			_texture;
	ID3D11Resource*				_resource;
	ID3D11ShaderResourceView*	_shaderResourceView;
	int							_width;
	int							_height;
};

