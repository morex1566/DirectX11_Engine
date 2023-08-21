#pragma once

#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	Camera(const Camera&)		= default;
	~Camera() override			= default;

	void SetPosition(float x_, float y_, float z_);
	void SetRotation(float x_, float y_, float z_);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	XMMATRIX GetViewMatrix();
	void GetViewMatrix(XMMATRIX&);

private:
	XMFLOAT3 _position;
	XMFLOAT3 _rotation;
	XMFLOAT3 _scale;
	XMMATRIX _viewMatrix;
};

