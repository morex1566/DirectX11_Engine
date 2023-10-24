#include "PCH.h"
#include "Camera.h"

Camera::~Camera()
{
}

void Camera::Start()
{
	

	GameObject::Start();
}

void Camera::Update()
{
	GameObject::Update();
}

XMMATRIX Camera::GetViewMatrix()
{
	return _viewMatrix;
}
