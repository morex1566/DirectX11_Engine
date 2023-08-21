#include "pch.h"
#include "Texture.h"
#include "DirectXTex/WICTextureLoader11.h"
#include "DirectXTex/DDSTextureLoader11.h"

Texture::Texture(): _device(nullptr), _deviceContext(nullptr), _texture(nullptr), _resource(nullptr),
                    _shaderResourceView(nullptr),
                    _width(0), _height(0)
{
}

void Texture::Initialize(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, const std::string& filePath_)
{
	ClearMemory();

	bool result = false;
	_device = device_;
	_deviceContext = deviceContext_;

	result = loadWICTexture(filePath_);
	if (result)
	{
		return;
	}

	result = loadDDSTexture(filePath_);
	if (result)
	{
		return;
	}

	// TODO : Handle Exception is needed.
	return;
}

void Texture::ClearMemory()
{
	_device = nullptr;
	_deviceContext = nullptr;

	if (_texture != nullptr)
	{
		_texture->Release();
		_texture =	nullptr;
	}

	if (_resource != nullptr)
	{
		_resource->Release();
		_resource = nullptr;
	}

	if (_shaderResourceView != nullptr)
	{
		_shaderResourceView->Release();
		_shaderResourceView = nullptr;
	}

	_width = 0;
	_height = 0;
}

void Texture::ClearTexture()
{
	if (_texture != nullptr)
	{
		_texture->Release();
		_texture = nullptr;
	}

	if (_resource != nullptr)
	{
		_resource->Release();
		_resource = nullptr;
	}

	if (_shaderResourceView != nullptr)
	{
		_shaderResourceView->Release();
		_shaderResourceView = nullptr;
	}

	_width = 0;
	_height = 0;
}

void Texture::Destroy()
{
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView()
{
	return _shaderResourceView;
}

bool Texture::loadWICTexture(const std::string& filePath_)
{
	HRESULT result;

	result = CreateWICTextureFromFile(_device, _deviceContext, Utils::ToWString(filePath_).c_str(), &_resource, &_shaderResourceView);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Texture::loadDDSTexture(const std::string& filePath_)
{
	HRESULT result;

	result = CreateDDSTextureFromFile(_device, _deviceContext, Utils::ToWString(filePath_).c_str(), &_resource, &_shaderResourceView);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
