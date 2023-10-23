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
    Config::SetDepthBufferDesc(_depthBufferDesc);
    Config::SetDepthStencilDesc(_depthStencilDesc);
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

	// Setup swap chain desc.
	ZeroMemory(&_swapChainDesc, sizeof(_swapChainDesc));
	{
        _swapChainDesc.BufferCount = 1;
        _swapChainDesc.BufferDesc.Width = window_.GetClientWidth();
        _swapChainDesc.BufferDesc.Height = window_.GetClientHeight();
        _swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        _swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        _swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        _swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        _swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        _swapChainDesc.OutputWindow = window_.GetHWND();
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

void DirectX11::WaitForRefreshRate()
{
    _isVsyncEnabled ? _swapChain->Present(1, 0) : _swapChain->Present(0, 0);
}

void DirectX11::BindRenderTarget()
{
    _deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
}

void DirectX11::ResizeRenderTargetView(unsigned clientWidth_, unsigned clientHeight_)
{
    ComPtr<ID3D11Texture2D>         backBuffer;
    HRESULT                         result;

    if (_renderTargetView) { _renderTargetView->Release(); }

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
    result = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
    if (FAILED(result))
    {
        std::cout << "In ResizeRenderTargetView(), device->CreateRenderTargetView() is failed.";
        return;
    }
}

void DirectX11::ClearRenderTargetView(float clearColor_[4])
{
    // Default color.
    if (!clearColor_)
    {
        constexpr float cleanColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
        _deviceContext->ClearRenderTargetView(_renderTargetView.Get(), cleanColor);
    }
    else
    {
        _deviceContext->ClearRenderTargetView(_renderTargetView.Get(), clearColor_);
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
	return _renderTargetView;
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
