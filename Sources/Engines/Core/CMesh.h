#pragma once
#include "OComponent.h"

class CMesh : public OComponent
{
public:
	CMesh(OGameObject* InOwner);

	CMesh();
	CMesh(const CMesh&)							= default;
	CMesh& operator=(const CMesh&)				= default;
	CMesh(CMesh&&) noexcept						= default;
	CMesh& operator=(CMesh&&) noexcept			= default;
	~CMesh() override;


public:
	virtual void								Init() override;
	virtual void								Shutdown() override;
	virtual void								Start() override;
	virtual void								Tick() override;
	virtual void								End() override;


public:
	void										AddVertex(const FVertex& InVertex);
	void										AddIndex(UINT InIndex);
	void										Render();
	FORCEINLINE UINT							GetVertexCount() { return Vertices.size(); }
	FORCEINLINE UINT							GetIndexCount() { return Indices.size(); }
	FORCEINLINE void							SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY InType) { PrimitiveType = InType; }


private:
	bool										CreateVertexBuffer();
	bool										CreateIndexBuffer();

	ComPtr<ID3D11Buffer>						VertexBuffer;
	ComPtr<ID3D11Buffer>						IndexBuffer;
	std::vector<FVertex>						Vertices;
	std::vector<UINT>							Indices;
	D3D11_PRIMITIVE_TOPOLOGY					PrimitiveType;

	// std::vector<UINT>						MeshBaseVertices;
	std::vector<FVertexBoneData>				VertexToBones;
	std::map<std::string, UINT>					BoneNameToIndexMap;

};


