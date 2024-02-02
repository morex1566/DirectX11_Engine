#pragma once

struct FVertex
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT2 Texture;
	XMFLOAT3 Normal;
	XMFLOAT3 Binormal;
	XMFLOAT3 Tangent;
	XMINT4   BoneID = { -1, -1, -1, -1 };
	XMFLOAT4 Weight = { 0.0f, 0.0f, 0.0f, 0.0f };

	FVertex() = default;
	FVertex(XMFLOAT3 InPosition, XMFLOAT4 InColor, XMFLOAT2 InTexture, XMFLOAT3 InNormal, XMFLOAT3 InBinormal, XMFLOAT3 InTangent)
	: Position(InPosition), Color(InColor), Texture(InTexture), Normal(InNormal), Binormal(InBinormal), Tangent(InTangent) {}

	void AddBone(int InBoneID, float InWeight)
	{
		if (Weight.x == 0.0f)
		{
			BoneID.x = InBoneID;
			Weight.x = InWeight;
		}
		else
		if (Weight.y == 0.0f)
		{
			BoneID.y = InBoneID;
			Weight.y = InWeight;
		}
		else
		if (Weight.z == 0.0f)
		{
			BoneID.z = InBoneID;
			Weight.z = InWeight;
		}
		else
		if (Weight.w == 0.0f)
		{
			BoneID.w = InBoneID;
			Weight.w = InWeight;
		}
	}
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