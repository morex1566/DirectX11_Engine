#pragma once

struct FVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

struct FMatrixBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
};