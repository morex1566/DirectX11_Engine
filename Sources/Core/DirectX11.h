#pragma once

class Window;

class DirectX11 : public Object
{
public:
	DirectX11()												= default;
	DirectX11(const DirectX11&)								= default;
	DirectX11& operator=(const DirectX11&)					= default;
	DirectX11(DirectX11&&) noexcept							= default;
	DirectX11& operator=(DirectX11&&) noexcept				= default;
	~DirectX11();

	bool Initialize(Window& window_, bool isVsyncEnabled_, Config::ERefreshRateOption option_, unsigned int refreshRate_);

	ComPtr<ID3D11RenderTargetView>				CreateRenderTargetView();
	void										WaitForRefreshRate();
	void										BindRenderTarget();
	void										ResizeRenderTargetView(unsigned int clientWidth_ = 0, unsigned int clientHeight_ = 0);
	void										ClearRenderTargetView(float clearColor_[4] = nullptr);

	bool										GetIsVsyncEnabled();
	ComPtr<ID3D11Device>						GetDevice() const;
	ComPtr<ID3D11DeviceContext>					GetDeviceContext() const;
	ComPtr<ID3D11RenderTargetView>				GetRenderTargetView() const;

	void										SetIsVsyncEnabled(bool toggle_);
	void										SetRefreshRate(unsigned int refreshRate_);
	void										SetRefreshRateOption(Config::ERefreshRateOption option_);


private:
	bool										_isVsyncEnabled;
	ComPtr<IDXGISwapChain>						_swapChain;
	ComPtr<ID3D11Device>						_device;
	ComPtr<ID3D11DeviceContext>					_deviceContext;
	ComPtr<ID3D11RenderTargetView>				_renderTargetView;
	ComPtr<ID3D11Texture2D>						_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState>				_depthStencilState;
	ComPtr<ID3D11DepthStencilView>				_depthStencilView;
	ComPtr<ID3D11RasterizerState>				_rasterState;
	unsigned int								_refreshRate;
	Config::ERefreshRateOption					_refreshRateOption;

	DXGI_SWAP_CHAIN_DESC						_swapChainDesc;
	D3D11_TEXTURE2D_DESC						_depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC					_depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC				_depthStencilViewDesc;
	D3D11_RASTERIZER_DESC						_rasterDesc;
	D3D_FEATURE_LEVEL							_currFeatureLevel;
	D3D_FEATURE_LEVEL							_supfeatureLevels[2];

	// TODO : Move this variables to camera or main system.
	D3D11_VIEWPORT								_viewport;
	XMMATRIX									_projectionMatrix;
	XMMATRIX									_worldMatrix;
	XMMATRIX									_orthoMatrix;

	unsigned int								_displayRefreshRate;
	char										_gpuName[128];
	int											_gpuMemory;
};

