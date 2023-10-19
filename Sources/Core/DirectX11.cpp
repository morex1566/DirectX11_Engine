#include "PCH.h"
#include "DirectX11.h"

ComPtr<IDXGISwapChain> DirectX11::_swapChain;
ComPtr<ID3D11Device> DirectX11::_device;
ComPtr<ID3D11DeviceContext> DirectX11::_deviceContext;
ComPtr<ID3D11RenderTargetView> DirectX11::_renderTargetView;
ComPtr<ID3D11Texture2D> DirectX11::_depthStencilBuffer;
ComPtr<ID3D11DepthStencilState> DirectX11::_depthStencilState;
ComPtr<ID3D11DepthStencilView> DirectX11::_depthStencilView;
ComPtr<ID3D11RasterizerState> DirectX11::_rasterState;

DirectX11::~DirectX11()
{
}

bool DirectX11::Initialize(HWND hWnd_)
{
	HRESULT result;

	// Setup swap chain desc.
	ZeroMemory(&_swapChainDesc, sizeof(_swapChainDesc));
	{
        _swapChainDesc.BufferCount = 2;
        _swapChainDesc.BufferDesc.Width = 0;
        _swapChainDesc.BufferDesc.Height = 0;
        _swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        _swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        _swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        _swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        _swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        _swapChainDesc.OutputWindow = hWnd_;
        _swapChainDesc.SampleDesc.Count = 1;
        _swapChainDesc.SampleDesc.Quality = 0;
        _swapChainDesc.Windowed = TRUE;
        _swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

    // Setup the directx feature level.
    _supfeatureLevels[0] = D3D_FEATURE_LEVEL_10_0;
    _supfeatureLevels[1] = D3D_FEATURE_LEVEL_11_0;
    _currFeatureLevel = D3D_FEATURE_LEVEL_11_0;

    // Create the device and device context.
    result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, _supfeatureLevels, 2, D3D11_SDK_VERSION, &_swapChainDesc, _swapChain.GetAddressOf(), _device.GetAddressOf(), &_currFeatureLevel, _deviceContext.GetAddressOf());
    if (result == DXGI_ERROR_UNSUPPORTED)
    {
        // If GPU not support hardware acceleration.
        result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, _supfeatureLevels, 2, D3D11_SDK_VERSION, &_swapChainDesc, _swapChain.GetAddressOf(), _device.GetAddressOf(), &_currFeatureLevel, _deviceContext.GetAddressOf());
    }
	if (FAILED(result))
    {
	    std::cout << "D3D11CreateDeviceAndSwapChain() is failed.";
        return false;
    }

    if(!CreateRenderTargetView()) { return false; }

    // Setup the flag as true.
	{
        _isEnabled = true;
        _isActivated = true;
	}

    return true;
}

ComPtr<ID3D11RenderTargetView> DirectX11::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D>         backBuffer;
	HRESULT                         result;

    // Get the back buffer from swap chain.
    result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
	if (FAILED(result))
	{
		std::cout << "swapChain->GetBuffer() is failed.";
		return nullptr;
	}

    // Create the render target view.
    result = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
    if (FAILED(result))
    {
        std::cout << "device->CreateRenderTargetView() is failed.";
        return nullptr;
    }

    return _renderTargetView;
}

void DirectX11::ZeroRenderTargetView()
{
	if (_renderTargetView)
	{
		_renderTargetView->Release();
        _renderTargetView = nullptr;
	}
}

ComPtr<ID3D11Device> DirectX11::GetDevice() const
{
    return _device;
}

ComPtr<ID3D11DeviceContext> DirectX11::GetDeviceContext() const
{
    return _deviceContext;
}

ComPtr<ID3D11RenderTargetView> DirectX11::GetRenderTargetView() const
{
    return _renderTargetView;
}

void DirectX11::SyncWithVsync(bool isVsyncEnabled_)
{
    isVsyncEnabled_? _swapChain->Present(1, 0) : _swapChain->Present(0, 0);
}

void DirectX11::OMSetRenderTarget()
{
	constexpr float cleanColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
    _deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
    _deviceContext->ClearRenderTargetView(_renderTargetView.Get(), cleanColor);
}

void DirectX11::ResizeRenderTargetView(unsigned width_, unsigned height_)
{
    ComPtr<ID3D11Texture2D>         backBuffer;
    HRESULT                         result;

    if (_renderTargetView) { _renderTargetView->Release(); }

    // Resize the buffer.
    _swapChain->ResizeBuffers(0, width_, height_, DXGI_FORMAT_UNKNOWN, 0);

    // Get the back buffer from swap chain.
    result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
    if (FAILED(result))
    {
        std::cout << "In ResizeRenderTargetView(), swapChain->GetBuffer() is failed.";
        return;
    }

    // Create the render target view.
    result = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
    if (FAILED(result))
    {
        std::cout << "In ResizeRenderTargetView(), device->CreateRenderTargetView() is failed.";
        return;
    }
}
