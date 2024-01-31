#include "PCH.h"
#include "CTexture.h"
#include "DirectXTex/DirectXTex.h"
#include "SApplication.h"
#include "SConsole.h"

CTexture::CTexture(OGameObject* InOwner)
	: OComponent(InOwner)
{
}

CTexture::~CTexture()
{
	Shutdown();
}

void CTexture::Init()
{
	OComponent::Init();
}

void CTexture::Shutdown()
{
	OComponent::Shutdown();

	ReleaseResource();
}

void CTexture::Start()
{
	OComponent::Start();
}

void CTexture::Tick()
{
	OComponent::Tick();
}

void CTexture::End()
{
	OComponent::End();
}

void CTexture::ReleaseResource()
{
	if (Resource)
	{
		Resource->Release();
		Resource = nullptr;
	}
}

void CTexture::Load(const std::wstring& InFilePath, ETexture InType)
{
	ReleaseResource();

	HRESULT result;
	DirectX::ScratchImage image;

	auto DirectX11 = SApplication::GetDirectX11();

	switch (InType)
	{
		case ETexture::DDS:
		{
			result = LoadFromDDSFile(InFilePath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
			if (SUCCEEDED(result))
			{
				result = DirectX::CreateShaderResourceView(&DirectX11->GetDevice(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &Resource);
				if (FAILED(result))
				{
					SConsole::LogError(L"CreateShaderResourceView() is failed.", __FILE__, __LINE__);
				}
			}
			else
			{
				SConsole::LogError(L"LoadFromDDSFile() is failed.", __FILE__, __LINE__);
			}

			break;
		}

		case ETexture::TGA:
		{
			result = LoadFromTGAFile(InFilePath.c_str(), nullptr, image);
			if (SUCCEEDED(result))
			{
				result = DirectX::CreateShaderResourceView(&DirectX11->GetDevice(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &Resource);
				if (FAILED(result))
				{
					SConsole::LogError(L"CreateShaderResourceView() is failed.", __FILE__, __LINE__);
				}
			}
			else
			{
				SConsole::LogError(L"LoadFromTGAFile() is failed.", __FILE__, __LINE__);
			}

			break;
		}

		case ETexture::WIC:
		{
			result = LoadFromWICFile(InFilePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
			if (SUCCEEDED(result))
			{
				result = DirectX::CreateShaderResourceView(&DirectX11->GetDevice(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &Resource);
				if (FAILED(result))
				{
					SConsole::LogError(L"CreateShaderResourceView() is failed.", __FILE__, __LINE__);
				}
			}
			else
			{
				SConsole::LogError(L"LoadFromWICFile() is failed.", __FILE__, __LINE__);
			}
		}
	}
}
