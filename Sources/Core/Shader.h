#pragma once
#include "Component.h"

class Shader : public Component, public Management
{
public:
	Shader(const GameObject* owner_);
	Shader(const Shader&)						= delete;
	Shader& operator=(const Shader&)			= delete;
	Shader(Shader&&) noexcept					= delete;
	Shader& operator=(Shader&&) noexcept		= delete;
	~Shader() override;

	bool Initialize(ID3D11Device* device_, HWND hWnd_, const std::wstring& shaderFilePath_);
	void Render();

private:
	void outputShaderErrorMessage(ID3D10Blob* errorMsg_, HWND hWnd_, const std::wstring& wstring_);

private:
	ComPtr<ID3D11VertexShader>				_vertexShader;
	ComPtr<ID3D11PixelShader>				_pixelShader;
	ComPtr<ID3D11InputLayout>				_inputLayout;
	ComPtr<ID3D11Buffer>					_matrixBuffer;
};

