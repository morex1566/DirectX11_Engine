#include "PCH.h"
#include "CLine.h"
#include "CMesh.h"
#include "CShader.h"
#include "GCamera.h"
#include "GGizmo.h"
#include "GTestObject.h"
#include "ODirectX11.h"
#include "OWindow.h"
#include "OWorld.h"

OWorld::OWorld(const OWindow& InWindow, const ODirectX11& InDirectX11, const GCamera& InCamera)
	: Object()
{
	Window = &InWindow;
	DirectX11 = &InDirectX11;
	Camera = &InCamera;
}

OWorld::~OWorld()
{
}

void OWorld::Initialize()
{
	Object::Initialize();

	GTestObject& TestGameObject = TCreateGameObject<GTestObject>(*Camera, *Window, *DirectX11);
	GGizmo& Gizmo = TCreateGameObject<GGizmo>(*Camera, *Window, *DirectX11);

	for (const auto& GameObject : GameObjects)
	{
		GameObject->Initialize();
	}
}

void OWorld::Release()
{
	Object::Release();

	for (const auto& GameObject : GameObjects)
	{
		GameObject->Release();
	}
}

void OWorld::Start()
{
	Object::Start();

	for (const auto& GameObject : GameObjects)
	{
		GameObject->Start();
	}
}

void OWorld::Tick()
{
	Object::Tick();

	for (const auto& GameObject : GameObjects)
	{
		GameObject->Tick();
	}
}

void OWorld::End()
{
	Object::End();

	for (const auto& GameObject : GameObjects)
	{
		GameObject->End();
	}
}