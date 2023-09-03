#pragma once

#pragma warning(disable: 28251)
#pragma warning(disable: 26451)
#pragma warning(disable: 4101)
#pragma warning(disable: 4267)

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

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <Windows.h>
#include <wrl/client.h>

#include "json/json.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

#include "Types.h"
#include "Utils.h"
#include "ISingleton.h"
#include "IManageable.h"
#include "IManager.h"
