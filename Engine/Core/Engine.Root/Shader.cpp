#include "pch.h"
#include "Shader.h"

Shader::Shader(const std::string& vsPath_, const std::string& psPath_)
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
}