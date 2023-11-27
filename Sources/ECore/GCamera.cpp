#include "PCH.h"
#include "CTransform.h"
#include "GCamera.h"
#include "OWindow.h"
#include "SConsole.h"
#include "SInput.h"

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

	// toggle on/off camera move.
	if (SInput::GetKeyDown(VK_CONTROL) &&
		SInput::GetKeyDown(VK_V))
	{
		SConsole::Log("Camera move is " + std::to_string(bIsMoveAvailabled));
		bIsMoveAvailabled = 1;
	}

	// Move camera from keyboard input.
	if (bIsMoveAvailabled)
	{
		Move();
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
		if (SInput::GetKeyDown(VK_W))
		{
			Transform->Move(XMFLOAT3(0, 0, 0.5));
		}

		if (SInput::GetKeyDown(VK_A))
		{
			Transform->Move(XMFLOAT3(-0.5, 0, 0));
		}

		if (SInput::GetKeyDown(VK_S))
		{
			Transform->Move(XMFLOAT3(0, 0, -0.5));
		}

		if (SInput::GetKeyDown(VK_D))
		{
			Transform->Move(XMFLOAT3(0.5, 0, 0));
		}
	}
}
