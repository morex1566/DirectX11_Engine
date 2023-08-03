#include "pch.h"
#include "Transform.h"

Transform::Transform()
	: Component()
{
	_position = XMFLOAT3(0,0,0);
	_rotation = XMFLOAT3(0,0,0);
	_scale = XMFLOAT3(1,1,1);
	_up = XMFLOAT3(0, 1, 0);
	_lookAt = XMFLOAT3(0, 0, 1);
	_worldMatrix = XMMatrixIdentity();
}

void Transform::Update()
{
	Component::Update();

	// Update up and lookat vector.
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(_rotation.x * RADIAN_CONST, _rotation.y * RADIAN_CONST, _rotation.z * RADIAN_CONST);
	{
		XMVECTOR up = XMLoadFloat3(&_up);
		XMVECTOR lookAt = XMLoadFloat3(&_lookAt);

		XMStoreFloat3(&_up, XMVector3TransformCoord(up, rotationMatrix));
		XMStoreFloat3(&_lookAt, XMVector3TransformCoord(lookAt, rotationMatrix));
	}

	// Update the world matrix.
	{
		XMMATRIX translationM = XMMatrixTranslation(_position.x, _position.y, _position.z);
		XMMATRIX rotationM = XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z);
		XMMATRIX scaleM = XMMatrixScaling(_scale.x, _scale.y, _scale.z);

		_worldMatrix = translationM * rotationM * scaleM;
	}
}

XMFLOAT3 Transform::GetPosition()
{
	return _position;
}

XMFLOAT3 Transform::GetRotation()
{
	return _rotation;
}

XMFLOAT3 Transform::GetScale()
{
	return _scale;
}

XMFLOAT3 Transform::GetUp()
{
	return _up;
}

XMFLOAT3 Transform::GetLookAt()
{
	return _lookAt;
}

XMMATRIX Transform::GetWorldMatrix()
{
	return _worldMatrix;
}

void Transform::SetPosition(XMFLOAT3 position_)
{
	_position = position_;
}

void Transform::SetPosition(float x_, float y_, float z_)
{
	_position = XMFLOAT3(x_, y_, z_);
}

void Transform::SetRotation(XMFLOAT3 rotation_)
{
	_rotation = rotation_;
}

void Transform::SetRotation(float x_, float y_, float z_)
{
	_rotation = XMFLOAT3(x_, y_, z_);
}

void Transform::SetScale(XMFLOAT3 scale_)
{
	_scale = scale_;
}

void Transform::SetScale(float x_, float y_, float z_)
{
	_scale = XMFLOAT3(x_, y_, z_);
}
