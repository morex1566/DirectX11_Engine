#define MAX_BONE_COUNT 100

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer BoneTransform
{
    float4x4 offset[MAX_BONE_COUNT];
    float4x4 transform[MAX_BONE_COUNT];
};

struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;
    int4 boneID : TEXCOORD1;
    float4 weight : TEXCOORD2;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 binormal : BINORMAL;
    float3 tangent : TANGENT;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LitVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    float4x4 boneTransform = transform[input.boneID.x] * input.weight.x;
             boneTransform += transform[input.boneID.y] * input.weight.y;
             boneTransform += transform[input.boneID.z] * input.weight.z;
             boneTransform += transform[input.boneID.w] * input.weight.w;

    // 스키닝
    output.position = mul(input.position, boneTransform);
    output.normal = mul(float4(input.normal, 0.0f), boneTransform).xyz;

    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.tex = input.tex;

    return output;
}