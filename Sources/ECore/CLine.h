#pragma once
#include "OComponent.h"

class ODirectX11;

ECLASS()
class CLine : public OComponent
{
public:
	CLine(const OGameObject& InOwner,
		  const ODirectX11& InDirectX11);
	CLine(const CLine&)							= default;
	CLine& operator=(const CLine&)				= default;
	CLine(CLine&&) noexcept						= default;
	CLine& operator=(CLine&&) noexcept			= default;
	~CLine() override;

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
	 * \brief Add vertex which is used drawing line.
	 */
	void										AddVertex(const FVertex& InFirst, const FVertex& InSecond);
	/**
	 * \brief Add index which is used drawing line order.
	 */
	void										AddIndex(UINT InIndex);

	FORCEINLINE UINT							GetVertexCount() const { return Vertices.size(); }
	FORCEINLINE UINT							GetIndexCount() const { return Indices.size(); }

private:
	bool										CreateVertexBuffer();
	bool										CreateIndexBuffer();
	void										Render();

	const ODirectX11*							DirectX11;
	ComPtr<ID3D11Buffer>						VertexBuffer;
	ComPtr<ID3D11Buffer>						IndexBuffer;
	std::vector<FVertex>						Vertices;

	std::vector<UINT>							Indices;
};

