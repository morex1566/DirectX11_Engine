#include "pch.h"
#include "Camera.h"

Camera::Camera(): _position(), _rotation(), _scale(), _viewMatrix()
{
}

void Camera::SetPosition(float x_, float y_, float z_)
{
	_position = XMFLOAT3(x_, y_, z_);
	return;
}


void Camera::SetRotation(float x_, float y_, float z_)
{
	_rotation = XMFLOAT3(x_, y_, z_);
	return;
}


XMFLOAT3 Camera::GetPosition()
{
	return _position;
}


XMFLOAT3 Camera::GetRotation()
{
	return _rotation;
}


void Camera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = _position.x;
	position.y = _position.y;
	position.z = _position.z;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = _rotation.x * DEGREE_TO_RADIAN;
	yaw = _rotation.y * DEGREE_TO_RADIAN;
	roll = _rotation.z * DEGREE_TO_RADIAN;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

XMMATRIX Camera::GetViewMatrix()
{
	return _viewMatrix;
}


void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = _viewMatrix;
	return;
}
