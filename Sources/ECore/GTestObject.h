#pragma once
#include "OGameObject.h"

class GCamera;
class ODirectX11;
class OWindow;

class GTestObject : public OGameObject
{
public:
	GTestObject(const GCamera& InCamera,
				const OWindow& InWindow,
				const ODirectX11& InDirectX11);
	GTestObject(const GTestObject&)							= default;
	GTestObject& operator=(const GTestObject&)				= default;
	GTestObject(GTestObject&&) noexcept						= default;
	GTestObject& operator=(GTestObject&&) noexcept			= default;
	~GTestObject() override;

	void			    								    Initialize() override;
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
	const ODirectX11*										DirectX11;
	const OWindow*											Window;
};

