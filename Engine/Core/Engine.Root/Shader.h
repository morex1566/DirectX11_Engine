#pragma once
#include <d3d11.h>

class Shader
{
public:
	Shader(const std::string& vsPath_, const std::string& psPath_);
	virtual ~Shader();

	virtual void Render();
	virtual void Destroy();
	void Load(const std::string& vsFilePath_, const std::string& psFilePath_);

private:
	ID3D11VertexShader*				_vertexShader;
	ID3D11PixelShader*				_pixelShader;
	ID3D11InputLayout*				_layout;
	ID3D11SamplerState*				_sampleState;
	ID3D11Buffer*					_matrixBuffer;
};