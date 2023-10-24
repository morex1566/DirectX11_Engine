#include "PCH.h"
#include "DirectX11.h"
#include "Config.h"
#include "Console.h"
#include "Window.h"


DirectX11::~DirectX11()
{
    Config::SetIsVsyncEnabled(_isVsyncEnabled);
    Config::SetRefreshRateOption(_refreshRateOption);
    Config::SetRefreshRate(_refreshRate);
    Config::SetDepthStencilBufferDesc(_depthStencilBufferDesc);
    Config::SetDepthStencilStateDesc(_depthStencilDesc);
    Config::SetDepthStencilViewDesc(_depthStencilViewDesc);
    Config::SetRasterDesc(_rasterDesc);
}


bool DirectX11::Initialize(Window& window_, bool isVsyncEnabled_, Config::ERefreshRateOption option_, unsigned int refreshRate_)
{
	HRESULT                         result;
    ComPtr<IDXGIFactory>			factory;
    ComPtr<IDXGIAdapter>			adapter;
    ComPtr<IDXGIOutput>				adapterOutput;
    DXGI_MODE_DESC*                 displayModeList;
    DXGI_ADAPTER_DESC				adapterDesc;
    unsigned int                    numModes;
    unsigned long long              stringLength;
    errno_t                         error;

    // Initialize member variable
	{
		_isVsyncEnabled = isVsyncEnabled_;
        _refreshRateOption = option_;
        _refreshRate = refreshRate_;
	}

    // Create directX interface factory.
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
    if (FAILED(result))
    {
        Console::LogError("Processing in DirectX11::Initialize(), CreateDXGIFactory() is failed.");
        return false;
    }

    // Create adapter for the gpu.
    result = factory->EnumAdapters(0, adapter.GetAddressOf());
    if (FAILED(result))
    {
        Console::LogError("Processing in DirectX11::Initialize(), EnumAdapters() is failed.");
        return false;
    }

    // Set an primary adapter for the monitor.
    result = adapter->EnumOutputs(0, adapterOutput.GetAddressOf());
    if (FAILED(result))
    {
        Console::LogError("Processing in DirectX11::Initialize(), EnumOutputs() is failed.");
        return false;
    }

    // Initialize the number of modes.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
    if (FAILED(result))
    {
        Console::LogError("Processing in DirectX11::Initialize(), GetDisplayModeList() is failed.");
        return false;
    }

    // Create a list to hold all the possible display modes for this monitor/video card combination.
    displayModeList = new DXGI_MODE_DESC[numModes];
    if (displayModeList == nullptr)
    {
        Console::LogError("Processing in DirectX11::Initialize(), new DXGI_MODE_DESC[numModes] is failed.");
        return false;
    }

    // Fill the display mode list structures.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if (FAILED(result))
    {
        Console::LogError("Processing in DirectX11::Initialize(), GetDisplayModeList() is failed.");
        return false;
    }

    // Get maximum monitor's refresh rate.
    for (unsigned int i = 0; i < numModes; i++)
    {
        if (displayModeList[i].Width == window_.GetWindowWidth())
        {
            if (displayModeList[i].Height == window_.GetWindowHeight())
            {
                unsigned int refreshRate = displayModeList[i].RefreshRate.Numerator / displayModeList[i].RefreshRate.Denominator;
                _displayRefreshRate = (refreshRate > _displayRefreshRate) ? refreshRate : _displayRefreshRate;
            }
        }
    }

    // Get the gpu adapter description.
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result))
    {
        Console::LogError("Processing in DirectX11::Initialize(), adGetDesc() is failed.");
        return false;
    }

    // Get the gpu memory size.
    _gpuMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // Get the name of gpu.
    error = wcstombs_s(&stringLength, _gpuName, 128, adapterDesc.Description, 128);
    if (error != 0)
    {
        Console::LogWarning("Processing in DirectX11::Initialize(), wcstombs_s() is failed.");
        return false;
    }

    // Release the display mode list.
    delete[] displayModeList;
    displayModeList = nullptr;

    // Setup the directx feature level.
    _supfeatureLevels[0] = D3D_FEATURE_LEVEL_10_0;
    _supfeatureLevels[1] = D3D_FEATURE_LEVEL_11_0;
    _currFeatureLevel = D3D_FEATURE_LEVEL_11_0;

    if(!CreateDeviceAndSwapChain(window_.GetHWND(), window_.GetClientWidth(), window_.GetClientHeight())) { return false; }

    if(!CreateRenderTargetView()) { return false; }

    //if(!CreateDepthStencilBuffer(window_.GetClientWidth(), window_.GetClientHeight())) { return false; }

    //if(!CreateDepthStencilState()) { return false; }

    //if(!CreateDepthStencilView()) { return false; }

    //if(!CreateRasterizerState()) { return false; }

    // Setup the flag as true.
	{
        _isEnabled = true;
        _isActivated = true;
	}

    return true;
}

