#pragma once

#include "Component.h"

class Shader;
class Texture;
class Mesh;
class DirectionalLight;

class Material : public Component
{
public:
	Material();
	~Material() override;

	void Render();
	void Destroy() override;

	std::vector<Mesh*>			GetMeshs();
	std::vector<Shader*>		GetShaders();
	std::vector<Texture*>		GetTexture();
	DirectionalLight*			GetAlbedo();

	void SetMesh(const std::string& filePath_);
	void SetShader(const std::string& vsPath_, const std::string& psPath_);
	void SetTexture(const std::string& filePath_);

private:
	void loadMesh(const aiNode* node_, const aiScene* scene_);

private:
	std::vector<Mesh*>						_meshs;
	std::vector<Shader*>					_shaders;
	std::vector<Texture*>					_textures;
	std::unique_ptr<DirectionalLight>		_albedo;
};