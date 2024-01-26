#pragma once

struct FVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT2 Texture;
	XMFLOAT3 Normal;

	FVertex() = default;
	FVertex(XMFLOAT3 InPosition, XMFLOAT4 InColor, XMFLOAT2 InTexture, XMFLOAT3 InNormal) 
	: Position(InPosition), Color(InColor), Texture(InTexture), Normal(InNormal) {}
};

struct FWorldViewProjection
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;

	FWorldViewProjection() = default;
	FWorldViewProjection(const XMMATRIX& InWorld, const XMMATRIX& InView, const XMMATRIX& InProjection)
	: World(InWorld), View(InView), Projection(InProjection) {}
};