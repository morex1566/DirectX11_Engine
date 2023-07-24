#include "pch.h"
#include "Transform.h"

Transform::Transform()
{
	_Position = XMFLOAT3(0,0,0);
	_Rotation = XMFLOAT3(0,0,0);
	_Scale = XMFLOAT3(1,1,1);
}