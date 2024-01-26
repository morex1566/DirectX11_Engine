#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma warning(disable : 4267)
#pragma warning(disable : 26495)
#pragma warning(disable : 28251)

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winuser.h>
#include <memory>
#include <cstdint>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <d3d11.h>
#include <iostream>
#include <fstream>
#include <string_view>
#include <sstream>
#include <filesystem>
#include <directxmath.h>
#include <wrl/client.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using int8						= int8_t;
using uint8						= uint8_t;
using int16						= int16_t;
using uint16					= uint16_t;
using int32						= int32_t;
using uint32					= uint32_t;
using int64						= int64_t;
using uint64					= uint64_t;

#include "ETypes.h"
#include "EMacros.h"
#include "EUtls.h"
#include "SApplication.h"
#include "SConsole.h"
#include "SReflection.h"