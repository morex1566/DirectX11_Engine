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
	 * \brief Add vertex which is used drawing triangle.
	 */
	void										AddVertex(const FVertex& InFirst, const FVertex& InSecond, const FVertex& InThird);
	/**
	 * \brief Add index which is used drawing triangle order.
	 */
	void										AddIndex(UINT InIndex);

	FORCEINLINE UINT							GetVertexCount() { return Vertices.size(); }
	FORCEINLINE UINT							GetIndexCount() { return Indices.size(); }

private:
	bool										CreateVertexBuffer();
	bool										CreateIndexBuffer();
	void										Render();

	ComPtr<ID3D11Buffer>						VertexBuffer;
	ComPtr<ID3D11Buffer>						IndexBuffer;
	std::vector<FVertex>						Vertices;
	std::vector<UINT>							Indices;
};

