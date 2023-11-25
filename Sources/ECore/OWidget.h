#pragma once
#include "Object.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

class OWidget : public Object
{
public:
	OWidget();
	OWidget(const OWidget&)									= default;
	OWidget& operator=(const OWidget&)						= default;
	OWidget(OWidget&&) noexcept								= default;
	OWidget& operator=(OWidget&&) noexcept					= default;
	~OWidget() override;

	void													Initialize() override;
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
	virtual void											Render() = 0;
};