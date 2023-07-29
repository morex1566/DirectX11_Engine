// ReSharper disable CppClangTidyModernizeUseEqualsDefault

#include "pch.h"
#include "Material.h"
// TODO : needs to sort
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "D3DManager.h"

Material::Material()
{
	
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
		shader->Render();
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

void Material::SetShader(const std::string& vsFilePath_, const std::string& psFilePath_)
{
}

void Material::SetTexture(const std::string& filePath_)
{
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