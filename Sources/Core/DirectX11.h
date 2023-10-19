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

	bool Initialize(HWND hWnd_);

	ComPtr<ID3D11RenderTargetView>		CreateRenderTargetView();
	void								ZeroRenderTargetView();

	ComPtr<ID3D11Device>				GetDevice() const;
	ComPtr<ID3D11DeviceContext>			GetDeviceContext() const;
	ComPtr<ID3D11RenderTargetView>		GetRenderTargetView() const;

	void								SyncWithVsync(bool isVsyncEnabled_);
	static void							OMSetRenderTarget();
	static void							ResizeRenderTargetView(unsigned int width_, unsigned int height_);

private:
	DXGI_SWAP_CHAIN_DESC				_swapChainDesc;
	D3D11_TEXTURE2D_DESC				_depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC			_depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC		_depthStencilViewDesc;
	D3D11_RASTERIZER_DESC				_rasterDesc;
	D3D_FEATURE_LEVEL					_currFeatureLevel;
	D3D_FEATURE_LEVEL					_supfeatureLevels[2];

	static ComPtr<IDXGISwapChain>				_swapChain;
	static ComPtr<ID3D11Device>					_device;
	static ComPtr<ID3D11DeviceContext>			_deviceContext;
	static ComPtr<ID3D11RenderTargetView>		_renderTargetView;
	static ComPtr<ID3D11Texture2D>				_depthStencilBuffer;
	static ComPtr<ID3D11DepthStencilState>		_depthStencilState;
	static ComPtr<ID3D11DepthStencilView>		_depthStencilView;
	static ComPtr<ID3D11RasterizerState>		_rasterState;
};

