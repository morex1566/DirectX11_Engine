#pragma once

class DirectX11 : public Object
{
public:
	DirectX11()												= default;
	DirectX11(const DirectX11&)								= default;
	DirectX11& operator=(const DirectX11&)					= default;
	DirectX11(DirectX11&&) noexcept							= default;
	DirectX11& operator=(DirectX11&&) noexcept				= default;
	~DirectX11();

	bool Initialize(HWND hWnd_,
					unsigned int clientScreenWidth_,
					unsigned int clientScreenHeight_,
					bool isVsyncEnabled_,
					Config::ERefreshRateOption option_,
					unsigned int refreshRate_,
					bool isFullScreenEnabled_);

	static	bool								CreateDeviceAndSwapChain(HWND hWnd_, unsigned int clientScreenWidth_, unsigned int clientScreenHeight_);
	static	ComPtr<ID3D11RenderTargetView>		CreateRenderTargetView();
	static	ComPtr<ID3D11Texture2D>				CreateDepthStencilBuffer(unsigned int clientScreenWidth_, unsigned int clientScreenHeight);
	static	ComPtr<ID3D11DepthStencilState>		CreateDepthStencilState();
	static	ComPtr<ID3D11DepthStencilView>		CreateDepthStencilView();
	static ComPtr<ID3D11RasterizerState>		CreateRasterizerState();

	void										ReleaseSwapChain();
	void										ReleaseRenderTargetView();
	void										ReleaseDepthStencilBuffer();
	void										ReleaseDepthStencilState();
	void										ReleaseDepthStencilView();
	void										ReleaseRasterizerState();

	void										WaitForRefreshRate();
	static void									BindState();
	static void									BindRenderTargets();
	static void									ResizeRenderTargets(HWND hWnd_, unsigned int width_, unsigned int height_);

	static bool									GetIsVsyncEnabled();
	ComPtr<ID3D11Device>						GetDevice() const;
	ComPtr<ID3D11DeviceContext>					GetDeviceContext() const;
	ComPtr<ID3D11RenderTargetView>				GetRenderTargetView() const;

	static void									SetIsVsyncEnabled(bool toggle_);
	static void									SetRefreshRate(unsigned int refreshRate_);
	static void									SetRefreshRateOption(Config::ERefreshRateOption option_);


private:
	static bool									_isVsyncEnabled;
	static ComPtr<IDXGISwapChain>				_swapChain;
	static ComPtr<ID3D11Device>					_device;
	static ComPtr<ID3D11DeviceContext>			_deviceContext;
	static ComPtr<ID3D11RenderTargetView>		_renderTargetView;
	static ComPtr<ID3D11Texture2D>				_depthStencilBuffer;
	static ComPtr<ID3D11DepthStencilState>		_depthStencilState;
	static ComPtr<ID3D11DepthStencilView>		_depthStencilView;
	static ComPtr<ID3D11RasterizerState>		_rasterState;
	static unsigned int							_refreshRate;
	static unsigned int							_displayRefreshRate;
	static Config::ERefreshRateOption			_refreshRateOption;

	static DXGI_SWAP_CHAIN_DESC					_swapChainDesc;
	static D3D11_TEXTURE2D_DESC					_depthStencilBufferDesc;
	static D3D11_DEPTH_STENCIL_DESC				_depthStencilDesc;
	static D3D11_DEPTH_STENCIL_VIEW_DESC		_depthStencilViewDesc;
	static D3D11_RASTERIZER_DESC				_rasterDesc;
	static D3D_FEATURE_LEVEL					_currFeatureLevel;
	static D3D_FEATURE_LEVEL					_supfeatureLevels[2];

	// TODO : Move this variables to camera or main system.
	D3D11_VIEWPORT								_viewport;
	float										_fov;
	float										_screenAspect;
	float										_screenNear;
	float										_screenDepth;
	XMMATRIX									_projectionMatrix;
	XMMATRIX									_worldMatrix;
	XMMATRIX									_orthoMatrix;

	char										_gpuName[128];
	int											_gpuMemory;
};

