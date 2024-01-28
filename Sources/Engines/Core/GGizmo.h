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


public:
	void		Init() override;
	void		Shutdown() override;
	void		Start() override;
	void		Tick() override;
	void		End() override;


public:
	FORCEINLINE void	SetSpaceBtwGridByGrid(float InValue) { GridGap = InValue; }
	FORCEINLINE void	SetGridColor(XMFLOAT4 InColor) { GridColor = InColor; }


private:
	float				GridGap;
	float				GridLength;
	uint64				GridSize;
	XMFLOAT4			GridColor;


};

