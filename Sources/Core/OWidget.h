#pragma once
#include "Object.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

class OWindow;
class ODirectX11;

class OWidget : public Object
{
public:
	OWidget(const OWindow& InWindow, const ODirectX11& InDirectX11);
	OWidget(const OWidget&)										= default;
	OWidget& operator=(const OWidget&)							= default;
	OWidget(OWidget&&) noexcept									= default;
	OWidget& operator=(OWidget&&) noexcept						= default;
	~OWidget() override;

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

private:
	void													Render();

	const OWindow*											Window; // ReadOnly
	const ODirectX11*										DirectX11; // ReadOnly
};