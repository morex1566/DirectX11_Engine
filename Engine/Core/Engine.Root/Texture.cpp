#include "pch.h"

#include "D3DManager.h"
#include "DirectXTex/DirectXTex.h"
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

void Texture::ReadFile(const std::string& filePath_)
{
	HRESULT						result;
	DirectX::ScratchImage		image;
	DirectX::TexMetadata		metaData;

	result = LoadFromWICFile(GetWString(filePath_).c_str(), DirectX::WIC_FLAGS_NONE, &metaData, image);
	if (FAILED(result))
	{
		LogManager::LogWarning("Load file failure.");
		return;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	{
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = metaData.width;
		textureDesc.Height = metaData.height;
		textureDesc.MipLevels = metaData.mipLevels;
		textureDesc.ArraySize = 1;
		textureDesc.Format = metaData.format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
	}

	D3D11_SUBRESOURCE_DATA textureData;
	{
		ZeroMemory(&textureData, sizeof(textureData));
		textureData.pSysMem = image.GetPixels();
		textureData.SysMemPitch = static_cast<UINT>(metaData.width * DirectX::BitsPerPixel(metaData.format) / 8);
		textureData.SysMemSlicePitch = static_cast<UINT>(image.GetPixelsSize());
	}

	result = D3DManager::Get().GetDevice()->CreateTexture2D(&textureDesc, &textureData, _texture.GetAddressOf());
	if(FAILED(result))
	{
		LogManager::LogWarning("Create texture failure.");
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	{
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	}

	result = D3DManager::Get().GetDevice()->CreateShaderResourceView(_texture.Get(), &shaderResourceViewDesc, _textureView.GetAddressOf());
	if (FAILED(result))
	{
		LogManager::LogWarning("Create shader resource view failure.");
		return;
	}
}

ID3D11ShaderResourceView* Texture::GetTextureView()
{
	return _textureView.Get();
}

ID3D11Texture2D* Texture::GetTexture()
{
	return _texture.Get();
}
