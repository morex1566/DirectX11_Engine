// ReSharper disable CppClangTidyClangDiagnosticLanguageExtensionToken

#pragma once

#include <DirectXMath.h>

using namespace DirectX;

using byte		= unsigned char;
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint		= unsigned int;
using uchar		= unsigned char;
using ushort	= unsigned short;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct LightBufferType
{
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 specularColor;
	DirectX::XMFLOAT3 lightDirection;
	float			  specularIntensity;
};

struct CameraBufferType
{
	DirectX::XMFLOAT3 cameraPosition;
	float padding;
};

struct VertexType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;
};

struct TargaHeaderType
{
	uchar		data1[12];
	ushort		width;
	ushort		height;
	uchar		bpp;
	uchar		data2;
};