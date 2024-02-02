#pragma warning (disable : 4244)

#include "PCH.h"
#include "CModel.h"

XMMATRIX ToXMMATRIX(aiMatrix4x4 InMatrix)
{
	XMMATRIX Converted = XMMATRIX(
		InMatrix.a1, InMatrix.a2, InMatrix.a3, InMatrix.a4,
		InMatrix.b1, InMatrix.b2, InMatrix.b3, InMatrix.b4,
		InMatrix.c1, InMatrix.c2, InMatrix.c3, InMatrix.c4,
		InMatrix.d1, InMatrix.d2, InMatrix.d3, InMatrix.d4);

	return Converted;
}

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

	if (BoneTransforms != nullptr)
	{
		delete[] BoneTransforms;
		BoneTransforms = nullptr;
	}
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

	Time += OTime::GetDeltaTime();

	// 뼈 행렬 업데이트
	UpdateBoneTransforms(Time);

	SConsole::Log(L"current time : " + std::to_wstring(Time));

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
									Camera->GetProjectionMatrix(), BoneTransforms, Texture->GetResource());
	}

	// 그리기를 시작할 순서(인덱스) 설정
	Render();
	Shader->Render(IndexCount, 0, 0);
}

void CModel::End()
{
	OComponent::End();
}


void CModel::Load(const std::wstring& InFilePath)
{
	Scene = Importer.ReadFile(ToString(InFilePath),
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);
	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		SConsole::LogError(L"Cannot read file from Assimp::Importer.", __FILE__, __LINE__);
	}
	else
	// 파일 읽기 성공
	{
		Parse();
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

void CModel::Parse()
{
	// 파일의 데이터 
	int TotalVertexCount = 0;
	int TotalIndexCount = 0;
	int TotalBoneCount = 0;

	// --Count 추출
	{
		for (UINT i = 0; i < Scene->mNumMeshes; i++)
		{
			// 메쉬정보 추출
			const aiMesh* Mesh = Scene->mMeshes[i];
			{
				TotalVertexCount += Mesh->mNumVertices;
				TotalIndexCount += Mesh->mNumFaces * 3; // D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
				TotalBoneCount += Mesh->mNumBones;
			}
		}

		VertexCount = TotalVertexCount;
		IndexCount = TotalIndexCount;
		BoneCount = TotalBoneCount;
	}

	BoneTransforms = new FBoneTransform[BoneCount];

	// 데이터 추출
	for (UINT i = 0; i < Scene->mNumMeshes; i++)
	{
		// 메쉬정보 추출
		const aiMesh* Mesh = Scene->mMeshes[i];
		{
			// 메쉬 정보 추출
			if (Mesh->mNumVertices > 0)
			{
				ParseMesh(Mesh);
			}

			// 뼈 정보 추출
			if (Mesh->HasBones())
			{
				ParseBone(i, Mesh);
			}
		}
	}
}

void CModel::ParseMesh(const aiMesh* InMesh)
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

void CModel::ParseBone(UINT InMeshIndex, const aiMesh* InMesh)
{
	for (UINT i = 0; i < InMesh->mNumBones; i++)
	{
		aiBone* Bone = InMesh->mBones[i];
		int     BoneID = GetBoneID(Bone->mName.C_Str());

		// 뼈의 기본행렬정보를 저장
		BoneTransforms[BoneID].Offset = ToXMMATRIX(Bone->mOffsetMatrix);
		BoneTransforms[BoneID].Transform = XMMatrixIdentity();

		// 뼈의 가중치를 FVertex에 저장
		for (UINT j = 0; j < Bone->mNumWeights; j++)
		{
			const aiVertexWeight& VertexWeight = Bone->mWeights[j];

			Vertices[VertexWeight.mVertexId].AddBone(GetBoneID(Bone->mName.C_Str()), VertexWeight.mWeight);
		}
	}
}

void CModel::UpdateBoneTransforms(float InTimeinSeconds)
{
	// TODO : 업데이트할 애니메이션 Walk.
	aiAnimation* WalkAnimation = Scene->mAnimations[0];

	// 초단위를 애니메이션의 틱으로 변환합니다.
	float TicksPerSecond = WalkAnimation->mTicksPerSecond != 0 ?
						   WalkAnimation->mTicksPerSecond 
						   : 25.0f;

	float TimeInTicks = InTimeinSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, WalkAnimation->mDuration);

	// 본 행렬을 구합니다.
	XMMATRIX Identity = XMMatrixIdentity();
	ReadNodeHierarchy(AnimationTime, Scene->mRootNode, Identity);
}

