#include "PCH.h"
#include "SConsole.h"
#include "ODirectX11.h"
#include "OWindow.h"

uint8							ODirectX11::bIsScreenSizeChanged = 0;
uint8							ODirectX11::bIsVsyncEnabled = 0;

ODirectX11::ODirectX11(const OWindow& Window): Window(&Window), SwapChainDesc(), FeatureLevel(), Viewport2D(),
                                               DepthStencilBufferDesc2D(),
                                               DepthStencilStateDesc2D(),
                                               DepthStencilViewDesc2D(),
                                               RasterizerDesc2D(), Viewport3D(),
                                               DepthStencilBufferDesc3D(),
                                               DepthStencilStateDesc3D(),
                                               DepthStencilViewDesc3D(),
                                               RasterizerDesc3D(), AdapterDesc(), DisplayMaxFPS(0)
{
}

ODirectX11::~ODirectX11()
{
}

Object::EHandleResultType ODirectX11::MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam)
{
	if (InMsg == WM_SIZE)
	{
		bIsScreenSizeChanged = 1;
	}

	return EHandleResultType::Success;
}

Object::EHandleResultType ODirectX11::Initialize()
{
	HRESULT								Result;
	ComPtr<IDXGIFactory>				Factory;
	ComPtr<IDXGIAdapter>				Adapter;
	ComPtr<IDXGIOutput>					AdapterOutput;
	std::unique_ptr<DXGI_MODE_DESC[]>	DisplayModeList;
	UINT								NumModes;


	// Create a DirectX graphics interface factory.
	Result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(Factory.GetAddressOf()));
	if (FAILED(Result))
	{
		SConsole::LogError(L"CreateDXGIFactory() is failed.");
		return EHandleResultType::Failed;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	Result = Factory->EnumAdapters(0, Adapter.GetAddressOf());
	if (FAILED(Result))
	{
		SConsole::LogError(L"EnumAdapters() is failed.");
		return EHandleResultType::Failed;
	}

	// Enumerate the primary adapter output (monitor).
	Result = Adapter->EnumOutputs(0, AdapterOutput.GetAddressOf());
	if (FAILED(Result))
	{
		SConsole::LogError(L"EnumOutputs() is failed.");
		return EHandleResultType::Failed;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	NumModes = 0;
	Result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &NumModes, nullptr);
	if (FAILED(Result))
	{
		SConsole::LogError(L"GetDisplayModeList() is failed.");
		return EHandleResultType::Failed;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	DisplayModeList = std::make_unique<DXGI_MODE_DESC[]>(NumModes);
	{
		// Now fill the display mode list structures.
		Result = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &NumModes, DisplayModeList.get());
		if (FAILED(Result))
		{
			SConsole::LogError(L"GetDisplayModeList() is failed.");
			return EHandleResultType::Failed;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		uint32 Numerator = 0;
		uint32 Denominator = 0;
		for (uint32 i = 0; i < NumModes; i++)
		{
			if (DisplayModeList[i].Width == Window->GetWindowScreenWidth())
			{
				if (DisplayModeList[i].Height == Window->GetWindowScreenHeight())
				{
					Numerator = DisplayModeList[i].RefreshRate.Numerator;
					Denominator = DisplayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		// Get refresh-rate.
		if (Numerator == 0 || Denominator == 0)
		{
			DisplayMaxFPS = 60;
		}
		else
		{
			DisplayMaxFPS = Numerator / Denominator;
		}
	}

	// Get the adapter (video card) description.
	Result = Adapter->GetDesc(&AdapterDesc);
	if (FAILED(Result))
	{
		SConsole::LogError(L"GetDesc() is failed.");
		return EHandleResultType::Failed;
	}

	// Set the feature level to DirectX 11.
	FeatureLevel = D3D_FEATURE_LEVEL_11_0;


	// Initialize SwapChain.
	{
		ReleaseDeviceAndSwapChain();

		ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
		{
			SwapChainDesc.BufferCount = 2;
			SwapChainDesc.BufferDesc.Width = Window->GetClientScreenWidth();
			SwapChainDesc.BufferDesc.Height = Window->GetClientScreenHeight();
			SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

			if (bIsVsyncEnabled)
			{
				SwapChainDesc.BufferDesc.RefreshRate.Numerator = DisplayMaxFPS;
				SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			}
			else
			{
				SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
				SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			}

			SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SwapChainDesc.OutputWindow = Window->GetHWnd();
			SwapChainDesc.SampleDesc.Count = 1;
			SwapChainDesc.SampleDesc.Quality = 0;
			SwapChainDesc.Windowed = true;
			SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			SwapChainDesc.Flags = 0;
		}

		if (CreateDeviceAndSwapChain(SwapChain.GetAddressOf(),Device.GetAddressOf(),DeviceContext.GetAddressOf(), SwapChainDesc) != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
	}


	// Initialize DepthStencilBuffer2D.
	{
		ReleaseDepthStencilBuffer2D();

		ZeroMemory(&DepthStencilBufferDesc2D, sizeof(DepthStencilBufferDesc2D));
		{
			DepthStencilBufferDesc2D.Width = Window->GetClientScreenWidth();
			DepthStencilBufferDesc2D.Height = Window->GetClientScreenHeight();
			DepthStencilBufferDesc2D.MipLevels = 1;
			DepthStencilBufferDesc2D.ArraySize = 1;
			DepthStencilBufferDesc2D.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			DepthStencilBufferDesc2D.SampleDesc.Count = 1;
			DepthStencilBufferDesc2D.SampleDesc.Quality = 0;
			DepthStencilBufferDesc2D.Usage = D3D11_USAGE_DEFAULT;
			DepthStencilBufferDesc2D.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			DepthStencilBufferDesc2D.CPUAccessFlags = 0;
			DepthStencilBufferDesc2D.MiscFlags = 0;
		}

		if (CreateDepthStencilBuffer(DepthStencilBuffer2D.GetAddressOf(), DepthStencilBufferDesc2D) != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
	}


	// Initialize DepthStencilBuffer3D.
	{
		ReleaseDepthStencilBuffer3D();

		ZeroMemory(&DepthStencilBufferDesc3D, sizeof(DepthStencilBufferDesc3D));
		{
			DepthStencilBufferDesc3D.Width = Window->GetClientScreenWidth();
			DepthStencilBufferDesc3D.Height = Window->GetClientScreenHeight();
			DepthStencilBufferDesc3D.MipLevels = 1;
			DepthStencilBufferDesc3D.ArraySize = 1;
			DepthStencilBufferDesc3D.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			DepthStencilBufferDesc3D.SampleDesc.Count = 1;
			DepthStencilBufferDesc3D.SampleDesc.Quality = 0;
			DepthStencilBufferDesc3D.Usage = D3D11_USAGE_DEFAULT;
			DepthStencilBufferDesc3D.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			DepthStencilBufferDesc3D.CPUAccessFlags = 0;
			DepthStencilBufferDesc3D.MiscFlags = 0;
		}

		if (CreateDepthStencilBuffer(DepthStencilBuffer3D.GetAddressOf(), DepthStencilBufferDesc3D) != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
	}


	// Initialize DepthStencilState2D.
	{
		ReleaseDepthStencilState2D();

		ZeroMemory(&DepthStencilStateDesc2D, sizeof(DepthStencilStateDesc2D));
		{
			DepthStencilStateDesc2D.DepthEnable = false;
			DepthStencilStateDesc2D.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			DepthStencilStateDesc2D.DepthFunc = D3D11_COMPARISON_LESS;

			DepthStencilStateDesc2D.StencilEnable = false;
			DepthStencilStateDesc2D.StencilReadMask = 0xFF;
			DepthStencilStateDesc2D.StencilWriteMask = 0xFF;

			DepthStencilStateDesc2D.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			DepthStencilStateDesc2D.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			DepthStencilStateDesc2D.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			DepthStencilStateDesc2D.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			DepthStencilStateDesc2D.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			DepthStencilStateDesc2D.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			DepthStencilStateDesc2D.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			DepthStencilStateDesc2D.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		}

		if (CreateDepthStencilState(DepthStencilState2D.GetAddressOf(), DepthStencilStateDesc2D) != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
	}


	// Initialize DepthStencilState3D.
	{
		ReleaseDepthStencilState3D();

		ZeroMemory(&DepthStencilStateDesc3D, sizeof(DepthStencilStateDesc3D));
		{
			DepthStencilStateDesc3D.DepthEnable = true;
			DepthStencilStateDesc3D.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			DepthStencilStateDesc3D.DepthFunc = D3D11_COMPARISON_LESS;

			DepthStencilStateDesc3D.StencilEnable = true;
			DepthStencilStateDesc3D.StencilReadMask = 0xFF;
			DepthStencilStateDesc3D.StencilWriteMask = 0xFF;

			DepthStencilStateDesc3D.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			DepthStencilStateDesc3D.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			DepthStencilStateDesc3D.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			DepthStencilStateDesc3D.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			DepthStencilStateDesc3D.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			DepthStencilStateDesc3D.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			DepthStencilStateDesc3D.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			DepthStencilStateDesc3D.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		}

		if (CreateDepthStencilState(DepthStencilState3D.GetAddressOf(), DepthStencilStateDesc3D) != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
	}


	// Initialize DepthStencilView2D.
	{
		ReleaseDepthStencilView2D();

		ZeroMemory(&DepthStencilViewDesc2D, sizeof(DepthStencilViewDesc2D));
		{
			DepthStencilViewDesc2D.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			DepthStencilViewDesc2D.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			DepthStencilViewDesc2D.Texture2D.MipSlice = 0;
		}

		if (CreateDepthStencilView(DepthStencilBuffer2D.Get(), DepthStencilView2D.GetAddressOf(), DepthStencilViewDesc2D) != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
	}


	// Initialize DepthStencilView3D.
	{
		ReleaseDepthStencilView3D();

		ZeroMemory(&DepthStencilViewDesc3D, sizeof(DepthStencilViewDesc3D));
		{
			DepthStencilViewDesc3D.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			DepthStencilViewDesc3D.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			DepthStencilViewDesc3D.Texture2D.MipSlice = 0;
		}

		if (CreateDepthStencilView(DepthStencilBuffer3D.Get(), DepthStencilView3D.GetAddressOf(), DepthStencilViewDesc3D) != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
	}


	// Initialize RasterizerState2D.
	{
		ReleaseRasterizerState2D();

		ZeroMemory(&RasterizerDesc2D, sizeof(RasterizerDesc2D));
		{
			RasterizerDesc2D.AntialiasedLineEnable = false;
			RasterizerDesc2D.CullMode = D3D11_CULL_BACK;
			RasterizerDesc2D.DepthBias = 0;
			RasterizerDesc2D.DepthBiasClamp = 0.0f;
			RasterizerDesc2D.DepthClipEnable = true;
			RasterizerDesc2D.FillMode = D3D11_FILL_SOLID;
			RasterizerDesc2D.FrontCounterClockwise = false;
			RasterizerDesc2D.MultisampleEnable = false;
			RasterizerDesc2D.ScissorEnable = false;
			RasterizerDesc2D.SlopeScaledDepthBias = 0.0f;
		}

		if (CreateRasterizerState(RasterizerState2D.GetAddressOf(), RasterizerDesc2D) != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
	}


	// Initialize RasterizerState3D.
	{
		ReleaseRasterizerState3D();

		ZeroMemory(&RasterizerDesc3D, sizeof(RasterizerDesc3D));
		{
			RasterizerDesc3D.AntialiasedLineEnable = false;
			RasterizerDesc3D.CullMode = D3D11_CULL_BACK;
			RasterizerDesc3D.DepthBias = 0;
			RasterizerDesc3D.DepthBiasClamp = 0.0f;
			RasterizerDesc3D.DepthClipEnable = true;
			RasterizerDesc3D.FillMode = D3D11_FILL_SOLID;
			RasterizerDesc3D.FrontCounterClockwise = false;
			RasterizerDesc3D.MultisampleEnable = false;
			RasterizerDesc3D.ScissorEnable = false;
			RasterizerDesc3D.SlopeScaledDepthBias = 0.0f;
		}

		if (CreateRasterizerState(RasterizerState3D.GetAddressOf(), RasterizerDesc3D) != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
	}


	// Initialize Viewport2D.
	{
		ZeroMemory(&Viewport2D, sizeof(Viewport2D));
		{
			Viewport2D.Width = static_cast<float>(Window->GetClientScreenWidth());
			Viewport2D.Height = static_cast<float>(Window->GetClientScreenHeight());
			Viewport2D.MinDepth = 0.0f;
			Viewport2D.MaxDepth = 1.0f;
			Viewport2D.TopLeftX = 0.0f;
			Viewport2D.TopLeftY = 0.0f;
		}
	}


	// Initialize Viewport3D.
	{
		ZeroMemory(&Viewport3D, sizeof(Viewport3D));
		{
			Viewport3D.Width = static_cast<float>(Window->GetClientScreenWidth());
			Viewport3D.Height = static_cast<float>(Window->GetClientScreenHeight());
			Viewport3D.MinDepth = 0.0f;
			Viewport3D.MaxDepth = 1.0f;
			Viewport3D.TopLeftX = 0.0f;
			Viewport3D.TopLeftY = 0.0f;
		}
	}


	return EHandleResultType::Success;
}

void ODirectX11::Release()
{
}

void ODirectX11::Tick()
{
	if (bIsScreenSizeChanged == 1)
	{
		Resize();
		bIsScreenSizeChanged = 0;
	}

	ClearRenderTargetView(ERenderModeType::Model);
	ClearDepthStencilView(ERenderModeType::Model);
	ClearRenderTargetView(ERenderModeType::Interface);
	ClearDepthStencilView(ERenderModeType::Interface);
}

void ODirectX11::ClearRenderTargetView(ERenderModeType InType, XMFLOAT4 InClearColor) const
{
	float ClearColor[4];

	ClearColor[0] = InClearColor.x;
	ClearColor[1] = InClearColor.y;
	ClearColor[2] = InClearColor.z;
	ClearColor[3] = InClearColor.w;

	if (InType == ERenderModeType::Interface)
	{
		DeviceContext->ClearRenderTargetView(RenderTargetView2D.Get(), ClearColor);
	}
	else if (InType == ERenderModeType::Model)
	{
		DeviceContext->ClearRenderTargetView(RenderTargetView3D.Get(), ClearColor);
	}
}

void ODirectX11::ClearDepthStencilView(ERenderModeType Type) const
{
	if (Type == ERenderModeType::Interface)
	{
		DeviceContext->ClearDepthStencilView(DepthStencilView2D.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	else if (Type == ERenderModeType::Model)
	{
		DeviceContext->ClearDepthStencilView(DepthStencilView3D.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}

void ODirectX11::Draw()
{
	if (bIsVsyncEnabled)
	{
		SwapChain->Present(1, 0);
	}
	else
	{
		SwapChain->Present(0, 0);
	}
}

void ODirectX11::SetRenderTargets(ERenderModeType InType) const
{
	if (InType == ERenderModeType::Interface)
	{
		DeviceContext->OMSetRenderTargets(1, RenderTargetView2D.GetAddressOf(), DepthStencilView2D.Get());
	}
	else if (InType == ERenderModeType::Model)
	{
		DeviceContext->OMSetRenderTargets(1, RenderTargetView3D.GetAddressOf(), DepthStencilView3D.Get());
	}
}

void ODirectX11::SetDepthStencilState(ERenderModeType InType) const
{
	if (InType == ERenderModeType::Interface)
	{
		DeviceContext->OMSetDepthStencilState(DepthStencilState2D.Get(), 1);
	}
	else if (InType == ERenderModeType::Model)
	{
		DeviceContext->OMSetDepthStencilState(DepthStencilState3D.Get(), 1);
	}
}

void ODirectX11::SetRasterizerState(ERenderModeType InType) const
{ 
	if (InType == ERenderModeType::Interface)
	{
		DeviceContext->RSSetState(RasterizerState2D.Get());
	}
	else if (InType == ERenderModeType::Model)
	{
		DeviceContext->RSSetState(RasterizerState3D.Get());
	}
}

void ODirectX11::SetViewport(ERenderModeType InType) const
{
	if (InType == ERenderModeType::Interface)
	{
		DeviceContext->RSSetViewports(1, &Viewport2D);
	}
	else if (InType == ERenderModeType::Model)
	{
		DeviceContext->RSSetViewports(1, &Viewport3D);
	}
}

ID3D11Device& ODirectX11::GetDevice() const
{
	return *Device.Get();
}

ID3D11DeviceContext& ODirectX11::GetDeviceContext() const
{
	return *DeviceContext.Get();
}

Object::EHandleResultType ODirectX11::CreateDeviceAndSwapChain(IDXGISwapChain** OutSwapChain, ID3D11Device** OutDevice,
	ID3D11DeviceContext** OutDeviceContext, const DXGI_SWAP_CHAIN_DESC& InSwapChainDesc)
{
	HRESULT Result;

	// Create swap chain, device, device context.
	Result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &FeatureLevel, 1,
		D3D11_SDK_VERSION, &InSwapChainDesc, OutSwapChain, OutDevice, nullptr, OutDeviceContext);
	if (FAILED(Result))
	{
		SConsole::LogError(L"D3D11CreateDeviceAndSwapChain() is failed.");
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

Object::EHandleResultType ODirectX11::CreateRenderTargetView(ID3D11RenderTargetView** OutRenderTargetView)
{
	HRESULT						Result;
	ComPtr<ID3D11Texture2D>		BackBuffer;

	// Get pointer to the back buffer.
	Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(BackBuffer.GetAddressOf()));
	if (FAILED(Result))
	{
		SConsole::LogError(L"GetBackBuffer() is failed.");
		return EHandleResultType::Failed;
	}

	// Create render target view from back buffer.
	Result = Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, OutRenderTargetView);
	if (FAILED(Result))
	{
		SConsole::LogError(L"CreateRenderTargetView() is failed.");
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

Object::EHandleResultType ODirectX11::CreateDepthStencilBuffer(ID3D11Texture2D** OutDepthStencilBuffer, const D3D11_TEXTURE2D_DESC& InDepthStencilBufferDesc)
{
	HRESULT						Result;

	// Create depth stencil buffer.
	Result = Device->CreateTexture2D(&InDepthStencilBufferDesc, nullptr, OutDepthStencilBuffer);
	if (FAILED(Result))
	{
		SConsole::LogError(L"CreateDepthStencilBuffer() is failed.");
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

Object::EHandleResultType ODirectX11::CreateDepthStencilState(ID3D11DepthStencilState** OutDepthStecilState, const D3D11_DEPTH_STENCIL_DESC& InDepthStencilStateDesc)
{
	HRESULT						Result;

	// Create depth stencil state.
	Result = Device->CreateDepthStencilState(&InDepthStencilStateDesc, OutDepthStecilState);
	if (FAILED(Result))
	{
		SConsole::LogError(L"CreateDepthStencilState() is failed.");
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

Object::EHandleResultType ODirectX11::CreateDepthStencilView(ID3D11Texture2D* InDepthStencilBuffer, ID3D11DepthStencilView** OutDepthStencilView, const D3D11_DEPTH_STENCIL_VIEW_DESC& InDepthStencilViewDesc)
{
	HRESULT						Result;

	// Create depth stencil view.
	Result = Device->CreateDepthStencilView(InDepthStencilBuffer, &InDepthStencilViewDesc, OutDepthStencilView);
	if (FAILED(Result))
	{
		SConsole::LogError(L"CreateDepthStencilView() is failed.");
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

Object::EHandleResultType ODirectX11::CreateRasterizerState(ID3D11RasterizerState** OutRasterizerState, const D3D11_RASTERIZER_DESC& InRasterStateDesc)
{
	HRESULT						Result;

	// Create rasterizer state.
	Result = Device->CreateRasterizerState(&InRasterStateDesc, OutRasterizerState);
	if (FAILED(Result))
	{
		SConsole::LogError(L"CreateRasterizerState() is failed.");
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

void ODirectX11::ReleaseDeviceAndSwapChain()
{
	if (SwapChain)
	{
		SwapChain->Release();
	}

	if (Device)
	{
		Device->Release();
	}

	if (DeviceContext)
	{
		DeviceContext->Release();
	}
}

void ODirectX11::ReleaseRenderTargetView2D()
{
	if (RenderTargetView2D)
	{
		RenderTargetView2D->Release();
	}
}

void ODirectX11::ReleaseDepthStencilBuffer2D()
{
	if (DepthStencilBuffer2D)
	{
		DepthStencilBuffer2D->Release();
	}
}

void ODirectX11::ReleaseDepthStencilState2D()
{
	if (DepthStencilState2D)
	{
		DepthStencilState2D->Release();
	}
}

void ODirectX11::ReleaseDepthStencilView2D()
{
	if (DepthStencilView2D)
	{
		DepthStencilView2D->Release();
	}
}

void ODirectX11::ReleaseRasterizerState2D()
{
	if (RasterizerState2D)
	{
		RasterizerState2D->Release();
	}
}

void ODirectX11::ReleaseRenderTargetView3D()
{
	if (RenderTargetView3D)
	{
		RenderTargetView3D->Release();
	}
}

void ODirectX11::ReleaseDepthStencilBuffer3D()
{
	if (DepthStencilBuffer3D)
	{
		DepthStencilBuffer3D->Release();
	}
}

void ODirectX11::ReleaseDepthStencilState3D()
{
	if (DepthStencilState3D)
	{
		DepthStencilState3D->Release();
	}
}

void ODirectX11::ReleaseDepthStencilView3D()
{
	if (DepthStencilView3D)
	{
		DepthStencilView3D->Release();
	}
}

void ODirectX11::ReleaseRasterizerState3D()
{
	if (RasterizerState3D)
	{
		RasterizerState3D->Release();
	}
}

void ODirectX11::Resize()
{
	// Release out-dated.
	{
		ReleaseRenderTargetView2D();
		ReleaseRenderTargetView3D();
		ReleaseDepthStencilBuffer2D();
		ReleaseDepthStencilBuffer3D();
		ReleaseDepthStencilView2D();
		ReleaseDepthStencilView3D();
	}

	// Update desc.
	uint32 ClientScreenWidth = Window->GetClientScreenWidth();
	uint32 ClientScreenHeight = Window->GetClientScreenHeight();;
	{
		DepthStencilBufferDesc2D.Width = ClientScreenWidth;
		DepthStencilBufferDesc2D.Height = ClientScreenHeight;

		DepthStencilBufferDesc3D.Width = ClientScreenWidth;
		DepthStencilBufferDesc3D.Height = ClientScreenHeight;

		Viewport2D.Width = static_cast<float>(ClientScreenWidth);
		Viewport2D.Height = static_cast<float>(ClientScreenHeight);

		Viewport3D.Width = static_cast<float>(ClientScreenWidth);
		Viewport3D.Height = static_cast<float>(ClientScreenHeight);
	}

	// Re-Create buffers and views.
	{
		SwapChain->ResizeBuffers(0, Window->GetClientScreenWidth(), Window->GetClientScreenHeight(), DXGI_FORMAT_UNKNOWN, 0);

		CreateRenderTargetView(RenderTargetView2D.GetAddressOf());
		CreateRenderTargetView(RenderTargetView3D.GetAddressOf());

		CreateDepthStencilBuffer(DepthStencilBuffer2D.GetAddressOf(), DepthStencilBufferDesc2D);
		CreateDepthStencilBuffer(DepthStencilBuffer3D.GetAddressOf(), DepthStencilBufferDesc3D);

		CreateDepthStencilView(DepthStencilBuffer2D.Get(), DepthStencilView2D.GetAddressOf(), DepthStencilViewDesc2D);
		CreateDepthStencilView(DepthStencilBuffer3D.Get(), DepthStencilView3D.GetAddressOf(), DepthStencilViewDesc3D);
	}
	
}
