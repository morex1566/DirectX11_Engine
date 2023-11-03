#pragma once
#include "OGameObject.h"

class ODirectX11;

class GTestObject : public OGameObject
{
public:
	GTestObject(ODirectX11& InDirectX11);
	GTestObject(const GTestObject&)							= default;
	GTestObject& operator=(const GTestObject&)				= default;
	GTestObject(GTestObject&&) noexcept						= default;
	GTestObject& operator=(GTestObject&&) noexcept			= default;
	~GTestObject() override;

	EHandleResultType										Initialize() override;
	void													Release() override;

	/**
	 * \brief Called only once before entering the main loop.
	 */
	void													Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	void													Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	void													End() override;
	/**
	 * \brief Set the buffers in the input assembler.
	 */
	void													Render();

	UINT													GetVertexCount() const;
	UINT													GetIndexCount() const;

protected:

private:
	ODirectX11*											DirectX11;
	ID3D11Buffer*										VertexBuffer;
	ID3D11Buffer*										IndexBuffer;
	UINT												VertexCount;
	UINT												IndexCount;
	XMFLOAT4											Color;
	XMFLOAT3											BL;
	XMFLOAT3											TM;
	XMFLOAT3											BR;
};

