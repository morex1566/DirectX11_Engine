#pragma once

#include <d3dcompiler.h>

#include "Component.h"

class Shader : public Component
{
public:
	Shader(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	Shader(const Shader&)						= default;
	Shader& operator=(const Shader&)			= default;
	Shader(Shader&&) noexcept					= default;
	Shader& operator=(Shader&&) noexcept		= default;
	~Shader() override  						= default;


public:
	void Start() override;
	void Update() override;
	void Shutdown() override;
	void Load(const std::wstring& vsFilePath, const std::wstring& psFilePath);
	void Render();
	void Release();
	void SetShaderParams();


private:
	void loadVertexShader(const std::wstring& filePath);
	void loadPixelShader(const std::wstring& filePath);
	std::wstring readShaderErrorMsg(ID3D10Blob* errorMessage);


private:
	// CAUTION : device를 수정하지 마세요!!!
	ID3D11Device* device = nullptr;
	// CAUTION : deviceContext를 수정하지 마세요!!!
	ID3D11DeviceContext* deviceContext = nullptr;

	ID3D11VertexShader*			vertexShader;
	ID3D11PixelShader*			pixelShader;
	ID3D11InputLayout*			layout;
	ID3D11SamplerState*			sampleState;
	ID3D11Buffer*				matrixBuffer;
	ID3D11Buffer*				lightBuffer;


};

