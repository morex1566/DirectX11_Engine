#pragma once
#include "Shader.h"

class DirectionalLight;

class UnlitShader : public Shader
{
public:
	UnlitShader();
	~UnlitShader() override;

	void Render(Material* material_, Camera* viewportCamera_, GameObject* owner_) override;
	void Destroy() override;

	void ReadShader(const std::string& vsPath_, const std::string& psPath_) override;

protected:
	void sendParametersToShader(Material* material_, Camera* viewportCamera_, GameObject* owner_, DirectionalLight* light_);

private:
	ComPtr<ID3D11Buffer>			_lightBuffer;
	ComPtr<ID3D11Buffer>			_cameraBuffer;
};