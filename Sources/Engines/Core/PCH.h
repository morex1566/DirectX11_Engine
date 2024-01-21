#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma warning (disable : 26495)

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <Windows.h>
#include <winuser.h>
#include <vector>
#include <queue>
#include <memory>
#include <d3d11.h>
#include <directxmath.h>

#include "Macros.h"
#include "Types.h"
#include "Utls.h"
#include "Config.h"
#include "IEnable.h"
#include "ITag.h"
#include "IName.h"
#include "Console.h"

using namespace DirectX;