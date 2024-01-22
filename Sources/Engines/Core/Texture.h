#pragma once

#include "Component.h"

enum class TextureType
{
	DDS,
	WIC,
	TGA
};

class Texture : public Component
{
public:
	Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	Texture(const Texture&)						= default;
	Texture& operator=(const Texture&)			= default;
	Texture(Texture&&) noexcept					= default;
	Texture& operator=(Texture&&) noexcept		= default;
	~Texture() override  						= default;


public:
	void Start() override;
	void Update() override;
	void Shutdown() override;
	void Load(const std::wstring& Filename, TextureType type);
	void Release();
	FORCEINLINE ID3D11ShaderResourceView* GetTexture() const { return resource; }


private:
	ID3D11Device*				device = nullptr;
	ID3D11DeviceContext*		deviceContext = nullptr;

	ID3D11ShaderResourceView*	resource = nullptr;


};

