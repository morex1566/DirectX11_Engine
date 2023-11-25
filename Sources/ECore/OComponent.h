#pragma once
#include "Object.h"

class OGameObject;

ECLASS()
class OComponent : public Object
{
public:
	OComponent(const OGameObject& InOwner);
	OComponent(const OComponent&)						= default;
	OComponent& operator=(const OComponent&)			= default;
	OComponent(OComponent&&) noexcept					= default;
	OComponent& operator=(OComponent&&) noexcept		= default;
	virtual ~OComponent() override;

	void												Initialize() override;
	void												Release() override;

	/**
	 * \brief Called only once before entering the main loop.
	 */
	void												Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	void												Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	void												End() override;

protected:
	const OGameObject*									Owner;

};