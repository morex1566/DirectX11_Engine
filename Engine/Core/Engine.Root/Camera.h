#pragma once

#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	~Camera() override = default;

	void Render();

	XMMATRIX GetViewMatrix();

private:
	XMMATRIX			_viewMatrix;
};