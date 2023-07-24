#pragma once

#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	~Transform() override = default;

public:
	XMFLOAT3 _Position;
	XMFLOAT3 _Rotation;
	XMFLOAT3 _Scale;
};