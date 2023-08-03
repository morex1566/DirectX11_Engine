#include "pch.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(float x_, float y_, float z_)
	: Light()
{
	_direction = XMFLOAT3(x_, y_, z_);
}

DirectionalLight::DirectionalLight(XMFLOAT4 diffuseColor_, XMFLOAT4 ambientColor_, XMFLOAT4 specularColor_, XMFLOAT3 direction_)
	: Light()
{
	_diffuseColor = diffuseColor_;
	_ambientColor = ambientColor_;
	_specularColor = specularColor_;

	_direction = direction_;
}

XMFLOAT3 DirectionalLight::GetDirection()
{
	return _direction;
}

void DirectionalLight::SetDirection(float x_, float y_, float z_)
{
	_direction = XMFLOAT3(x_, y_, z_);
}
