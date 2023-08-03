#include "pch.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"

Camera::Camera()
	: GameObject()
{
	_viewMatrix = XMMatrixIdentity();
}

void Camera::Render()
{
	Transform* transform = GetTransform();

	XMVECTOR upVector;
	{
		XMFLOAT3 up = transform->GetUp();
		upVector = XMLoadFloat3(&up);
	}

	XMVECTOR positionVector;
	{
		XMFLOAT3 position = transform->GetPosition();
		positionVector = XMLoadFloat3(&position);
	}

	XMVECTOR lookAtVector;
	{
		XMFLOAT3 lookAt = transform->GetLookAt();
		lookAtVector = XMLoadFloat3(&lookAt);

		lookAtVector = XMVectorAdd(positionVector, lookAtVector);
	}

	_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

XMMATRIX Camera::GetViewMatrix()
{
	return _viewMatrix;
}
