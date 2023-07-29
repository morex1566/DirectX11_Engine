// ReSharper disable CppPossiblyUninitializedMember

#include "pch.h"
#include "D3DManager.h"

#pragma warning(disable: 6214)
#pragma warning(disable: 26495)
#pragma warning(disable: 4018)
#pragma warning(disable: 6385)
#pragma warning(disable: 6387)
#pragma warning(disable: 6001)


D3DManager& D3DManager::Get()
{
	static D3DManager instance;

	return instance;
}

bool D3DManager::Initialize(const HWND& hWnd_)
{
	HRESULT result;

	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	ID3D11Texture2D* backBufferPtr;
	unsigned int numModes;
	unsigned int numerator;
	unsigned int denominator;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == static_cast<unsigned int>(Config::GET_ENGINE_SETTING_WINDOW_WIDTH()))
		{
			if (displayModeList[i].Height == static_cast<unsigned int>(Config::GET_ENGINE_SETTING_WINDOW_HEIGHT()))
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	_videoCardMemory = static_cast<unsigned int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	result = wcstombs_s(&_videoCardNameLength, _videoCardName, 128, adapterDesc.Description, 128);
	if (FAILED(result))
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();

	// Set the swap chain buffer
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	{
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = Config::GET_ENGINE_SETTING_WINDOW_WIDTH();
		swapChainDesc.BufferDesc.Height = Config::GET_ENGINE_SETTING_WINDOW_HEIGHT();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if (Config::GET_ENGINE_SETTING_WINDOW_VSYNC())
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = hWnd_;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if (Config::GET_ENGINE_SETTING_WINDOW_FULLSCREEN())
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		// Set the directX 11 version.
		_versionFeature = D3D_FEATURE_LEVEL_11_0;
	}

	// Create the swap chain, Direct3D device, and Direct3D device context.
	// TODO : if you don't have a directX11 video card, change D3D_DRIVER_TYPE_HARDWARE to D3D_DRIVER_TYPE_REFERENCE
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &_versionFeature, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, NULL, &_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr));
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	{
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.Width = Config::GET_ENGINE_SETTING_WINDOW_WIDTH();
		depthBufferDesc.Height = Config::GET_ENGINE_SETTING_WINDOW_HEIGHT();
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
	}

	// Create the texture for the depth buffer using the filled out description.
	result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilBufferDesc;
	{
		ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));

		// Set up the description of the stencil state.
		depthStencilBufferDesc.DepthEnable = true;
		depthStencilBufferDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilBufferDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilBufferDesc.StencilEnable = true;
		depthStencilBufferDesc.StencilReadMask = 0xFF;
		depthStencilBufferDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilBufferDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilBufferDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilBufferDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilBufferDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilBufferDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilBufferDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilBufferDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilBufferDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}

	// Create the depth stencil state.
	result = _device->CreateDepthStencilState(&depthStencilBufferDesc, &_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	{
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
	}

	// Create the depth stencil view.
	result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	{
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

		rasterizerDesc.AntialiasedLineEnable = false;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	}

	// Create the rasterizer state from the description we just filled out.
	result = _device->CreateRasterizerState(&rasterizerDesc, &_rasterizerState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	_deviceContext->RSSetState(_rasterizerState);

	// Setup the viewport for rendering.
	_viewport.Width = static_cast<float>(Config::GET_ENGINE_SETTING_WINDOW_WIDTH());
	_viewport.Height = static_cast<float>(Config::GET_ENGINE_SETTING_WINDOW_HEIGHT());
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;

	// Create the viewport.
	_deviceContext->RSSetViewports(1, &_viewport);

	// Setup the projection matrix.
	float fov = 3.141592654f / 4.0f;
	float screenAspect = static_cast<float>(Config::GET_ENGINE_SETTING_WINDOW_WIDTH()) /
		static_cast<float>(Config::GET_ENGINE_SETTING_WINDOW_HEIGHT());

	// Set matrixs.
	{
		_projectionMatrix = XMMatrixPerspectiveFovLH(
			fov,
			screenAspect,
			Config::GET_ENGINE_SETTING_WINDOW_SCREENNEAR(),
			Config::GET_ENGINE_SETTING_WINDOW_SCREENDEPTH()
		);

		_worldMatrix = XMMatrixIdentity();

		_orthoMatrix = XMMatrixOrthographicLH(
			static_cast<float>(Config::GET_ENGINE_SETTING_WINDOW_WIDTH()),
			static_cast<float>(Config::GET_ENGINE_SETTING_WINDOW_HEIGHT()),
			Config::GET_ENGINE_SETTING_WINDOW_SCREENNEAR(),
			Config::GET_ENGINE_SETTING_WINDOW_SCREENDEPTH()
		);
	}

	return true;
}

void D3DManager::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (_swapChain)
	{
		_swapChain->SetFullscreenState(false, NULL);
	}

	if (_rasterizerState)
	{
		_rasterizerState->Release();
		_rasterizerState = 0;
	}

	if (_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if (_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = 0;
	}

	if (_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}

	if (_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = 0;
	}

	if (_deviceContext)
	{
		_deviceContext->Release();
		_deviceContext = 0;
	}

	if (_device)
	{
		_device->Release();
		_device = 0;
	}

	if (_swapChain)
	{
		_swapChain->Release();
		_swapChain = 0;
	}
}

void D3DManager::BeginScene()
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = 0.1451f;
	color[1] = 0.1451f;
	color[2] = 0.1490f;
	color[3] = 1.0f;

	_deviceContext->ClearRenderTargetView(_renderTargetView, color);

	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DManager::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (Config::GET_ENGINE_SETTING_WINDOW_VSYNC())
	{
		// Lock to screen refresh rate.
		_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		_swapChain->Present(0, 0);
	}
}

void D3DManager::Clear()
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = 0.1451f;
	color[1] = 0.1451f;
	color[2] = 0.1490f;
	color[3] = 1.0f;

	_deviceContext->ClearRenderTargetView(_renderTargetView, color);

	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11Device* D3DManager::GetDevice() const
{
	return _device;
}

ID3D11DeviceContext* D3DManager::GetDeviceContext() const
{
	return _deviceContext;
}

XMMATRIX D3DManager::GetProjectionMatrix()
{
	return _projectionMatrix;
}

XMMATRIX D3DManager::GetWorldMatrix()
{
	return _worldMatrix;
}

XMMATRIX D3DManager::GetOrthoMatrix()
{
	return _orthoMatrix;
}

D3DManager::D3DManager()
{
	_device = nullptr;
	_deviceContext = nullptr;
	_swapChain = nullptr;
	_renderTargetView = nullptr;
	_depthStencilBuffer = nullptr;
	_depthStencilView = nullptr;
	_depthStencilState = nullptr;
	_rasterizerState = nullptr;
	_videoCardMemory = 0;
	_videoCardNameLength = 0;
	_versionFeature = D3D_FEATURE_LEVEL_11_0;
	_viewport = {};
	_projectionMatrix = XMMatrixIdentity();
	_worldMatrix = XMMatrixIdentity();
	_orthoMatrix = XMMatrixIdentity();
}
