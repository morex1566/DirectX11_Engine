// ReSharper disable CppClangTidyModernizeUseEqualsDefault

#include "pch.h"
#include "Material.h"
// TODO : needs to sort
#include "Mesh.h"
#include "Shader.h"
#include "UnlitShader.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "SceneManager.h"
#include "DirectXTex/DirectXTex.h"


Material::Material()
{
	_albedo = std::make_unique<DirectionalLight>(
		XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT3(1.0f, 0.0f, 1.0f)
	);
}

Material::~Material()
{
	Material::Destroy();
}

void Material::Render()
{
	for (const auto& mesh : _meshs)
	{
		mesh->Render();
	}

	for (const auto& shader : _shaders)
	{
		shader->Render(this, SceneManager::Get().GetCurrentScene()->GetViewportCamera(), this->GetOwner());
	}
}

void Material::Destroy()
{
	Component::Destroy();

	for (const auto& mesh : _meshs)
	{
		mesh->Destroy();
	}

	for (const auto& shader : _shaders)
	{
		shader->Destroy();
	}

	for (const auto& texture : _textures)
	{
		texture->Destroy();
	}
}

std::vector<Mesh*> Material::GetMeshs()
{
	return _meshs;
}

std::vector<Shader*> Material::GetShaders()
{
	return _shaders;
}

std::vector<Texture*> Material::GetTexture()
{
	return _textures;
}

DirectionalLight* Material::GetAlbedo()
{
	return _albedo.get();
}

void Material::SetMesh(const std::string& filePath_)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath_,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	if (scene == nullptr ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		scene->mRootNode == nullptr)
	{
		LogManager::LogWarning(std::string("Error loading file : ") + std::string(importer.GetErrorString()));
		return;
	}

	loadMesh(scene->mRootNode, scene);
}

void Material::SetShader(const std::string& vsPath_, const std::string& psPath_)
{
	Shader* shader = new UnlitShader;
	shader->ReadShader(vsPath_, psPath_);

	_shaders.push_back(shader);
}

void Material::SetTexture(const std::string& filePath_)
{
	Texture* texture = new Texture;
	texture->ReadFile(filePath_);

	_textures.push_back(texture);
}

void Material::loadMesh(const aiNode* node_, const aiScene* scene_)
{
	for (uint i = 0; i < node_->mNumMeshes; i++)
	{
		const aiMesh* fbxMesh = scene_->mMeshes[node_->mMeshes[i]];
		Mesh* mesh = new Mesh;
		_meshs.emplace_back(mesh);
		{
			_meshs.back()->ReadVertices(fbxMesh, scene_);
			_meshs.back()->ReadIndices(fbxMesh, scene_);
		}
	}

	for (uint i = 0; i < node_->mNumChildren; i++)
	{
		loadMesh(node_->mChildren[i], scene_);
	}
}