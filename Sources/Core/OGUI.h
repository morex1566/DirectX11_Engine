#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Object.h"

class OWindow;
class ODirectX11;

class OGUI : public Object
{
public:
	OGUI(const OWindow& InWindow, const ODirectX11& InDirectX11);
	OGUI(const OGUI&)										= default;
	OGUI& operator=(const OGUI&)							= default;
	OGUI(OGUI&&) noexcept									= default;
	OGUI& operator=(OGUI&&) noexcept						= default;
	~OGUI() override;

	static EHandleResultType CALLBACK MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam);

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
	void													Render();

private:
	const OWindow*											Window; // ReadOnly
	const ODirectX11*										DirectX11; // ReadOnly
};