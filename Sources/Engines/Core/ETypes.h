#pragma once

struct FVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT2 Texture;
	XMFLOAT3 Normal;
	XMFLOAT3 Binormal;
	XMFLOAT3 Tangent;
	XMINT4   Bone;
	XMFLOAT4 Weight;

	FVertex() = default;
	FVertex(XMFLOAT3 InPosition, XMFLOAT4 InColor, XMFLOAT2 InTexture, XMFLOAT3 InNormal, XMFLOAT3 InBinormal, XMFLOAT3 InTangent)
	: Position(InPosition), Color(InColor), Texture(InTexture), Normal(InNormal), Binormal(InBinormal), Tangent(InTangent) {}
};

struct FBoneTransform
{
	XMMATRIX Offset;
	XMMATRIX Transform;
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