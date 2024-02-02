#pragma once
#include "OComponent.h"

#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

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
	void LoadMesh(const std::wstring& InFilePath);
	void LoadTexture(const std::wstring& InFilePath, ETexture InTextureType);
	void LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath);
	void AddVertex(const FVertex& InVertex);
	void AddIndex(UINT InIndex);
	void Render();


private:
	void Parse(const aiScene* InScene);
	void ParseMesh(const aiMesh* InMesh, const aiScene* InScene);
	void ParseBone(UINT InMeshIndex, const aiMesh* InMesh);
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	FORCEINLINE UINT GetVertexCount() { return VertexCount; }
	FORCEINLINE UINT GetIndexCount() { return IndexCount; }
	FORCEINLINE UINT GetBoneCount() { return BoneCount; }


private:
	CTexture*							Texture;
	CLitShader*							Shader;
	std::vector<FVertex>				Vertices;
	std::vector<UINT>					Indices;
	std::vector<FBoneTransform>			BoneTransforms;

	UINT								VertexCount;
	UINT								IndexCount;
	UINT								BoneCount;

	ID3D11Buffer*						VertexBuffer;
	ID3D11Buffer*						IndexBuffer;


};