void CModel::ReadNodeHierarchy(float AnimationTime, const aiNode* InNode, const XMMATRIX& InParentTransform)
{
	std::string NodeName(InNode->mName.C_Str());

	// TODO : 업데이트할 애니메이션 Walk.
	aiAnimation* WalkAnimation = Scene->mAnimations[0];

	XMMATRIX NodeTransformation = ToXMMATRIX(InNode->mTransformation);

	const aiNodeAnim* NodeAnim = FindNodeAnimation(WalkAnimation, NodeName);

	if (NodeAnim) 
	{
		// Interpolate scaling
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, NodeAnim);
		XMMATRIX ScalingM = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, NodeAnim);
		XMMATRIX RotationM = XMMatrixRotationQuaternion(XMVectorSet(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z));

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation; 
		CalcInterpolatedPosition(Translation, AnimationTime, NodeAnim);
		XMMATRIX TranslationM = XMMatrixTranslation(Translation.x, Translation.y, Translation.z);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	XMMATRIX GlobalTransformation = InParentTransform * NodeTransformation;

	// 변형 매트릭스 등록
	if (BoneIDs.find(NodeName) != BoneIDs.end())
	{
		int BoneID = GetBoneID(NodeName);
		BoneTransforms->Transform = ToXMMATRIX(Scene->mRootNode->mTransformation.Inverse()) 
									* GlobalTransformation
									* BoneTransforms[BoneID].Transform;
	}

	// 자식 노드 순회
	for (UINT i = 0; i < InNode->mNumChildren; i++) 
	{
		ReadNodeHierarchy(AnimationTime, InNode->mChildren[i], GlobalTransformation);
	}
}

const aiNodeAnim* CModel::FindNodeAnimation(const aiAnimation* Animation, const std::string& NodeName)
{
	for (UINT i = 0; i < Animation->mNumChannels; i++) {
		const aiNodeAnim* NodeAnimation = Animation->mChannels[i];

		if (std::string(NodeAnimation->mNodeName.data) == NodeName) {
			return NodeAnimation;
		}
	}

	return nullptr;
}

void CModel::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* NodeAnim)
{
	// we need at least two values to interpolate...
	if (NodeAnim->mNumScalingKeys == 1) 
	{
		Out = NodeAnim->mScalingKeys[0].mValue;
		return;
	}

	UINT ScalingIndex = FindScaling(AnimationTimeTicks, NodeAnim);
	UINT NextScalingIndex = ScalingIndex + 1;
	assert(NextScalingIndex < NodeAnim->mNumScalingKeys);
	float t1 = (float)NodeAnim->mScalingKeys[ScalingIndex].mTime;
	float t2 = (float)NodeAnim->mScalingKeys[NextScalingIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTimeTicks - (float)t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = NodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = NodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

UINT CModel::FindScaling(float AnimationTimeTicks, const aiNodeAnim* NodeAnim)
{
	assert(NodeAnim->mNumScalingKeys > 0);

	for (UINT i = 0; i < NodeAnim->mNumScalingKeys - 1; i++) 
	{
		float t = (float)NodeAnim->mScalingKeys[i + 1].mTime;
		if (AnimationTimeTicks < t) 
		{
			return i;
		}
	}

	return 0;
}

void CModel::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* NodeAnim)
{
	// we need at least two values to interpolate...
	if (NodeAnim->mNumRotationKeys == 1) 
	{
		Out = NodeAnim->mRotationKeys[0].mValue;
		return;
	}

	UINT RotationIndex = FindRotation(AnimationTimeTicks, NodeAnim);
	UINT NextRotationIndex = RotationIndex + 1;
	assert(NextRotationIndex < NodeAnim->mNumRotationKeys);
	float t1 = (float)NodeAnim->mRotationKeys[RotationIndex].mTime;
	float t2 = (float)NodeAnim->mRotationKeys[NextRotationIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTimeTicks - t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = NodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = NodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out.Normalize();
}

UINT CModel::FindRotation(float AnimationTimeTicks, const aiNodeAnim* NodeAnim)
{
	assert(NodeAnim->mNumRotationKeys > 0);

	for (UINT i = 0; i < NodeAnim->mNumRotationKeys - 1; i++) 
	{
		float t = (float)NodeAnim->mRotationKeys[i + 1].mTime;
		if (AnimationTimeTicks < t) 
		{
			return i;
		}
	}

	return 0;
}

void CModel::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* NodeAnim)
{
	// we need at least two values to interpolate...
	if (NodeAnim->mNumPositionKeys == 1) {
		Out = NodeAnim->mPositionKeys[0].mValue;
		return;
	}

	UINT PositionIndex = FindPosition(AnimationTimeTicks, NodeAnim);
	UINT NextPositionIndex = PositionIndex + 1;
	assert(NextPositionIndex < NodeAnim->mNumPositionKeys);
	float t1 = (float)NodeAnim->mPositionKeys[PositionIndex].mTime;
	float t2 = (float)NodeAnim->mPositionKeys[NextPositionIndex].mTime;
	float DeltaTime = t2 - t1;
	float Factor = (AnimationTimeTicks - t1) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = NodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = NodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

UINT CModel::FindPosition(float AnimationTimeTicks, const aiNodeAnim* NodeAnim)
{
	for (UINT i = 0; i < NodeAnim->mNumPositionKeys - 1; i++)
	{
		float t = (float)NodeAnim->mPositionKeys[i + 1].mTime;
		if (AnimationTimeTicks < t) 
		{
			return i;
		}
	}

	return 0;
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