#pragma once

using WinProc = LRESULT CALLBACK(HWND, UINT, WPARAM, LPARAM);

using byte = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint = unsigned int;
using uchar = unsigned char;
using ushort = unsigned short;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct LightBufferType
{
	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;
	float	 padding;  
};

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};