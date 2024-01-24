#pragma once
#include "OComponent.h"

ECLASS()
class CMesh : public OComponent
{
public:
	CMesh(const OGameObject* InOwner);
	CMesh(const CMesh&)							= default;
	CMesh& operator=(const CMesh&)				= default;
	CMesh(CMesh&&) noexcept						= default;
	CMesh& operator=(CMesh&&) noexcept			= default;
	~CMesh() override;

	virtual void								Initialize() override;
	virtual void								Release() override;

	/**
	 * \brief Called only once before entering the main loop.
	 */
	virtual void								Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	virtual void								Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	virtual void								End() override;
	/**
	 * \brief Add vertex.
	 */
	void										AddVertex(const FVertex& InVertex);
	/**
	 * \brief Add index.
	 */
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
};

