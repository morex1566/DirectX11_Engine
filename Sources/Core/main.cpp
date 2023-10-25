#include "PCH.h"

#include "Camera.h"
#include "Console.h"
#include "Config.h"
#include "DirectX11.h"
#include "GUI.h"
#include "Window.h"
#include "GameObject.h"
#include "Transform.h"


// TODO : Need to better programming design.
static bool IsLooping = true;
static bool IsResizing = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

LRESULT WINAPI WndProc(HWND hWnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd_, msg_, wParam_, lParam_))
		return true;

    switch (msg_)
    {
		case WM_SIZE:
			IsResizing = true;
			break;

	    case WM_SYSCOMMAND:
	        if ((wParam_ & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
	            return 0;
	        break;

	    case WM_DESTROY:
	        ::PostQuitMessage(0);
	        return 0;

	    case WM_DPICHANGED:
	        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
	        {
	            const RECT* suggested_rect;
	            suggested_rect = reinterpret_cast<RECT*>(lParam_);
	            ::SetWindowPos(hWnd_, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
	        }
	        break;

		case WM_CLOSE:
			IsLooping = false;
			return 0;

        default:
			break;
    }

    return ::DefWindowProcW(hWnd_, msg_, wParam_, lParam_);
}

int main()
{
	std::unique_ptr<Console> console = std::make_unique<Console>();
	{
		if (!console->Initialize()) { return 0; }
	}

	std::unique_ptr<Config> config = std::make_unique<Config>();
	{
		if (!config->Initialize(CONFIG_DIR)) { return 0; }
	}

	std::unique_ptr<Window> window = std::make_unique<Window>();
	{
		if (!window->Initialize(WndProc,
								L"Engine",
								Config::GetWindowStartPosX(),
								Config::GetWindowStartPosY(),
							    Config::GetWindowWidth(),
								Config::GetWindowHeight(),
								Config::GetIsFullScreenEnabled())) { return 0; }
	}

	std::unique_ptr<DirectX11> directX11 = std::make_unique<DirectX11>();
	{
		if (!directX11->Initialize(*window,
								   Config::GetIsVsyncEnabled(),
								   Config::GetRefreshRateOption(),
								   Config::GetRefreshRate())) { return 0; }
	}

	std::unique_ptr<GUI> gui = std::make_unique<GUI>();
	{
		if (!gui->Initialize(window->GetHWND(), directX11->GetDevice().Get(), directX11->GetDeviceContext().Get())) { return 0; }
	}

	window->Show();

	// main loop
    while(IsLooping)
    {
	    MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

			if (IsResizing)
			{
				window->Resize();
				directX11->ResizeRenderTargetView(window->GetClientWidth(), window->GetClientHeight());
				IsResizing = false;
			}
        }

		window->Update();

		// Draw model vertex.
		directX11->BindRenderTarget(DirectX11::ERenderTargetViewType::Model);
		directX11->ClearRenderTargetView(DirectX11::ERenderTargetViewType::Model);

		// Draw interface vertex.
		directX11->BindRenderTarget(DirectX11::ERenderTargetViewType::Interface);
		directX11->ClearRenderTargetView(DirectX11::ERenderTargetViewType::Interface);
		gui->Draw();

		// Bind all of ID3D11RenderTargetView
		directX11->BindRenderTarget(DirectX11::ERenderTargetViewType::All);

		// Render all of ID3D11RenderTargetView.
		directX11->WaitForRefreshRate();
    }


	return 0;
}