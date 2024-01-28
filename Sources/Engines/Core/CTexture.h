#pragma once
#include "OComponent.h"

enum class ETexture
{
	DDS,
	WIC,
	TGA
};

class CTexture : public OComponent
{
public:
	CTexture(const OGameObject* InOwner);
	CTexture(const CTexture&)						= default;
	CTexture& operator=(const CTexture&)			= default;
	CTexture(CTexture&&) noexcept					= default;
	CTexture& operator=(CTexture&&) noexcept		= default;
	~CTexture() override;


public:
	void			Init() override;
	void			Shutdown() override;
	void			Start() override;
	void			Tick() override;
	void			End() override;


public:
	void Load(const std::wstring& InFilePath, ETexture InType);
	FORCEINLINE ID3D11ShaderResourceView* GetResource() 
	{ 
		return Resource; 
	}
	FORCEINLINE void SetResource(ID3D11ShaderResourceView* InResource) 
	{ 
		Shutdown();
		Resource = InResource;  
	}


private:
	ID3D11ShaderResourceView* Resource = nullptr;


};

