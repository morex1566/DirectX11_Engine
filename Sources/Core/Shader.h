#pragma once

class Shader
{
public:
	Shader()					= default;
	Shader(const Shader&)		= default;
	~Shader()					= default;

	bool Initialize(ID3D11Device* device_, HWND hWnd_);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext_, int indexCount_, XMMATRIX worldMatrix_, XMMATRIX viewMatrix_, XMMATRIX projectionMatrix_,
		ID3D11ShaderResourceView* texture_, XMFLOAT3 lightDirection_, XMFLOAT4 diffuseColor_);

private:
	bool InitializeShader(ID3D11Device* device_, HWND hWnd_, const std::wstring& vsFilePath_, const std::wstring& psFilePath_);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage_, HWND hWnd_, const std::wstring& shaderFilename_);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext_, XMMATRIX worldMatrix_, XMMATRIX viewMatrix_, XMMATRIX projectionMatrix_,
		ID3D11ShaderResourceView* texture_, XMFLOAT3 lightDirection_, XMFLOAT4 diffuseColor_);
	void RenderShader(ID3D11DeviceContext* deviceContext_, int indexCount_);

private:
	ID3D11VertexShader*					_vertexShader;
	ID3D11PixelShader*					_pixelShader;
	ID3D11InputLayout*					_layout;
	ID3D11SamplerState*					_sampleState;
	ID3D11Buffer*						_matrixBuffer;
	ID3D11Buffer*						_lightBuffer;
};

