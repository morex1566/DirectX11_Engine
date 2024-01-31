#pragma once
#include "OComponent.h"

class CAnimator : public OComponent
{
public:
	CAnimator(OGameObject* InOwner);

	CAnimator()											= default;
	CAnimator(const CAnimator&)							= default;
	CAnimator& operator=(const CAnimator&)				= default;
	CAnimator(CAnimator&&) noexcept						= default;
	CAnimator& operator=(CAnimator&&) noexcept			= default;
	~CAnimator() override;


public:
	virtual void 					Init() override;
	virtual void					Shutdown() override;
	virtual void					Start() override;
	virtual void					Tick() override;
	virtual void					End() override;


};

