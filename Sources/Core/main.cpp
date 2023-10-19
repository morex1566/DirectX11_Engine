#include "PCH.h"

#include "DirectX11.h"
#include "GUI.h"
#include "Window.h"
#include "Shader.h"

static bool IsLooping = true;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

LRESULT WINAPI WndProc(HWND hWnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd_, msg_, wParam_, lParam_))
		return true;

    switch (msg_)
    {
	    case WM_SIZE:
	        if (wParam_ == SIZE_MINIMIZED) { return 0; }

			DirectX11::ResizeRenderTargetView(LOWORD(lParam_), HIWORD(lParam_));
			DirectX11::OMSetRenderTarget();
			return 0;

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
	            const RECT* suggested_rect = reinterpret_cast<RECT*>(lParam_);
	            ::SetWindowPos(hWnd_, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
	        }
	        break;

		case WM_CLOSE:
			IsLooping = false;
			return 0;
    }

    return ::DefWindowProcW(hWnd_, msg_, wParam_, lParam_);
}

int main()
{
	std::unique_ptr<Window> window = std::make_unique<Window>();
	{
		if (!window->Initialize(WndProc, L"Engine", 800, 600)) { return 0; }
	}

	std::unique_ptr<DirectX11> directX11 = std::make_unique<DirectX11>();
	{
		if (!directX11->Initialize(window->GetHWND())) { return 0; }
	}

	std::unique_ptr<GUI> gui = std::make_unique<GUI>();
	{
		if (!gui->Initialize(window->GetHWND(), directX11->GetDevice().Get(), directX11->GetDeviceContext().Get())) { return 0; }
	}

	std::unique_ptr<Shader> shader = std::make_unique<Shader>();
	{
		shader->Initialize(directX11->GetDevice().Get(), window->GetHWND(), L"D:\\Devs\\Projects\\Engine\\Shaders\\Unlit.hlsl");
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
        }

		window->Update();

		directX11->OMSetRenderTarget();
		gui->Render(directX11->GetDeviceContext().Get(), directX11->GetRenderTargetView().Get());

		directX11->SyncWithVsync(true);
    }

	return 0;
}