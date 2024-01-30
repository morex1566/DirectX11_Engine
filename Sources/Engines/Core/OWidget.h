#pragma once
#include "Object.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

class OWidget : public Object, public IName, public IEnable, public ITag
{
public:
	OWidget();
	OWidget(const OWidget&)									= default;
	OWidget& operator=(const OWidget&)						= default;
	OWidget(OWidget&&) noexcept								= default;
	OWidget& operator=(OWidget&&) noexcept					= default;
	~OWidget() override;


public:
	void		Init() override {}
	void		Shutdown() override {}
	void		Start() override {}
	void		Tick() override {}
	void		End() override {}


public:
	virtual void	Render() = 0;


};