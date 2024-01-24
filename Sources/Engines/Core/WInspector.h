#pragma once
#include "OWidget.h"

class OWorld;
class OGameObject;

class WInspector : public OWidget
{
public:
	WInspector(const OWorld& InWorld);
	WInspector(const WInspector&)					= default;
	WInspector& operator=(const WInspector&)		= default;
	WInspector(WInspector&&) noexcept				= default;
	WInspector& operator=(WInspector&&) noexcept	= default;
	~WInspector() override;

	void											Initialize() override;
	void											Release() override;
	/**
	 * \brief Called only once before entering the main loop.
	 */
	void											Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	void											Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	void											End() override;
	/**
	 * \brief Set game object which is showed at inspector.
	 */
	FORCEINLINE void								SetFocus(OGameObject* InInspectTarget) { FocusedGameObject = InInspectTarget; }

private:
	void											Render() override;

	OGameObject*									FocusedGameObject;
};

