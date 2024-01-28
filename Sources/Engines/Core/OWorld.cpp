#include "PCH.h"
#include "GGizmo.h"
#include "GTestObject.h"
#include "ODirectX11.h"
#include "OWorld.h"

OWorld::OWorld()
	: Object()
{
}

OWorld::~OWorld()
{
}

void OWorld::Init()
{
	Object::Init();

	GTestObject& TestGameObject1 = TCreateGameObject<GTestObject>();

	GGizmo& Gizmo = TCreateGameObject<GGizmo>();

	for (const auto& GameObject : GameObjects)
	{
		GameObject->Init();
	}
}

void OWorld::Shutdown()
{
	Object::Shutdown();

	for (const auto& GameObject : GameObjects)
	{
		GameObject->Shutdown();
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