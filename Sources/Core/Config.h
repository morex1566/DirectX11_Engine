// ReSharper disable CppClangTidyBugproneReservedIdentifier
#pragma once

class Config
{
public:
	enum class ERefreshRateOption
	{
		Limit,
		DisplayBased,
		Maximum
	};
	inline static std::map<std::string, ERefreshRateOption> _RefreshRateOption =
	{
		{ "Limit", ERefreshRateOption::Limit },
		{ "DisplayBased", ERefreshRateOption::DisplayBased },
		{ "Maximum", ERefreshRateOption::Maximum }
	};

	enum class EWindowSizePreset
	{
		
	};

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
	static unsigned int						GetWindowStartPosX();
	static unsigned int						GetWindowStartPosY();
	static bool								GetIsVsyncEnabled();
	static bool								GetIsFullScreenEnabled();
	static DXGI_SWAP_CHAIN_DESC				GetSwapChainDesc();
	static D3D11_TEXTURE2D_DESC				GetDepthBufferDesc();
	static D3D11_DEPTH_STENCIL_DESC			GetDepthStencilDesc();
	static D3D11_DEPTH_STENCIL_VIEW_DESC	GetDepthStencilViewDesc();
	static D3D11_RASTERIZER_DESC			GetRasterDesc();
	static ERefreshRateOption				GetRefreshRateOption();
	static unsigned int						GetRefreshRate();

	static void								SetWindowWidth(unsigned int width_);
	static void								SetWindowHeight(unsigned int height_);
	static void								SetWindowStartPosX(unsigned int posX_);
	static void								SetWindowStartPosY(unsigned int posY_);
	static void								SetIsVsyncEnabled(bool toggle_);
	static void								SetIsFullScreenEnabled(bool toggle_);
	static void								SetSwapChainDesc(DXGI_SWAP_CHAIN_DESC swapChainDesc_);
	static void								SetDepthStencilBufferDesc(D3D11_TEXTURE2D_DESC depthBufferDesc_);
	static void								SetDepthStencilStateDesc(D3D11_DEPTH_STENCIL_DESC depthStencilDesc_);
	static void								SetDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc_);
	static void								SetRasterDesc(D3D11_RASTERIZER_DESC rasterizerDesc_);
	static void								SetRefreshRateOption(ERefreshRateOption refreshRateOption_);
	static void								SetRefreshRate(unsigned int refreshRate_);

private:
	static std::string			_configFilePath;
	static json					_config;
};
