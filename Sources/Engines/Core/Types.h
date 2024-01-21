#pragma once

#include <directxmath.h>

using namespace DirectX;

struct Vertex
{
    XMFLOAT3 position;
    XMFLOAT2 texture;
    XMFLOAT3 normal;
};