// ReSharper disable CppClangTidyClangDiagnosticLanguageExtensionToken

#pragma once

#include <DirectXMath.h>

using byte = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = unsigned __int32;
using int64 = unsigned __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct LightBufferType
{
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 lightDirection;
	float padding;
};