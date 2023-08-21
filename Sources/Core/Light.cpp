#include "pch.h"
#include "Light.h"

Light::Light(): _diffuseColor(), _direction()
{
}

void Light::SetDiffuseColor(float r_, float g_, float b_, float a_)
{
	_diffuseColor = XMFLOAT4(r_, g_, b_, a_);
	return;
}


void Light::SetDirection(float x_, float y_, float z_)
{
	_direction = XMFLOAT3(x_, y_, z_);
	return;
}


XMFLOAT4 Light::GetDiffuseColor()
{
	return _diffuseColor;
}


XMFLOAT3 Light::GetDirection()
{
	return _direction;
}