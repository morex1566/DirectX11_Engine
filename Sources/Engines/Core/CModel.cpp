#include "PCH.h"
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "CModel.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CLitShader.h"

CModel::CModel(const OGameObject* InOwner)
	: OComponent(InOwner)
{
}

CModel::~CModel()
{
	Shutdown();
}

void CModel::Init() {}

void CModel::Shutdown()
{
	ReleaseMesh();
	ReleaseTexture();
	ReleaseShader();
}

void CModel::Start() {}

void CModel::Tick() {}

void CModel::End() {}

void CModel::LoadModel(const std::wstring& InFilePath)
{
	ReleaseMesh();

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(ToString(InFilePath), aiProcess_Triangulate);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		SConsole::LogError("ERROR::ASSIMP::", __FILE__, __LINE__);
	}
}

void CModel::LoadTexture(const std::wstring& InFilePath)
{
	ReleaseTexture();

	Texture = new CTexture(Owner);
	Texture->Load(InFilePath, ETexture::TGA);
}

void CModel::LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath)
{
	ReleaseShader();

	Shader = new CLitShader(Owner);
	Shader->Load(InVSFilePath, InPSFilePath);
}

void CModel::ReleaseMesh()
{
	for (CMesh* Mesh : Meshes)
	{
		delete Mesh;
	}
	Meshes.clear();
}

void CModel::ReleaseTexture()
{
	if (Texture)
	{
		delete Texture;
		Texture = nullptr;
	}
}

void CModel::ReleaseShader()
{
	if (Shader)
	{
		delete Shader;
		Shader = nullptr;
	}
}
