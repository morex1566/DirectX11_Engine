#pragma once

#include <directxmath.h>

using namespace DirectX;

struct VertexType
{
    XMFLOAT3 position;
    XMFLOAT2 texture;
    XMFLOAT3 normal;
};

struct MVPType
{
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
};

struct LightType
{
    XMFLOAT4 diffuse;
    XMFLOAT3 direction;
    float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
};