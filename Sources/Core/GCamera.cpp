#include "PCH.h"
#include "CTransform.h"
#include "GCamera.h"
#include "OWindow.h"

uint8					GCamera::bIsScreenSizeChanged = 0;

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

	Transform->SetPosition(XMFLOAT3(0, 0, -5));

	return EHandleResultType::Success;
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