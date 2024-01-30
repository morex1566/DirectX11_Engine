#include "PCH.h"
#include "CModel.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CLitShader.h"

CModel::CModel(OGameObject* InOwner)
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

void CModel::Tick()
{
	auto* DirectX11 = SApplication::GetDirectX11();
	{
		DirectX11->SetDepthStencilState(ODirectX11::ERenderMode::R_3D);
		DirectX11->SetRasterizerState(ODirectX11::ERenderMode::R_3D);
		DirectX11->SetRenderTargets(ODirectX11::ERenderMode::R_3D);
		DirectX11->SetViewport(ODirectX11::ERenderMode::R_3D);
	}

	// 셰이더에 파라미터 전달
	auto* Camera = SApplication::GetCamera();
	auto* Transform = Owner->GetTransform();
	{
		Shader->SetShaderParameters(Transform->GetWorldMatrix(), Camera->GetViewMatrix(),
									Camera->GetProjectionMatrix(), Texture->GetResource());
	}

	// 그리기를 시작할 순서(인덱스) 설정
	for (auto*& Mesh : Meshes)
	{
		Mesh->Render();
		Shader->Render(Mesh->GetIndexCount(), 0, 0);
	}
}

void CModel::End() {}

void CModel::LoadModel(const std::wstring& InFilePath)
{
	ReleaseMesh();

	Assimp::Importer Importer;

	const aiScene* Scene = Importer.ReadFile(ToString(InFilePath), aiProcess_Triangulate);
	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		SConsole::LogError(L"Cannot read file from Assimp::Importer.", __FILE__, __LINE__);
	}
	else
	// 파일 읽기 성공
	{
		ProcessNode(Scene->mRootNode, Scene);
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
	for (auto& Mesh : Meshes)
	{
		Mesh->Shutdown();
		delete Mesh;
		Mesh = nullptr;
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

void CModel::ProcessNode(const aiNode* InNode, const aiScene* InScene)
{
	for (UINT i = 0; i < InNode->mNumMeshes; i++)
	{
		aiMesh* Mesh = InScene->mMeshes[InNode->mMeshes[i]];
		Meshes.emplace_back(ProcessMesh(Mesh, InNode));
	}

	for (UINT i = 0; i < InNode->mNumChildren; i++)
	{
		ProcessNode(InNode->mChildren[i], InScene);
	}
}

CMesh* CModel::ProcessMesh(const aiMesh* InMesh, const aiNode* InNode)
{
	std::vector<FVertex>		Vertices;
	std::vector<UINT>			Indices;


	// 정점 추출
	CMesh* Mesh = new CMesh;
	for (UINT i = 0; i < InMesh->mNumVertices; i++)
	{
		// 위치
		XMFLOAT3 Position = XMFLOAT3(
			InMesh->mVertices[i].x,
			InMesh->mVertices[i].y,
			InMesh->mVertices[i].z
		);

		// 텍스처 좌표
		// Index '0' means that the main texture of model.
		// If the model has another textures, ex. normal textures..., change the index.
		XMFLOAT2 TexCoord = XMFLOAT2(
			InMesh->mTextureCoords[0][i].x,
			InMesh->mTextureCoords[0][i].y	
		);

		// 노멀
		XMFLOAT3 Normal = XMFLOAT3(
			InMesh->mNormals[i].x,
			InMesh->mNormals[i].y,
			InMesh->mNormals[i].z
		);

		Mesh->AddVertex(FVertex(Position, XMFLOAT4(), TexCoord, Normal));
	}

	// 인덱스 설정
	for (UINT i = 0; i < InMesh->mNumFaces; i++)
	{
		aiFace Face = InMesh->mFaces[i];

		for (UINT j = 0; j < Face.mNumIndices; j++)
		{
			Mesh->AddIndex(Face.mIndices[j]);
		}
	}

	// 정점 로드X
	if (Mesh->GetVertexCount() <= 0)
	{
		SConsole::LogError(L"Vertice is not loaded.", __FILE__, __LINE__);
	}

	return Mesh;
}