bool DirectX11::CreateDeviceAndSwapChain(HWND hWnd_, unsigned int clientScreenWidth_, unsigned int clientScreenHeight_, DXGI_SWAP_CHAIN_DESC desc_)
{
    HRESULT result;

    // Setup swap chain desc.
    ZeroMemory(&_swapChainDesc, sizeof(_swapChainDesc));
    {
        _swapChainDesc.BufferCount = 1;
        _swapChainDesc.BufferDesc.Width = clientScreenWidth_;
        _swapChainDesc.BufferDesc.Height = clientScreenHeight_;
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

        // Set the refresh rate of the back buffer.
        if (_isVsyncEnabled)
        {
            _swapChainDesc.BufferDesc.RefreshRate.Numerator = _displayRefreshRate;
            _swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        }
        else
        {
            switch (_refreshRateOption)
            {
            case Config::ERefreshRateOption::Limit:
            {
                _swapChainDesc.BufferDesc.RefreshRate.Numerator = _refreshRate;
                _swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
                break;
            }
            case Config::ERefreshRateOption::DisplayBased:
            {
                _swapChainDesc.BufferDesc.RefreshRate.Numerator = _displayRefreshRate;
                _swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
                break;
            }
            case Config::ERefreshRateOption::Maximum:
            {
                _swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
                _swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
                break;
            }
            }
        }
    }

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

    return true;
}

bool DirectX11::CreateRenderTargetView(D3D11_RENDER_TARGET_VIEW_DESC desc_)
{
    ComPtr<ID3D11Texture2D>         backBuffer;
    HRESULT                         result;

    // Get the back buffer from swap chain.
    result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
    if (FAILED(result))
    {
        std::cout << "swapChain->GetBuffer() is failed.";
        return false;
    }

    // Create the render target view.
    result = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _interfaceRenderTargetView.GetAddressOf());
    if (FAILED(result))
    {
        std::cout << "device->CreateRenderTargetView() is failed.";
        return false;
    }

    // Create the render target view.
    result = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _modelRenderTargetView.GetAddressOf());
    if (FAILED(result))
    {
        std::cout << "device->CreateRenderTargetView() is failed.";
        return false;
    }

    return true;
}

bool DirectX11::CreateDepthStencilBuffer(unsigned int clientScreenWidth_, unsigned int clientScreenHeight, D3D11_TEXTURE2D_DESC desc_)
{
    HRESULT result;

    if (_depthStencilBuffer)
    {
	    _depthStencilBuffer->Release();
    }

    // Initialize the depth buffer desc.
    ZeroMemory(&_depthStencilBufferDesc, sizeof(_depthStencilBufferDesc));
    {
        _depthStencilBufferDesc.Width = clientScreenWidth_;
        _depthStencilBufferDesc.Height = clientScreenHeight;
        _depthStencilBufferDesc.MipLevels = 1;
        _depthStencilBufferDesc.ArraySize = 1;
        _depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        _depthStencilBufferDesc.SampleDesc.Count = 1;
        _depthStencilBufferDesc.SampleDesc.Quality = 0;
        _depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        _depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        _depthStencilBufferDesc.CPUAccessFlags = 0;
        _depthStencilBufferDesc.MiscFlags = 0;
    }

    // Create the depth buffer.
    result = _device->CreateTexture2D(&_depthStencilBufferDesc, nullptr, _depthStencilBuffer.GetAddressOf());
    if (FAILED(result))
    {
        Console::LogError("CreateTexture2D() is failed.");
        return false;
    }

    return true;
}

bool DirectX11::CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC desc_)
{
    HRESULT result;

    if (_depthStencilState) { _depthStencilState->Release(); }

    // Initialize the depth stencil state desc.
    ZeroMemory(&_depthStencilDesc, sizeof(_depthStencilDesc));
    {
        _depthStencilDesc.DepthEnable = true;
        _depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        _depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

        _depthStencilDesc.StencilEnable = true;
        _depthStencilDesc.StencilReadMask = 0xFF;
        _depthStencilDesc.StencilWriteMask = 0xFF;

        _depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        _depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        _depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        _depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        _depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        _depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        _depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        _depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    }

    // Create the depth stencil state.
    result = _device->CreateDepthStencilState(&_depthStencilDesc, _depthStencilState.GetAddressOf());
    if (FAILED(result))
    {
        Console::LogError("CreateDepthStencilState() is failed.");
        return false;
    }

    return true;
}

