#pragma once
#include "OGameObject.h"

class GCamera;
class OWindow;
class ODirectX11;

class GTestObject : public OGameObject
{
public:
	GTestObject();
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

};

