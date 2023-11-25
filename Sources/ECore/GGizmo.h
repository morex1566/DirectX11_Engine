#pragma once
#include "OGameObject.h"

class GCamera;

class GGizmo : public OGameObject
{
public:
	GGizmo(const GCamera& InCamera);
	GGizmo(const GGizmo&)									= default;
	GGizmo& operator=(const GGizmo&)						= default;
	GGizmo(GGizmo&&) noexcept								= default;
	GGizmo& operator=(GGizmo&&) noexcept					= default;
	~GGizmo() override;

	void													Initialize() override;
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

private:
	const GCamera*											Camera;
};

