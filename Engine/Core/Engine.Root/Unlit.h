#pragma once
#include "Shader.h"

class Unlit : public Shader
{
public:
	Unlit(const std::string& vsPath_, const std::string& psPath_);
	~Unlit() override;

	void Destroy() override;

private:
	ID3D11Buffer* m_lightBuffer;
};