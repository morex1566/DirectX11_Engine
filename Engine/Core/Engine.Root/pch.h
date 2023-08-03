#pragma once
#pragma comment(lib, "assimp-vc143-mt.lib")

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <codecvt>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <windows.h>
#include <functional>
#include <DirectXMath.h>
#include <locale>
#include <memory>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#define FROM_SOLUTION_PATH_TO(path_) std::string(SOLUTION_DIR) + path_
#define FROM_RESOURCE_PATH_TO(path_) std::string(SOLUTION_DIR) + "Resources\\" + path_

#define RADIAN_CONST 0.0174532925f

// External lib
#include "json/json.h"
#include "imgui.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h" 
#include "assimp/scene.h"

#include "types.h"
#include "LogManager.h"
#include "ISingleton.h"
#include "IObjectPool.h"
#include "IManager.h"
//#include "WindowManager.h"
//#include "D3DManager.h"
//#include "SceneManager.h"
//#include "GameObjectManager.h"
//#include "ComponentManager.h"

class Config
{
public:
	static int GET_ENGINE_SETTING_WINDOW_WIDTH();

	static int SET_ENGINE_SETTING_WINDOW_WIDTH(int value);

	static int GET_ENGINE_SETTING_WINDOW_HEIGHT();

	static int SET_ENGINE_SETTING_WINDOW_HEIGHT(int value);

	static int GET_ENGINE_SETTING_WINDOW_VSYNC();

	static int GET_ENGINE_SETTING_WINDOW_FULLSCREEN();

	static float GET_ENGINE_SETTING_WINDOW_SCREENDEPTH();

	static float GET_ENGINE_SETTING_WINDOW_SCREENNEAR();

};

inline std::wstring GetWString(const std::string& string_)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	return converter.from_bytes(string_);
}