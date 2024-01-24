#pragma once

struct FVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;

	FVertex() = default;
	FVertex(XMFLOAT3 InPosition, XMFLOAT4 InColor) : Position(InPosition), Color(InColor) {}
};

struct FMatrixBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;

	FMatrixBuffer() = default;
	FMatrixBuffer(const XMMATRIX& InWorld, const XMMATRIX& InView, const XMMATRIX& InProjection) : World(InWorld), View(InView), Projection(InProjection) {}
};