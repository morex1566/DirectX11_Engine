#pragma once
#include "OComponent.h"

class ODirectX11;

class CMesh : public OComponent
{
public:
	CMesh(const OGameObject& InOwner, const ODirectX11& InDirectX11);
	CMesh(const CMesh&)							= default;
	CMesh& operator=(const CMesh&)				= default;
	CMesh(CMesh&&) noexcept						= default;
	CMesh& operator=(CMesh&&) noexcept			= default;
	virtual ~CMesh() override;

	virtual EHandleResultType					Initialize() override;
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

	void										AddVertex(const FVertex& InVertex);
	void										AddIndex(UINT InIndex);


	UINT										GetVertexCount() const;
	UINT										GetIndexCount() const;

private:
	EHandleResultType							CreateVertexBuffer();
	EHandleResultType							CreateIndexBuffer();
	void										Render();

	const ODirectX11*							DirectX11;
	ComPtr<ID3D11Buffer>						VertexBuffer;
	ComPtr<ID3D11Buffer>						IndexBuffer;
	std::vector<FVertex>						Vertices;
	std::vector<UINT>							Indices;
};

