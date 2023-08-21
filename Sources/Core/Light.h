#pragma once

#include "GameObject.h"

class Light : public GameObject
{
public:
	Light();
	Light(const Light&)		= default;
	~Light() override		= default;

	void SetDiffuseColor(float r_, float g_, float b_, float a_);
	void SetDirection(float x_, float y_, float z_);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

private:
	XMFLOAT4 _diffuseColor;
	XMFLOAT3 _direction;
};

