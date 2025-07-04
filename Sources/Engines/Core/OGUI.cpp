#include "PCH.h"
#include "ODirectX11.h"
#include "OGUI.h"
#include "OWindow.h"
#include "OWorld.h"
#include "WConsole.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

OGUI::OGUI(const OWindow& InWindow, const ODirectX11& InDirectX11, const OWorld& InWorld)
	: Object()
{
	Window = &InWindow;
	DirectX11 = &InDirectX11;
	World = &InWorld;
}

OGUI::~OGUI()
{

}

void OGUI::MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam)
{
	ImGui_ImplWin32_WndProcHandler(InHWnd, InMsg, InWParam, InLParam);
}

void OGUI::Init()
{
	Object::Init();

	// Setup Dear ImGui context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.FontGlobalScale = 1.75;

	// Setup Dear ImGui style.
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends.
	ImGui_ImplWin32_Init(Window->GetHWnd());
	ImGui_ImplDX11_Init(&DirectX11->GetDevice(), &DirectX11->GetDeviceContext());

	// Add default gui.
	{
		TAddWidget<WConsole>();
	}

	//for (const auto& Widget : Widgets)
	//{
	//	if (Widget->IsEnable)
	//	{
	//		Widget->Render();
	//	}
	//}

	for (const auto& Widget : Widgets)
	{
		Widget->Init();
	}
}

void OGUI::Shutdown()
{
	Object::Shutdown();

	for (const auto& Widget : Widgets)
	{
		Widget->Shutdown();
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void OGUI::Start()
{
	Object::Start();

	for (const auto& Widget : Widgets)
	{
		if (Widget->IsEnable)
		{
			Widget->Start();
		}
	}
}

void OGUI::Tick()
{
	Object::Tick();

	for (const auto& Widget : Widgets)
	{
		if (Widget->IsEnable)
		{
			Widget->Tick();
		}
	}

	DirectX11->SetDepthStencilState(ODirectX11::ERenderMode::R_2D);
	DirectX11->SetRasterizerState(ODirectX11::ERenderMode::R_2D);
	DirectX11->SetRenderTargets(ODirectX11::ERenderMode::R_2D);
	DirectX11->SetViewport(ODirectX11::ERenderMode::R_2D);

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (const auto& Widget : Widgets)
	{
		if (Widget->IsEnable)
		{
			Widget->Render();
		}
	}

	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void OGUI::End()
{
	Object::End();

	for (const auto& Widget : Widgets)
	{
		if (Widget->IsEnable)
		{
			Widget->End();
		}
	}
}