#include "PCH.h"
#include "CLine.h"
#include "CShader.h"
#include "GCamera.h"
#include "GGizmo.h"
#include "ODirectX11.h"
#include "OWindow.h"

GGizmo::GGizmo(const GCamera& InCamera, const OWindow& InWindow, const ODirectX11& InDirectX11)
	: OGameObject()
{
	Camera = &InCamera;
	DirectX11 = &InDirectX11;
	Window = &InWindow;
}

GGizmo::~GGizmo()
{
}

void GGizmo::Initialize()
{
	TAddComponent<CLine>(*this, *DirectX11);
	TAddComponent<CShader>(*this, *Window, *DirectX11, *Camera);


	Object::Initialize();
}

void GGizmo::Release()
{
	Object::Release();
}

void GGizmo::Start()
{
	Object::Start();
}

void GGizmo::Tick()
{
	Object::Tick();
}

void GGizmo::End()
{
	Object::End();
}
