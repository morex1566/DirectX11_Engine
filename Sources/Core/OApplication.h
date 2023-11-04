#pragma once
#include "GCamera.h"
#include "Object.h"
#include "ODirectX11.h"
#include "OGUI.h"
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

	static uint8											CheckIsPlaying();

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

	void													Draw();

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
	 * \brief Engine's gui manager instance.
	 */
	std::shared_ptr<OGUI>									GUI;
	/**
	 * \brief game objects where located in hierarchy are here.
	 */
	std::shared_ptr<OWorld>									World;
	/**
	 * \brief Current application's status.
	 */
	static uint8											bIsPlaying;
};
