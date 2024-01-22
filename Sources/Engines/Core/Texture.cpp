#include "PCH.h"
#include "Texture.h"
#include "DirectXTex/DirectXTex.h"

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: device(device), deviceContext(deviceContext)
{
}

void Texture::Start()
{
	Component::Start();
}

void Texture::Update()
{
	Component::Update();
}

void Texture::Shutdown()
{
	Component::Shutdown();

	Release();
}

void Texture::Load(const std::wstring& Filename, TextureType type)
{
	Release();

	HRESULT result;
	DirectX::ScratchImage image;

	switch (type)
	{
		case TextureType::DDS:
		{
			result = LoadFromDDSFile(Filename.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
			if (SUCCEEDED(result))
			{
				result = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &resource);
				if (FAILED(result))
				{
					Console::LogWarning(L"CreateShaderResourceView() is failed.", __FILE__, __LINE__);
					return;
				}
			}
			else
			{
				Console::LogWarning(L"LoadFromDDSFile() is failed.", __FILE__, __LINE__);
				return;
			}

			break;
		}

		case TextureType::WIC:
		{
			result = LoadFromWICFile(Filename.c_str(), DirectX::WIC_FLAGS_DEFAULT_SRGB, nullptr, image);
			if (SUCCEEDED(result))
			{
				result = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &resource);
				if (FAILED(result))
				{
					Console::LogWarning(L"CreateShaderResourceView() is failed.", __FILE__, __LINE__);
					return;
				}
			}
			else
			{
				Console::LogWarning(L"LoadFromDDSFile() is failed.", __FILE__, __LINE__);
				return;
			}

			break;
		}

		case TextureType::TGA:
		{
			result = LoadFromTGAFile(Filename.c_str(), nullptr, image);
			if (SUCCEEDED(result))
			{
				result = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &resource);
				if (FAILED(result))
				{
					Console::LogWarning(L"CreateShaderResourceView() is failed.", __FILE__, __LINE__);
					return;
				}
			}
			else
			{
				Console::LogWarning(L"LoadFromDDSFile() is failed.", __FILE__, __LINE__);
				return;
			}

			break;
		}
	}
}

void Texture::Release()
{
	if (resource)
	{
		resource->Release();
		resource = nullptr;
	}
}
