#pragma once
#include "OGameObject.h"

class CLine;

class GGizmo : public OGameObject
{
public:
	GGizmo(float InSpaceBtwGridByGrid = 10.0f,
		   XMFLOAT4 InGridColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
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

	FORCEINLINE void							SetSpaceBtwGridByGrid(float InValue) { SpaceBtwGridByGrid = InValue; }
	FORCEINLINE void							SetGridColor(XMFLOAT4 InColor) { GridColor = InColor; }

private:
	CLine*										Line;
	float										SpaceBtwGridByGrid;
	XMFLOAT4									GridColor;
};

