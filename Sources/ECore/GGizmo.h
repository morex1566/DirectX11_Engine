#pragma once
#include "OGameObject.h"

class GGizmo : public OGameObject
{
public:
	GGizmo(float InGridGap = 10.0f,
		   float InGridLength = 50000.0f,
		   uint64 InGridSize = 50000,
		   XMFLOAT4 InGridColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.1f));
	GGizmo(const GGizmo&)						= default;
	GGizmo& operator=(const GGizmo&)			= default;
	GGizmo(GGizmo&&) noexcept					= default;
	GGizmo& operator=(GGizmo&&) noexcept		= default;
	~GGizmo() override;

	void										Initialize() override;
	void										Release() override;
	/**
	 * \brief Called only once before entering the main loop.
	 */
	void										Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	void										Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	void										End() override;

	FORCEINLINE void							SetSpaceBtwGridByGrid(float InValue) { GridGap = InValue; }
	FORCEINLINE void							SetGridColor(XMFLOAT4 InColor) { GridColor = InColor; }

private:
	float										GridGap;
	float										GridLength;
	uint64										GridSize;
	XMFLOAT4									GridColor;

};

