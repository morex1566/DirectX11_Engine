
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 LitPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
}