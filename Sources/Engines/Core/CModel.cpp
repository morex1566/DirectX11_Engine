#include "PCH.h"
#include "CModel.h"

CModel::CModel(OGameObject* InOwner)
	: OComponent(InOwner)
{
}

CModel::~CModel()
{
	Shutdown();
}

void CModel::Init()
{
	OComponent::Init();
}

void CModel::Shutdown()
{
	OComponent::Shutdown();
}

void CModel::Start() 
{
	OComponent::Start();

	CreateVertexBuffer();
	CreateIndexBuffer();
}

void CModel::Tick()
{
	OComponent::Tick();

	// 랜더타겟뷰를 3D로 변경
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
	Render();
	Shader->Render(IndexCount, 0, 0);
}

void CModel::End()
{
	OComponent::End();
}


void CModel::LoadMesh(const std::wstring& InFilePath)
{
	Assimp::Importer Importer;

	const aiScene* Scene = Importer.ReadFile(ToString(InFilePath), 
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);
	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		SConsole::LogError(L"Cannot read file from Assimp::Importer.", __FILE__, __LINE__);
	}
	else
	// 파일 읽기 성공
	{
		Parse(Scene);
	}
}

void CModel::LoadTexture(const std::wstring& InFilePath, ETexture InTextureType)
{
	Texture = new CTexture(Owner);
	Texture->Load(InFilePath, InTextureType);

	Owner->TAttachComponent<CTexture>(Texture);
}

void CModel::LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath)
{
	Shader = new CLitShader(Owner);
	Shader->Load(InVSFilePath, InPSFilePath);

	Owner->TAttachComponent<CLitShader>(Shader);
}

void CModel::AddVertex(const FVertex& InVertex)
{
	Vertices.emplace_back(InVertex);
	VertexCount++;
}

void CModel::AddIndex(UINT InIndex)
{
	Indices.emplace_back(InIndex);
	IndexCount++;
}

void CModel::Render()
{
	UINT				 Stride;
	UINT				 Offset;

	Stride = sizeof(FVertex);
	Offset = 0;
	ID3D11DeviceContext& DeviceContext = SApplication::GetDirectX11()->GetDeviceContext();
	{
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		DeviceContext.IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		DeviceContext.IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		DeviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}

void CModel::Parse(const aiScene* InScene)
{
	// 파일의 데이터 
	int TotalVertexCount = 0;
	int TotalIndexCount = 0;
	int TotalBoneCount = 0;

	for (UINT i = 0; i < InScene->mNumMeshes; i++)
	{
		// 메쉬정보 추출
		const aiMesh* Mesh = InScene->mMeshes[i];
		{
			int VertexCountInMesh = Mesh->mNumVertices;
			int IndexCountInMesh = Mesh->mNumFaces * 3; // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			int BoneCountInMesh = Mesh->mNumBones;

			// 메쉬 정보 추출
			if (Mesh->mNumVertices > 0)
			{
				ParseMesh(Mesh, InScene);
			}

			TotalVertexCount += VertexCountInMesh;
			TotalIndexCount += IndexCountInMesh;
			TotalBoneCount += BoneCountInMesh;

			// 뼈 정보 추출
			if (Mesh->HasBones())
			{
				ParseBone(i, Mesh, InScene);
			}
		}
	}

	VertexCount = TotalVertexCount;
	IndexCount = TotalIndexCount;
	BoneCount = TotalBoneCount;
}

void CModel::ParseMesh(const aiMesh* InMesh, const aiScene* InScene)
{
	// 정점 추출
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

		AddVertex(FVertex(Position, XMFLOAT4(), TexCoord, Normal, XMFLOAT3(), XMFLOAT3()));
	}

	// 인덱스 설정
	for (UINT i = 0; i < InMesh->mNumFaces; i++)
	{
		aiFace Face = InMesh->mFaces[i];

		for (UINT j = 0; j < Face.mNumIndices; j++)
		{
			AddIndex(Face.mIndices[j]);
		}
	}
}

void CModel::ParseBone(UINT InMeshIndex, const aiMesh* InMesh, const aiScene* InScene)
{
	for (UINT i = 0; i < InMesh->mNumBones; i++)
	{
		aiBone* Bone = InMesh->mBones[i];

		// 뼈의 가중치를 Vertex에 전달합니다.
		for (UINT j = 0; j < Bone->mNumWeights; j++)
		{
			const aiVertexWeight& VertexWeight = Bone->mWeights[j];

			Vertices[VertexWeight.mVertexId].AddBone(i, VertexWeight.mWeight);
		}
	}
}

void CModel::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC			VertexBufferDesc;
	D3D11_SUBRESOURCE_DATA		VertexData;
	HRESULT						Result;

	// Initialize vertex buffer desc.
	{
		ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));

		VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(FVertex) * Vertices.size());
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = 0;
		VertexBufferDesc.MiscFlags = 0;
		VertexBufferDesc.StructureByteStride = 0;
	}

	// Initialize vertex data.
	{
		ZeroMemory(&VertexData, sizeof(VertexData));

		VertexData.pSysMem = Vertices.data();
		VertexData.SysMemPitch = 0;
		VertexData.SysMemSlicePitch = 0;
	}

	// Create vertex buffer.
	ID3D11Device& Device = SApplication::GetDirectX11()->GetDevice();
	{
		Result = Device.CreateBuffer(&VertexBufferDesc, &VertexData, &VertexBuffer);
		if (FAILED(Result))
		{
			SConsole::LogError(L"VertexCreateBuffer() is failed.", __FILE__, __LINE__);
		}
	}
}

void CModel::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC			IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA		IndexData;
	HRESULT						Result;

	// Initialize index buffer desc.
	{
		ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));

		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(UINT) * Indices.size());
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;
		IndexBufferDesc.MiscFlags = 0;
		IndexBufferDesc.StructureByteStride = 0;
	}

	// Initialize index data.
	{
		ZeroMemory(&IndexData, sizeof(IndexData));

		IndexData.pSysMem = Indices.data();
		IndexData.SysMemPitch = 0;
		IndexData.SysMemSlicePitch = 0;
	}

	// Create index buffer.
	ID3D11Device& Device = SApplication::GetDirectX11()->GetDevice();
	{
		Result = Device.CreateBuffer(&IndexBufferDesc, &IndexData, &IndexBuffer);
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreateBuffer() is failed.", __FILE__, __LINE__);
		}
	}
}

void CModel::CreateBoneTransforms(float InTimeinSeconds, const aiScene* InScene)
{
	XMMATRIX BoneTransform;

	float TicksPerSecond = InScene->mAnimations[0]->mTicksPerSecond != 0 ?
						   InScene->mAnimations[0]->mTicksPerSecond 
						   : 25.0f;

	float TimeInTicks = InTimeinSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, InScene->mAnimations[0]->mDuration);

}
