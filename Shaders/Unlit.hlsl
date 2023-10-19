cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VertexInput
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PixelInput UnlitVertexShader(VertexInput input)
{
    PixelInput output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
    // Store the input color for the pixel shader to use.
    output.color = input.color;

    return output;
}

float4 UnlitPixelShader(PixelInput input) : SV_TARGET
{
    return input.color;
}
