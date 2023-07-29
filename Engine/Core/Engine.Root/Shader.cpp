#include "pch.h"
#include "Shader.h"

Shader::Shader(const std::string& vsPath_, const std::string& psPath_)
	:	_vertexShader(nullptr),
		_pixelShader(nullptr),
		_layout(nullptr),
		_sampleState(nullptr),
		_matrixBuffer(nullptr)
{
}

Shader::~Shader()
{
	Destroy();
}

void Shader::Render()
{
}

void Shader::Destroy()
{
	_vertexShader->Release();
	_vertexShader = nullptr;

	_pixelShader->Release();
	_pixelShader = nullptr;

	_layout->Release();
	_layout = nullptr;

	_sampleState->Release();
	_sampleState = nullptr;

	_matrixBuffer->Release();
	_matrixBuffer = nullptr;
}

void Shader::Load(const std::string& vsFilePath_, const std::string& psFilePath_)
{
}
