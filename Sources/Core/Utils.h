#pragma once

#define FROM_SOLUTION_PATH_TO(path_) std::string(SOLUTION_DIR) + path_
#define RADIAN_CONST 0.0174532925f

template <typename Derived, typename Base> using IsNotBaseOf	= std::enable_if_t<!std::is_base_of<Base, Derived>::value, Derived>;
template <typename Derived, typename Base> using IsBaseOf		= std::enable_if_t<std::is_base_of<Base, Derived>::value, Derived>;

class Utils
{
public:
	inline static std::wstring ToWString(const std::string& param_)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(param_);
	}

	inline static std::wstring ToWString(const char* param_)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(param_);
	}

	inline static std::string ToString(const std::wstring& param_)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(param_);
	}

	inline static std::string ToString(const wchar_t* param_)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(param_);
	}

	inline static int GetMonitorResX()
	{
		return GetSystemMetrics(SM_CXSCREEN);
	}

	inline static int GetMonitorResY()
	{
		return GetSystemMetrics(SM_CYSCREEN);
	}
};

class Config
{
public:
	class Setting
	{
	public:
		class Window
		{
		public:
			inline static int GetWidth()
			{
				std::ifstream config;
				config.open(FROM_SOLUTION_PATH_TO("config.json"), std::ios_base::binary);

				Json::Value root;
				config >> root;

				config.close();

				if (root["Engine"]["Setting"]["Window"]["IsFullScreenEnabled"].asBool() == true)
				{
					return GetSystemMetrics(SM_CXSCREEN);
				}
				else
				{
					return root["Engine"]["Setting"]["Window"]["Width"].asInt();
				}
			}

			inline static int GetHeight()
			{
				std::ifstream config;
				config.open(FROM_SOLUTION_PATH_TO("config.json"), std::ios_base::binary);

				Json::Value root;
				config >> root;

				config.close();

				if (root["Engine"]["Setting"]["Window"]["IsFullScreenEnabled"].asBool() == true)
				{
					return GetSystemMetrics(SM_CYSCREEN);
				}
				else
				{
					return root["Engine"]["Setting"]["Window"]["Height"].asInt();
				}
			}

			inline static bool GetFullScreenEnabled()
			{
				std::ifstream config;
				config.open(FROM_SOLUTION_PATH_TO("config.json"), std::ios_base::binary);

				Json::Value root;
				config >> root;

				config.close();

				return root["Engine"]["Setting"]["Window"]["IsFullScreenEnabled"].asBool();
			}

			inline static bool GetVsyncEnabled()
			{
				std::ifstream config;
				config.open(FROM_SOLUTION_PATH_TO("config.json"), std::ios_base::binary);

				Json::Value root;
				config >> root;

				config.close();

				return root["Engine"]["Setting"]["Window"]["IsVsyncEnabled"].asBool();
			}
		};
	};
};