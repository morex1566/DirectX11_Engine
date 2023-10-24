#pragma once

class Window;

class DirectX11 : public Object
{
public:
	enum class ERenderTargetViewType
	{
		Interface,
		Model
	};

public:
	DirectX11()												= default;
	DirectX11(const DirectX11&)								= default;
	DirectX11& operator=(const DirectX11&)					= default;
	DirectX11(DirectX11&&) noexcept							= default;
	DirectX11& operator=(DirectX11&&) noexcept				= default;
	~DirectX11();

	bool Initialize(Window& window_, bool isVsyncEnabled_, Config::ERefreshRateOption option_, unsigned int refreshRate_);

	bool										CreateDeviceAndSwapChain(HWND hWnd_, unsigned int clientScreenWidth_, unsigned int clientScreenHeight_, DXGI_SWAP_CHAIN_DESC desc_ = {});
	bool										CreateRenderTargetView(D3D11_RENDER_TARGET_VIEW_DESC desc_ = {});
	bool										CreateDepthStencilBuffer(unsigned int clientScreenWidth_, unsigned int clientScreenHeight, D3D11_TEXTURE2D_DESC desc_ = {});
	bool										CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC desc_ = {});
	bool										CreateDepthStencilView(D3D11_DEPTH_STENCIL_VIEW_DESC desc_ = {});
	bool										CreateRasterizerState(D3D11_RASTERIZER_DESC desc_ = {});
	void										WaitForRefreshRate();
	void										BindRenderTarget(ERenderTargetViewType type_);
	void										ResizeRenderTargetView(unsigned int clientWidth_ = 0, unsigned int clientHeight_ = 0);
	void										ClearRenderTargetView(ERenderTargetViewType type_, float clearColor_[4] = nullptr);

	bool										GetIsVsyncEnabled();
	ComPtr<ID3D11Device>						GetDevice() const;
	ComPtr<ID3D11DeviceContext>					GetDeviceContext() const;

	void										SetIsVsyncEnabled(bool toggle_);
	void										SetRefreshRate(unsigned int refreshRate_);
	void										SetRefreshRateOption(Config::ERefreshRateOption option_);

private:
	bool																_isVsyncEnabled;
	ComPtr<IDXGISwapChain>												_swapChain;
	ComPtr<ID3D11Device>												_device;
	ComPtr<ID3D11DeviceContext>											_deviceContext;
	std::map<ERenderTargetViewType, ComPtr<ID3D11RenderTargetView>>		_renderTargetViews;
	ComPtr<ID3D11Texture2D>												_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState>										_depthStencilState;
	ComPtr<ID3D11DepthStencilView>										_depthStencilView;
	ComPtr<ID3D11RasterizerState>										_rasterState;
	unsigned int														_refreshRate;
	Config::ERefreshRateOption											_refreshRateOption;

	DXGI_SWAP_CHAIN_DESC												_swapChainDesc;
	D3D11_TEXTURE2D_DESC												_depthStencilBufferDesc;
	D3D11_DEPTH_STENCIL_DESC											_depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC										_depthStencilViewDesc;
	D3D11_RASTERIZER_DESC												_rasterDesc;
	D3D_FEATURE_LEVEL													_currFeatureLevel;
	D3D_FEATURE_LEVEL													_supfeatureLevels[2];

	// TODO : Move this variables to camera or main system.
	D3D11_VIEWPORT														_viewport;
	XMMATRIX															_projectionMatrix;
	XMMATRIX															_worldMatrix;
	XMMATRIX															_orthoMatrix;

	unsigned int														_displayRefreshRate;
	char																_gpuName[128];
	int																	_gpuMemory;
};

