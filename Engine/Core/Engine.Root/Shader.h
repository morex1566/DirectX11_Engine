#pragma once
#include <d3d11.h>

class Material;
class Camera;
class GameObject;

class Shader
{
public:
	Shader() = default;
	virtual ~Shader();

	virtual void Render(Material* material_, Camera* viewportCamera_, GameObject* owner_) = 0;
	virtual void Destroy() = 0;

	virtual void ReadShader(const std::string& vsPath_, const std::string& psPath_) = 0;

protected:
	ComPtr<ID3D11VertexShader>		_vertexShader;
	ComPtr<ID3D11PixelShader>		_pixelShader;
	ComPtr<ID3D11InputLayout>		_inputLayout;
	ComPtr<ID3D11SamplerState>		_sampleState;
	ComPtr<ID3D11Buffer>			_matrixBuffer;
};