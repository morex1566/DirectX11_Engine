#pragma once

#include <fstream>

#include "json/json.hpp"

using Json = nlohmann::json;

class Config
{
public:
	Config(const std::wstring& path);
	Config(const Config&)						= delete;
	Config& operator=(const Config&)			= delete;
	Config(Config&&) noexcept					= delete;
	Config& operator=(Config&&) noexcept		= delete;
	~Config()  									= default;


public:
	void Init();
	FORCEINLINE static Json& GetConfig() { return config; }


private:
	static Json				config;
	std::wstring			path;


};

