#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <string>
#include <locale>
#include <codecvt>
#include <memory>
#include <functional>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <DirectXMath.h>
#include <Windows.h>

#include "json/json.h"

using namespace DirectX;
using namespace std;

#include "Types.h"
#include "Utils.h"
#include "ISingleton.h"
#include "IManageable.h"
#include "IManager.h"
