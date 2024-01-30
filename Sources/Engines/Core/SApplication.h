#pragma once
#include "GCamera.h"
#include "ODirectX11.h"
#include "OGUI.h"
#include "OInput.h"
#include "OTime.h"
#include "OWindow.h"
#include "OWorld.h"

class SApplication
{
public:
	SApplication(const SApplication&)						= delete;
	SApplication& operator=(const SApplication&)			= delete;
	SApplication(SApplication&&) noexcept					= delete;
	SApplication& operator=(SApplication&&) noexcept		= delete;
	~SApplication()											= default;

	static SApplication&									GetInstance();
	/**
	 * \brief End repetition of the main loop.
	 */
	static void												Quit();
	static uint8											CheckIsPlaying();
	void													Init(HINSTANCE InHInstance);
	void													Shutdown();
	/**
	 * \brief Called only once before entering the main loop.
	 */
	void													Start();
	/**
	 * \brief Called once when the every frame.
	 */
	void													Tick();
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	void													End();

	void													Draw();

	/**
	 * \brief Get current program's main window class.
	 */
	FORCEINLINE static const OWindow*						GetWindow() { return Window.get(); }
	/**
	 * \brief Get current d3d11 class.
	 */
	FORCEINLINE static const ODirectX11*					GetDirectX11() { return DirectX11.get(); }
	/**
	 * \brief Get current viewport camera class.
	 */
	FORCEINLINE static const GCamera*						GetCamera() { return Camera.get(); }
	/**
	 * \brief Get current program handle instance.
	 */
	FORCEINLINE static HINSTANCE							GetHInstance() { return HInstance; }
private:
	SApplication()											= default;

	/**
	 * \brief Essential instance vector. etc) Window, DirectX, Config...
	 */
	static std::vector<std::shared_ptr<Object>>				Objects;
	/**
	 * \brief Delta time instance.
	 */
	static std::shared_ptr<OTime>							Time;
	/**
	 * \brief Application Window frame instance.
	 */
	static std::shared_ptr<OWindow>							Window;
	/**
	 * \brief Renderer instance.
	 */
	static std::shared_ptr<ODirectX11>						DirectX11;
	/**
	 * \brief Input manager instance.
	 */
	static std::shared_ptr<OInput>							Input;
	/**
	 * \brief Viewport camera instance.
	 */
	static std::shared_ptr<GCamera>							Camera;
	/**
	 * \brief Engine's gui manager instance.
	 */
	static std::shared_ptr<OGUI>							GUI;
	/**
	 * \brief game objects where located in hierarchy are here.
	 */
	static std::shared_ptr<OWorld>							World;
	/**
	 * \brief Current application's status.
	 */
	static uint8											bIsPlaying;
	/**
	 * \brief Current application's handle.
	 */
	static HINSTANCE										HInstance;
};
