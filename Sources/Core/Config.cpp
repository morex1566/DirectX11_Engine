// ReSharper disable CppClangTidyBugproneReservedIdentifier
#include "PCH.h"
#include "Config.h"

std::string											Config::_configFilePath;
json												Config::_config;

Config::~Config()
{
	std::ofstream file(_configFilePath);
	file << std::setw(4) << _config << std::endl;
}

bool Config::Initialize(const std::string& configFilePath_)
{
	// Initialize member variable
	{
		_configFilePath = configFilePath_;
	}

	std::ifstream file(_configFilePath);

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

unsigned Config::GetWindowStartPosX()
{
	return _config["Engine"]["Setting"]["Window"]["WindowStartPosX"];
}

unsigned Config::GetWindowStartPosY()
{
	return _config["Engine"]["Setting"]["Window"]["WindowStartPosY"];
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

Config::ERefreshRateOption Config::GetRefreshRateOption()
{
	return _RefreshRateOption[_config["Engine"]["Setting"]["Window"]["RefreshRateOption"]];
}

unsigned int Config::GetRefreshRate()
{
	return _config["Engine"]["Setting"]["Window"]["RefreshRate"];
}


void Config::SetWindowWidth(unsigned int width_)
{
	_config["Engine"]["Setting"]["Window"]["WindowWidth"] = width_;
}

void Config::SetWindowHeight(unsigned int height_)
{
	_config["Engine"]["Setting"]["Window"]["WindowHeight"] = height_;
}

void Config::SetWindowStartPosX(unsigned int posX_)
{
	_config["Engine"]["Setting"]["Window"]["WindowStartPosX"] = posX_;
}

void Config::SetWindowStartPosY(unsigned int posY_)
{
	_config["Engine"]["Setting"]["Window"]["WindowStartPosY"] = posY_;
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

void Config::SetRefreshRateOption(ERefreshRateOption refreshRateOption_)
{
	for (auto& option : _RefreshRateOption)
	{
		if (option.second == refreshRateOption_)
		{
			_config["Engine"]["Setting"]["Window"]["RefreshRateOption"] = option.first;
		}
	}
}

void Config::SetRefreshRate(unsigned int refreshRate_)
{
	_config["Engine"]["Setting"]["Window"]["RefreshRate"] = refreshRate_;
}