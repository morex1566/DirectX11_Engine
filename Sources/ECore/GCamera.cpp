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
		ViewMatrix(),
        ProjectionMatrix(),
        OrthoMatrix()
{
	Window = &InWindow;

	GetTransform()->SetPosition(XMFLOAT3(0, 0, -5));
}

GCamera::~GCamera()
{
}

void GCamera::MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam)
{
	if (InMsg == WM_SIZE)
	{
		bIsScreenSizeChanged = 1;
	}
}

void GCamera::Initialize()
{
	OGameObject::Initialize();
}

void GCamera::Release()
{
	OGameObject::Release();
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
		if (OInput::GetKeyDown(DIK_W))
		{
			XMFLOAT3 Forward = Transform->GetForward();
			{
				Forward.x = Forward.x * 2.0f * OTime::GetDeltaTime();
				Forward.y = Forward.y * 2.0f * OTime::GetDeltaTime();
				Forward.z = Forward.z * 2.0f * OTime::GetDeltaTime();
			}


			Transform->Move(Forward);
		}

		if (OInput::GetKeyDown(DIK_A))
		{
			XMFLOAT3 Left = Transform->GetLeft();
			{
				Left.x = Left.x * 2.0f * OTime::GetDeltaTime();
				Left.y = Left.y * 2.0f * OTime::GetDeltaTime();
				Left.z = Left.z * 2.0f * OTime::GetDeltaTime();
			}

			Transform->Move(Left);
		}

		if (OInput::GetKeyDown(DIK_S))
		{
			XMFLOAT3 Back = Transform->GetBack();
			{
				Back.x = Back.x * 2.0f * OTime::GetDeltaTime();
				Back.y = Back.y * 2.0f * OTime::GetDeltaTime();
				Back.z = Back.z * 2.0f * OTime::GetDeltaTime();
			}

			Transform->Move(Back);
		}

		if (OInput::GetKeyDown(DIK_D))
		{
			XMFLOAT3 Right = Transform->GetRight();
			{
				Right.x = Right.x * 2.0f * OTime::GetDeltaTime();
				Right.y = Right.y * 2.0f * OTime::GetDeltaTime();
				Right.z = Right.z * 2.0f * OTime::GetDeltaTime();
			}

			Transform->Move(Right);
		}
	}
}

void GCamera::Rotate()
{
	CTransform*	 Transform = GetTransform();
	{
		Transform->Rotate(XMFLOAT3(0, OInput::GetMouseAxisX() * 0.1, 0));
		Transform->Rotate(XMFLOAT3(OInput::GetMouseAxisY() * 0.1, 0, 0));
	}
}
