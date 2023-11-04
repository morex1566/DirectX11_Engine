#include "PCH.h"
#include "OApplication.h"
#include "ODirectX11.h"
#include "OWidget.h"
#include "OWindow.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);


OWidget::OWidget(const OWindow& InWindow, const ODirectX11& InDirectX11)
{
	Window = &InWindow;
	DirectX11 = &InDirectX11;
}

OWidget::~OWidget()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

OApplication::EHandleResultType OWidget::MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam)
{
	if (ImGui_ImplWin32_WndProcHandler(InHWnd, InMsg, InWParam, InLParam))
	{
		return EHandleResultType::Success;
	}

	return EHandleResultType::Success;
}

Object::EHandleResultType OWidget::Initialize()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.FontGlobalScale = 1.5f;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(Window->GetHWnd());
	ImGui_ImplDX11_Init(&DirectX11->GetDevice(), &DirectX11->GetDeviceContext());

	return EHandleResultType::Success;
}

void OWidget::Release()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void OWidget::Start()
{
	Object::Start();
}

void OWidget::Tick()
{
	Object::Tick();

	DirectX11->SetDepthStencilState(ODirectX11::ERenderModeType::Interface);
	DirectX11->SetRasterizerState(ODirectX11::ERenderModeType::Interface);
	DirectX11->SetRenderTargets(ODirectX11::ERenderModeType::Interface);
	DirectX11->SetViewport(ODirectX11::ERenderModeType::Interface);

	Render();
}

void OWidget::End()
{
	Object::End();
}

void OWidget::Render()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Create your GUI here
	ImGui::Begin("My Window");

	// Add widgets to your window
	ImGui::Text("Hello, ImGui!");
	if (ImGui::Button("Click Me!")) {
		// Handle button click
	}

	ImGui::End();

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