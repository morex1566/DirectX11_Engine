#include "PCH.h"
#include "GGizmo.h"
#include "GTestObject.h"
#include "ODirectX11.h"
#include "OWorld.h"

#include "CTransform.h"

OWorld::OWorld()
	: Object()
{
}

OWorld::~OWorld()
{
}

void OWorld::Initialize()
{
	Object::Initialize();

	GTestObject& TestGameObject = TCreateGameObject<GTestObject>();

	GGizmo& Gizmo = TCreateGameObject<GGizmo>();

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