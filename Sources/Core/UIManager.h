#pragma once

#include "UIElement.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

class UIManager : public IManager<UIElement>
{
public:
	UIManager()						= default;
	~UIManager() override			= default;

	void Initialize(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, HWND hWnd_);
	void Destroy();
	void Render();
	void Clear() override;

	int counters = 0;

};