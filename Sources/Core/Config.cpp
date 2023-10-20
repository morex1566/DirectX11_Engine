#include "PCH.h"
#include "Config.h"

std::string				Config::_configFilePath_;
json					Config::_config;

Config::~Config()
{
	std::ofstream file(_configFilePath_);
	file << std::setw(4) << _config << std::endl;
}

bool Config::Initialize(const std::string& configFilePath_)
{
	// Initialize member variable
	{
		_configFilePath_ = configFilePath_;
	}

	std::ifstream file(_configFilePath_);

	// Load engine configuration.
	_config = json::parse(file);

	if (_config.empty())
	{
		return false;
	}

	return true;
}

int Config::GetWindowWidth()
{
	return _config["Engine"]["Setting"]["Window"]["WindowWidth"];
}

int Config::GetWindowHeight()
{
	return _config["Engine"]["Setting"]["Window"]["WindowHeight"];
}

bool Config::GetIsVsyncEnabled()
{
	return _config["Engine"]["Setting"]["Window"]["IsVsyncEnabled"];
}

bool Config::GetIsFullScreenEnabled()
{
	return _config["Engine"]["Setting"]["Window"]["IsFullScreenEnabled"];
}

DXGI_SWAP_CHAIN_DESC Config::GetSwapChainDesc()
{
	return DXGI_SWAP_CHAIN_DESC();
}

D3D11_TEXTURE2D_DESC Config::GetDepthBufferDesc()
{
	return D3D11_TEXTURE2D_DESC();
}

D3D11_DEPTH_STENCIL_DESC Config::GetDepthStencilDesc()
{
	return D3D11_DEPTH_STENCIL_DESC();
}

D3D11_DEPTH_STENCIL_VIEW_DESC Config::GetDepthStencilViewDesc()
{
	return D3D11_DEPTH_STENCIL_VIEW_DESC();
}

D3D11_RASTERIZER_DESC Config::GetRasterDesc()
{
	return D3D11_RASTERIZER_DESC();
}

void Config::SetWindowWidth(unsigned int width_)
{
	_config["Engine"]["Setting"]["Window"]["WindowWidth"] = width_;
}

void Config::SetWindowHeight(unsigned int height_)
{
	_config["Engine"]["Setting"]["Window"]["WindowHeight"] = height_;
}

void Config::SetIsVsyncEnabled(bool toggle_)
{
	 _config["Engine"]["Setting"]["Window"]["IsVsyncEnabled"] = toggle_;
}

void Config::SetIsFullScreenEnabled(bool toggle_)
{
	_config["Engine"]["Setting"]["Window"]["IsFullScreenEnabled"] = toggle_;
}

void Config::SetSwapChainDesc(DXGI_SWAP_CHAIN_DESC swapChainDesc_)
{
}

void Config::SetDepthBufferDesc(D3D11_TEXTURE2D_DESC depthBufferDesc_)
{
}

void Config::SetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC depthStencilDesc_)
{
}

void Config::SetDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc_)
{
}

void Config::SetRasterDesc(D3D11_RASTERIZER_DESC rasterizerDesc_)
{
}