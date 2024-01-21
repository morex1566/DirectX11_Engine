#include "PCH.h"
#include "Config.h"

Json Config::config;

Config::Config(const std::wstring& path)
	: path(path)
{
}

void Config::Init()
{
	std::ifstream f(path);
	config = Json::parse(f);
}

void Config::Shutdown()
{
}
