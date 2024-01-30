#include "PCH.h"
#include "CTransform.h"
#include "GCamera.h"
#include "OWindow.h"
#include "OTime.h"
#include "OInput.h"

uint8					GCamera::bIsScreenSizeChanged = 0;
uint8					GCamera::bIsMoveAvailabled = 0;

GCamera::GCamera(const OWindow& InWindow)
	:   OGameObject(),
		FOV(3.141592654f / 4.0f),
        ScreenAspect(static_cast<float>(InWindow.GetClientScreenWidth()) / static_cast<float>(InWindow.GetClientScreenHeight())),
		ScreenNear(0.3f),
		ScreenDepth(1000.0f),
		MoveSpeed(1.0f),
		RotationSpeed(0.1f),
		ViewMatrix(),
        ProjectionMatrix(),
        OrthoMatrix()
{
	Window = &InWindow;

	GetTransform()->SetPosition(XMFLOAT3(0, 0, -5));
}

GCamera::~GCamera()
{
	Shutdown();
}

void GCamera::MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam)
{
	if (InMsg == WM_SIZE)
	{
		bIsScreenSizeChanged = 1;
	}
}

void GCamera::Init()
{
	OGameObject::Init();
}

void GCamera::Shutdown()
{
	OGameObject::Shutdown();
}

void GCamera::Start()
{
	OGameObject::Start();
}

void GCamera::Tick()
{
	OGameObject::Tick();

	if (bIsScreenSizeChanged == 1)
	{
		Resize();
		bIsScreenSizeChanged = 0;
	}

	// Update matices.
	{
		ViewMatrix = XMMatrixLookAtLH(
		Transform->GetPositionVector(), 
		Transform->GetLookAtVector(),
		Transform->GetUpVector());

		ProjectionMatrix = XMMatrixPerspectiveFovLH(FOV, ScreenAspect, ScreenNear, ScreenDepth);

		OrthoMatrix = XMMatrixOrthographicLH(
			static_cast<float>(Window->GetClientScreenWidth()),
			static_cast<float>(Window->GetClientScreenHeight()),
			ScreenNear,
			ScreenDepth);
	}

	// Movement acelleration.
	MoveSpeed = OInput::GetKeyDown(DIK_LSHIFT) ? 4.0f : 2.0f;

	// Move and rotate camera from keyboard input.
	if (OInput::GetMouseButtonDown(OInput::EMouseButton::Right))
	{
		Move();
		Rotate();
	}
}

void GCamera::End()
{
	OGameObject::End();
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

void GCamera::Resize()
{
	ScreenAspect = static_cast<float>(Window->GetClientScreenWidth()) / static_cast<float>(Window->GetClientScreenHeight());
}

void GCamera::Move()
{
	CTransform* Transform = GetTransform();
	{
		XMVECTOR DistanceVector = XMVectorSet(0, 0, 0, 0);
		{
			if (OInput::GetKeyDown(DIK_W))
			{
				DistanceVector += Transform->GetForwardVector();
			}

			if (OInput::GetKeyDown(DIK_S))
			{
				DistanceVector += Transform->GetBackVector();
			}

			if (OInput::GetKeyDown(DIK_A))
			{
				DistanceVector += Transform->GetLeftVector();
			}

			if (OInput::GetKeyDown(DIK_D))
			{
				DistanceVector += Transform->GetRightVector();
			}

			DistanceVector = XMVector3Normalize(DistanceVector);
		}

		XMFLOAT3 Distance;
		{
			XMStoreFloat3(&Distance, DistanceVector);

			Distance.x = Distance.x * MoveSpeed * OTime::GetDeltaTime();
			Distance.y = Distance.y * MoveSpeed * OTime::GetDeltaTime();
			Distance.z = Distance.z * MoveSpeed * OTime::GetDeltaTime();
		}

		Transform->Move(Distance);
	}
}

void GCamera::Rotate()
{
	CTransform*	 Transform = GetTransform();
	{
		Transform->Rotate(XMFLOAT3(0, RotationSpeed * OInput::GetMouseAxisX(), 0));
		Transform->Rotate(XMFLOAT3(RotationSpeed * OInput::GetMouseAxisY(), 0, 0));
	}
}
