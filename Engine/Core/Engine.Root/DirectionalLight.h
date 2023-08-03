#pragma once

#include "Light.h"

class DirectionalLight final : public Light
{
public:
	DirectionalLight() = default;
	DirectionalLight(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f);
	DirectionalLight(XMFLOAT4 diffuseColor_, XMFLOAT4 ambientColor_, XMFLOAT4 specularColor_, XMFLOAT3 direction_);
	~DirectionalLight() override = default;

	XMFLOAT3	GetDirection();
	void		SetDirection(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f);

private:
	XMFLOAT3			_direction;

};