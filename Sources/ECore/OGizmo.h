#pragma once
#include "GCamera.h"
#include "Object.h"

class OGizmo : public Object
{
public:
	OGizmo(const GCamera& InCamera);
	OGizmo(const OGizmo&)									= default;
	OGizmo& operator=(const OGizmo&)						= default;
	OGizmo(OGizmo&&) noexcept								= default;
	OGizmo& operator=(OGizmo&&) noexcept					= default;
	~OGizmo() override;

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

