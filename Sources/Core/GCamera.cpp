#include "PCH.h"
#include "GCamera.h"
#include "OWindow.h"

uint8					GCamera::bIsScreenSizeChanged = 0;

GCamera::GCamera(const OWindow& InWindow):  Position(), Rotation(), FOV(3.141592654f / 4.0f),
                                            ScreenAspect(static_cast<float>(InWindow.GetWindowScreenWidth()) / static_cast<float>(InWindow.GetWindowScreenHeight())),
										    ScreenNear(0.3f),
										    ScreenDepth(1000.0f),
											ViewMatrix(),
                                            ProjectionMatrix(),
                                            OrthoMatrix()
{
	Window = &InWindow;
}

GCamera::~GCamera()
{
}

Object::EHandleResultType GCamera::MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam)
{
	if (InMsg == WM_SIZE)
	{
		bIsScreenSizeChanged = 1;
	}

	return EHandleResultType::Success;
}

Object::EHandleResultType GCamera::Initialize()
{
	OGameObject::Initialize();

	return EHandleResultType::Success;
}

void GCamera::Release()
{
}

void GCamera::Start()
{
}

void GCamera::Tick()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	if (bIsScreenSizeChanged == 1)
	{
		Resize();
		bIsScreenSizeChanged = 0;
	}


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = Position.x;
	position.y = Position.y;
	position.z = Position.z;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = Rotation.x * 0.0174532925f;
	yaw = Rotation.y * 0.0174532925f;
	roll = Rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	ViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	ProjectionMatrix = XMMatrixPerspectiveFovLH(FOV, ScreenAspect, ScreenNear, ScreenDepth);

	OrthoMatrix = XMMatrixOrthographicLH(
	static_cast<float>(Window->GetWindowScreenWidth()),
	static_cast<float>(Window->GetWindowScreenHeight()),
	ScreenNear,
	ScreenDepth);

}

void GCamera::End()
{
}

XMMATRIX GCamera::GetViewMatrix() const
{
	return ViewMatrix;
}

XMMATRIX GCamera::GetProjectionMatrix() const
{
	return ProjectionMatrix;
}

XMMATRIX GCamera::GetOrthoMatrix() const
{
	return OrthoMatrix;
}

XMFLOAT3 GCamera::GetPosition() const
{
	return Position;
}

XMFLOAT3 GCamera::GetRotation() const
{
	return Rotation;
}

float GCamera::GetFOV() const
{
	return FOV;
}

float GCamera::GetScreenAspect() const
{
	return ScreenAspect;
}

float GCamera::GetScreenNear() const
{
	return ScreenNear;
}

float GCamera::GetScreenDepth() const
{
	return ScreenDepth;
}

void GCamera::SetPosition(XMFLOAT3 InPosition)
{
	Position = InPosition;
}

void GCamera::Resize()
{
	ScreenAspect = static_cast<float>(Window->GetWindowScreenWidth()) / static_cast<float>(Window->GetWindowScreenHeight());
}