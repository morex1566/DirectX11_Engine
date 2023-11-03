#pragma once
#include "Object.h"
#include <d3dcompiler.h>

class ODirectX11;
class OWindow;

class OShader : public Object
{
public:
	OShader(ODirectX11& InDirectX11, OWindow& InWindow, const std::string& InVertexShaderFilePath, const std::string& InPixelShaderFilePath);
	OShader(const OShader&)									= default;
	OShader& operator=(const OShader&)						= default;
	OShader(OShader&&) noexcept								= default;
	OShader& operator=(OShader&&) noexcept					= default;
	~OShader() override;

	EHandleResultType										Initialize() override;
	void													Release() override;
	EHandleResultType										SetShaderParameters(XMMATRIX InWorld, XMMATRIX InView, XMMATRIX InProjection);
	void													Process(UINT InIndexCount);

protected:

private:
	ODirectX11*												DirectX11;
	OWindow*												Window;
	std::string												VertexShaderFilePath;
	std::string												PixelShaderFilePath;

	ID3D11VertexShader*										m_vertexShader;
	ID3D11PixelShader*										m_pixelShader;
	ID3D11InputLayout*										m_layout;
	ID3D11Buffer*											m_matrixBuffer;
};

