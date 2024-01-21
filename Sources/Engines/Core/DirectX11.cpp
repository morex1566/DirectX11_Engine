#include "PCH.h"
#include "DirectX11.h"

#pragma warning(disable : 6001) // use uninitialized value
#pragma warning(disable : 6385) // reading invalid data
#pragma warning(disable : 6387) // ptr could be nullptr


bool DirectX11::isVsyncEnabled = true;


DirectX11::DirectX11(HINSTANCE hInstance)
	: hInstance(hInstance)
{
}

void __stdcall DirectX11::MsgHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
}

void DirectX11::Init(unsigned int windowWidth, unsigned int windowHeight,
						HWND hOutputWindow, bool outputWindowFullScreenEnabled)
{
	HRESULT				result;

	// 모니터의 refresh-rate를 가져옵니다.
	IDXGIFactory*		factory;
	IDXGIAdapter*		adapter;
	IDXGIOutput*		adapterOutput;
	DXGI_MODE_DESC*		displayModeList;
	DXGI_ADAPTER_DESC	adapterDesc;
	unsigned int		numModes, i, numerator, denominator;
	{
		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			Console::LogError(L"CreateDXGIFactory()", __FILE__, __LINE__);
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
		{
			Console::LogError(L"EnumAdapters()", __FILE__, __LINE__);
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			Console::LogError(L"EnumOutputs()", __FILE__, __LINE__);
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			Console::LogError(L"GetDisplayModeList()", __FILE__, __LINE__);
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			Console::LogError(L"GetDisplayModeList()", __FILE__, __LINE__);
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)windowWidth)
			{
				if (displayModeList[i].Height == (unsigned int)windowHeight)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		delete[] displayModeList;
		displayModeList = 0;

		adapterOutput->Release();
		adapterOutput = 0;

		adapter->Release();
		adapter = 0;

		factory->Release();
		factory = 0;
	}


	// 다이렉트X 버전 설정
	D3D_FEATURE_LEVEL featureLevel;
	{
		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;
	}

	
	// 스왑체인, D3DDevice, D3DDeviceContext 설정/생성
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	{
		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = windowWidth;
		swapChainDesc.BufferDesc.Height = windowHeight;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if (isVsyncEnabled)
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
		swapChainDesc.OutputWindow = hOutputWindow;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if (outputWindowFullScreenEnabled)
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

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);
		if (FAILED(result))
		{
			Console::LogError(L"D3D11CreateDeviceAndSwapChain()", __FILE__, __LINE__);
		}
	}


	// 랜더타겟뷰 설정/생성
	ID3D11Texture2D* backBufferPtr;
	{
		// Get the pointer to the back buffer.
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result))
		{
			Console::LogError(L"swapChain->GetBuffer()", __FILE__, __LINE__);
		}

		// Create the render target view with the back buffer pointer.
		result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
		if (FAILED(result))
		{
			Console::LogError(L"device->CreateRenderTargetView()", __FILE__, __LINE__);
		}

		backBufferPtr->Release();
		backBufferPtr = 0;
	}


	// 깊이버퍼 설정/생성
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	{
		// Initialize the description of the depth buffer.
		ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));

		// Set up the description of the depth buffer.
		depthStencilBufferDesc.Width = windowWidth;
		depthStencilBufferDesc.Height = windowHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = device->CreateTexture2D(&depthStencilBufferDesc, NULL, &depthStencilBuffer);
		if (FAILED(result))
		{
			Console::LogError(L"device->CreateTexture2D()", __FILE__, __LINE__);
		}
	}


	// 깊이-스텐실 상태 설정/생성
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	{
		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilStateDesc, sizeof(depthStencilStateDesc));

		// Set up the description of the stencil state.
		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilStateDesc.StencilEnable = true;
		depthStencilStateDesc.StencilReadMask = 0xFF;
		depthStencilStateDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
		if (FAILED(result))
		{
			Console::LogError(L"device->CreateDepthStencilState()", __FILE__, __LINE__);
		}
	}


	// 깊이-스텐실 상태 바인딩
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);


	// 깊이-스텐실뷰 설정/생성
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	{
		// Initialize the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
		if (FAILED(result))
		{
			Console::LogError(L"device->CreateDepthStencilView()", __FILE__, __LINE__);
		}
	}


	// 랜더타겟뷰, 깊이-스텐실뷰 바인딩
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);


	// 레스터라이저 설정/생성
	D3D11_RASTERIZER_DESC rasterDesc;
	{
		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = device->CreateRasterizerState(&rasterDesc, &rasterState);
		if (FAILED(result))
		{
			Console::LogError(L"device->CreateRasterizerState()", __FILE__, __LINE__);
		}
	}


	// 레스터라이저 바인딩
	deviceContext->RSSetState(rasterState);


	// 뷰포트 설정
	{
		// Setup the viewport for rendering.
		viewport.Width = (float)windowWidth;
		viewport.Height = (float)windowHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
	}


	// 뷰포트 바인딩
	deviceContext->RSSetViewports(1, &viewport);


	// 카메라 설정
	float fieldOfView, screenAspect, screenNear, screenDepth;
	{
		fieldOfView = 3.141592654f / 4.0f;
		screenAspect = (float)windowWidth / (float)windowHeight;
		screenNear = 0.3f;
		screenDepth = 1000.0f;
	}


	// 메트릭스 설정
	{
		projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
		worldMatrix = XMMatrixIdentity();
		orthoMatrix = XMMatrixOrthographicLH((float)windowWidth, (float)windowHeight, screenNear, screenDepth);
	}
}

void DirectX11::Shutdown()
{
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
	}

	if (rasterState)
	{
		rasterState->Release();
		rasterState = 0;
	}

	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = 0;
	}

	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = 0;
	}

	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = 0;
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = 0;
	}

	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = 0;
	}

	if (device)
	{
		device->Release();
		device = 0;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = 0;
	}
}

void DirectX11::ClearRenderTargetView()
{
	// 회색
	float color[4];
	{
		color[0] = 0.5f;
		color[1] = 0.5f;
		color[2] = 0.5f;
		color[3] = 1.0f;
	}

	deviceContext->ClearRenderTargetView(renderTargetView, color);
}

void DirectX11::ClearDepthStencilView()
{
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectX11::Draw()
{
	// Present the back buffer to the screen since rendering is complete.
	if (isVsyncEnabled)
	{
		// Lock to screen refresh rate.
		swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		swapChain->Present(0, 0);
	}
}
