#pragma once

#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "OComponent.h"
#include "CAnimator.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CLitShader.h"


class CModel : public OComponent
{
public:
	CModel(OGameObject* InOwner);

	CModel()										= default;
	CModel(const CModel&)							= default;
	CModel& operator=(const CModel&)				= default;
	CModel(CModel&&) noexcept						= default;
	CModel& operator=(CModel&&) noexcept			= default;
	~CModel() override;


public:
	virtual void		Init() override;
	virtual void		Shutdown() override;
	virtual void		Start() override;
	virtual void		Tick() override;
	virtual void		End() override;


public:
	void Load(const std::wstring& InFilePath);
	void LoadTexture(const std::wstring& InFilePath, ETexture InTextureType);
	void LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath);
	void AddVertex(const FVertex& InVertex);
	void AddIndex(UINT InIndex);
	void Render();


private:
	void Parse();
	void ParseMesh(const aiMesh* InMesh);
	void ParseBone(UINT InMeshIndex, const aiMesh* InMesh);
	void UpdateBoneTransforms(float InTimeinSeconds);
	void ReadNodeHierarchy(float AnimationTime, const aiNode* InNode, const XMMATRIX& InParentTransform);
	const aiNodeAnim* FindNodeAnimation(const aiAnimation* Anim, const std::string& NodeName);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* NodeAnim);
	UINT FindScaling(float AnimationTimeTicks, const aiNodeAnim* NodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTimeTicks, const aiNodeAnim* NodeAnim);
	UINT FindRotation(float AnimationTimeTicks, const aiNodeAnim* NodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTimeTicks, const aiNodeAnim* NodeAnim);
	UINT FindPosition(float AnimationTimeTicks, const aiNodeAnim* NodeAnim);
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	FORCEINLINE UINT GetVertexCount() { return VertexCount; }
	FORCEINLINE UINT GetIndexCount() { return IndexCount; }
	FORCEINLINE UINT GetBoneCount() { return BoneCount; }
	FORCEINLINE int GetBoneID(const std::string& InBoneName) 
	{
		if (BoneIDs.find(InBoneName) == BoneIDs.end())
		{
			BoneIDs[InBoneName] = (int)BoneIDs.size();
		}

		return BoneIDs[InBoneName];
	}
	

private:
	CTexture*							Texture;
	CLitShader*							Shader;
	std::vector<FVertex>				Vertices;
	std::vector<UINT>					Indices;

	// BoneID는 BoneTransforms의 인덱스와 매치.
	FBone*								BoneTransform;
	
	std::map<std::string, int>			BoneIDs;

	// Assimp Importer 인스턴스
	Assimp::Importer					Importer;
	const aiScene*						Scene;

	// Animation Test 용 시간
	float								Time = 0.0f;

	UINT								VertexCount;
	UINT								IndexCount;
	UINT								BoneCount;

	ID3D11Buffer*						VertexBuffer;
	ID3D11Buffer*						IndexBuffer;


};

