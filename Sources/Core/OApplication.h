#pragma once
#include "GCamera.h"
#include "GTestObject.h"
#include "Object.h"
#include "ODirectX11.h"
#include "OGUI.h"
#include "OShader.h"
#include "OWindow.h"
#include "OWorld.h"

class OApplication final : public Object
{
public:
	OApplication(const OApplication&)						= delete;
	OApplication& operator=(const OApplication&)			= delete;
	OApplication(OApplication&&) noexcept					= delete;
	OApplication& operator=(OApplication&&) noexcept		= delete;
	~OApplication() override;

	static OApplication&									GetInstance();
	/**
	 * \brief End repetition of the main loop.
	 */
	static void												Quit();

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

	uint8													GetIsPlaying() const;
	const OWindow&											GetWindow() const;

protected:

private:
	OApplication();

	/**
	 * \brief Essential instance vector. etc) Window, DirectX, Config...
	 */
	std::vector<std::shared_ptr<Object>>					Objects;
	/**
	 * \brief Application Window frame instance.
	 */
	std::shared_ptr<OWindow>								Window;
	/**
	 * \brief Renderer instance.
	 */
	std::shared_ptr<ODirectX11>								DirectX11;
	/**
	 * \brief Viewport camera instance.
	 */
	std::shared_ptr<GCamera>								Camera;
	/**
	 * \brief Test 2D object.
	 */
	std::shared_ptr<GTestObject>							TestObject;
	/**
	 * \brief Test shader instance.
	 */
	std::shared_ptr<OShader>								Shader;
	/**
	 * \brief Engine's gui manager instance.
	 */
	std::shared_ptr<OGUI>									GUI;
	/**
	 * \brief Current application's status.
	 */
	static uint8											bIsPlaying;
};
