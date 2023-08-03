#pragma once

#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	~Transform() override = default;

	void Update() override;

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetLookAt();
	XMMATRIX GetWorldMatrix();

	void SetPosition(XMFLOAT3 position_);
	void SetPosition(float x_, float y_, float z_);
	void SetRotation(XMFLOAT3 rotation_);
	void SetRotation(float x_, float y_, float z_);
	void SetScale(XMFLOAT3 scale_);
	void SetScale(float x_, float y_, float z_);

private:
	XMFLOAT3 _position;
	XMFLOAT3 _rotation;
	XMFLOAT3 _scale;
	XMFLOAT3 _up;
	XMFLOAT3 _lookAt;
	XMMATRIX _worldMatrix;
};