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

#define MAX_NUM_BONES_PER_VERTEX 4
#define ARRAY_SIZE_IN_ELEMENTS(ARR) (sizeof(ARR) / sizeof(ARR[0]))
struct FVertexBoneData
{
	UINT BoneIDs[MAX_NUM_BONES_PER_VERTEX] = {0, };
	float Weights[MAX_NUM_BONES_PER_VERTEX] = {0.0f, };

	void AddBoneData(UINT BoneID, float Weight)
	{
		for (UINT i = 0; i < ARRAY_SIZE_IN_ELEMENTS(BoneIDs); i++)
		{
			if (Weights[i] == 0.0f) 
			{
				BoneIDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
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