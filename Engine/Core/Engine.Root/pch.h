#pragma once
#define WIN32_LEAN_AND_MEAN

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

#define FILE_PATH(path_) std::string(SOLUTION_DIR) + path_

#include "json/json.h"
#include "types.h"
#include "Logger.h"


class Config
{
public:
	static int GET_ENGINE_SETTING_WINDOW_WIDTH();

	static int SET_ENGINE_SETTING_WINDOW_WIDTH(int value);

	static int GET_ENGINE_SETTING_WINDOW_HEIGHT();

	static int SET_ENGINE_SETTING_WINDOW_HEIGHT(int value);
};