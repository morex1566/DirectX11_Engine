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

	ComPtr<ID3D11RenderTargetView>				CreateRenderTargetView();
	void										ZeroRenderTargetView();
	void										WaitForRefreshRate();
	static void									OMSetRenderTarget();
	static void									ResizeRenderTargetView(unsigned int width_, unsigned int height_);

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
	static Config::ERefreshRateOption			_refreshRateOption;

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

