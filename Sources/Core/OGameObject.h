#pragma once
#include "Object.h"

class OGameObject : public Object
{
public:
	OGameObject()													= default;
	OGameObject(const OGameObject&)									= default;
	OGameObject& operator=(const OGameObject&)						= default;
	OGameObject(OGameObject&&) noexcept								= default;
	OGameObject& operator=(OGameObject&&) noexcept					= default;
	virtual ~OGameObject() override;

	virtual EHandleResultType										Initialize() override;
	virtual void													Release() override;

	/**
	 * \brief Called only once before entering the main loop.
	 */
	virtual void													Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	virtual void													Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	virtual void													End() override;

protected:

private:

};