bool DirectX11::CreateDepthStencilView(D3D11_DEPTH_STENCIL_VIEW_DESC desc_)
{
    HRESULT result;

    if (_depthStencilView) { _depthStencilView->Release(); }

    // Initialize the depth stencil view.
    ZeroMemory(&_depthStencilViewDesc, sizeof(_depthStencilViewDesc));
    {
        _depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        _depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        _depthStencilViewDesc.Texture2D.MipSlice = 0;
    }

    // Create the depth stencil view.
    result = _device->CreateDepthStencilView(_depthStencilBuffer.Get(), &_depthStencilViewDesc, _depthStencilView.GetAddressOf());
    if (FAILED(result))
    {
        Console::LogError("CreateDepthStencilView() is failed.");
        return false;
    }

    return true;
}

bool DirectX11::CreateRasterizerState(D3D11_RASTERIZER_DESC desc_)
{
    HRESULT result;

    if (_rasterState) { _rasterState->Release(); }

    // Initialize the rasterizer desc.
    ZeroMemory(&_rasterDesc, sizeof(_rasterDesc));
    {
        _rasterDesc.AntialiasedLineEnable = false;
        _rasterDesc.CullMode = D3D11_CULL_BACK;
        _rasterDesc.DepthBias = 0;
        _rasterDesc.DepthBiasClamp = 0.0f;
        _rasterDesc.DepthClipEnable = true;
        _rasterDesc.FillMode = D3D11_FILL_SOLID;
        _rasterDesc.FrontCounterClockwise = false;
        _rasterDesc.MultisampleEnable = false;
        _rasterDesc.ScissorEnable = false;
        _rasterDesc.SlopeScaledDepthBias = 0.0f;
    }

    // Create the rasterizer state.
    result = _device->CreateRasterizerState(&_rasterDesc, _rasterState.GetAddressOf());
    if (FAILED(result))
    {
        Console::LogError("CreateRasterizerState() is failed.");
        return false;
    }

    _deviceContext->RSSetState(_rasterState.Get());

    return true;
}

void DirectX11::WaitForRefreshRate()
{
    _isVsyncEnabled ? _swapChain->Present(1, 0) : _swapChain->Present(0, 0);
}

void DirectX11::BindRenderTarget(DirectX11::ERenderTargetViewType type_)
{
    _deviceContext->OMSetRenderTargets(1, _interfaceRenderTargetView.GetAddressOf(), nullptr);
}

void DirectX11::ResizeRenderTargetView(unsigned clientWidth_, unsigned clientHeight_)
{
    ComPtr<ID3D11Texture2D>         backBuffer;
    HRESULT                         result;

    if (_interfaceRenderTargetView) { _interfaceRenderTargetView->Release(); }
    if (_modelRenderTargetView) { _modelRenderTargetView->Release(); }

    // Resize the buffer.
    _swapChain->ResizeBuffers(0, clientWidth_, clientHeight_, DXGI_FORMAT_UNKNOWN, 0);

    // Get the back buffer from swap chain.
    result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
    if (FAILED(result))
    {
        std::cout << "In ResizeRenderTargetView(), swapChain->GetBuffer() is failed.";
        return;
    }

    // Create the render target view.
    result = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _interfaceRenderTargetView.GetAddressOf());
    if (FAILED(result))
    {
        std::cout << "In ResizeRenderTargetView(), device->CreateRenderTargetView() is failed.";
        return;
    }

    // Create the render target view.
    result = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _modelRenderTargetView.GetAddressOf());
    if (FAILED(result))
    {
        std::cout << "In ResizeRenderTargetView(), device->CreateRenderTargetView() is failed.";
        return;
    }
}

void DirectX11::ClearRenderTargetView(DirectX11::ERenderTargetViewType type_, float clearColor_[4])
{
    // Default color.
    if (!clearColor_)
    {
        constexpr float cleanColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
        _deviceContext->ClearRenderTargetView(_interfaceRenderTargetView.Get(), cleanColor);
    }
    else
    {
        _deviceContext->ClearRenderTargetView(_interfaceRenderTargetView.Get(), clearColor_);
    }
}



bool DirectX11::GetIsVsyncEnabled()
{
    return _isVsyncEnabled;
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
	return _interfaceRenderTargetView;
}



void DirectX11::SetIsVsyncEnabled(bool toggle_)
{
    _isVsyncEnabled = toggle_;
}

void DirectX11::SetRefreshRate(unsigned int refreshRate_)
{
    _refreshRate = refreshRate_;
}

void DirectX11::SetRefreshRateOption(Config::ERefreshRateOption option_)
{
    _refreshRateOption = option_;
}
