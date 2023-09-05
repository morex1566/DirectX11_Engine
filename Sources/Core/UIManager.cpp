#include "pch.h"
#include "UIManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

void UIManager::Initialize(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, HWND hWnd_)
{
	// Setup the Imgui framework
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	{
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	}

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd_);
	ImGui_ImplDX11_Init(device_, deviceContext_);
}

void UIManager::Destroy()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void UIManager::Render()
{
	// (Your code process and dispatch Win32 messages)
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	counters++;
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	ImGui::Begin("Hello, world!");
	ImGui::Text("counter = %d", counters);
	ImGui::End();

	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::Clear()
{
	IManager<UIElement>::Clear();
}
