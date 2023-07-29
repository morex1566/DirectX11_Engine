#pragma once

#include "Component.h"

class Shader;
class Texture;
class Mesh;

class Material : public Component
{
public:
	Material();
	~Material() override;

	void Render();
	void Destroy() override;

	void SetMesh(const std::string& filePath_);
	void SetShader(const std::string& vsFilePath_, const std::string& psFilePath_);
	void SetTexture(const std::string& filePath_);

private:
	void loadMesh(const aiNode* node_, const aiScene* scene_);

private:
	std::vector<Mesh*>			_meshs;
	std::vector<Shader*>		_shaders;
	std::vector<Texture*>		_textures;
};