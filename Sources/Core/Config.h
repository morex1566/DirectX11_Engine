#pragma once

class Config
{
public:
	Config()									= default;
	Config(const Config&)						= default;
	Config& operator=(const Config&)			= default;
	Config(Config&&) noexcept					= default;
	Config& operator=(Config&&) noexcept		= default;
	~Config();

	bool									Initialize(const std::string& configFilePath_);

	static int								GetWindowWidth();
	static int								GetWindowHeight();
	static bool								GetIsVsyncEnabled();
	static bool								GetIsFullScreenEnabled();
	static DXGI_SWAP_CHAIN_DESC				GetSwapChainDesc();
	static D3D11_TEXTURE2D_DESC				GetDepthBufferDesc();
	static D3D11_DEPTH_STENCIL_DESC			GetDepthStencilDesc();
	static D3D11_DEPTH_STENCIL_VIEW_DESC	GetDepthStencilViewDesc();
	static D3D11_RASTERIZER_DESC			GetRasterDesc();

	static void								SetWindowWidth(unsigned int width_);
	static void								SetWindowHeight(unsigned int height_);
	static void								SetIsVsyncEnabled(bool toggle_);
	static void								SetIsFullScreenEnabled(bool toggle_);
	static void								SetSwapChainDesc(DXGI_SWAP_CHAIN_DESC swapChainDesc_);
	static void								SetDepthBufferDesc(D3D11_TEXTURE2D_DESC depthBufferDesc_);
	static void								SetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC depthStencilDesc_);
	static void								SetDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc_);
	static void								SetRasterDesc(D3D11_RASTERIZER_DESC rasterizerDesc_);

private:
	static std::string			_configFilePath_;
	static json					_config;
};
