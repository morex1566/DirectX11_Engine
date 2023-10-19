#pragma once
class Shader : public Object
{
public:
	Shader()									= default;
	Shader(const Shader&)						= default;
	Shader& operator=(const Shader&)			= default;
	Shader(Shader&&) noexcept					= default;
	Shader& operator=(Shader&&) noexcept		= default;
	~Shader();

	bool Initialize(ID3D11Device* device_, HWND hWnd_, const std::wstring& shaderFilePath_);
	void ZeroInstanceMemory();
	void Render();

private:
	void OutputShaderErrorMessage(ID3D10Blob* errorMsg_, HWND hWnd_, const std::wstring& wstring_);

private:
	ComPtr<ID3D11VertexShader>				_vertexShader;
	ComPtr<ID3D11PixelShader>				_pixelShader;
	ComPtr<ID3D11InputLayout>				_inputLayout;
	ComPtr<ID3D11Buffer>					_matrixBuffer;
};

