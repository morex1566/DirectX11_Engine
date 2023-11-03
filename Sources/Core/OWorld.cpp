#include "PCH.h"
#include "OWorld.h"

OWorld::~OWorld()
{
}

Object::EHandleResultType OWorld::Initialize()
{
	return EHandleResultType::Success;
}

void OWorld::Release()
{
	for (const auto& GameObject : GameObjects)
	{
		GameObject->Release();
	}
}

void OWorld::Start()
{
	for (const auto& GameObject : GameObjects)
	{
		GameObject->Start();
	}
}

void OWorld::Tick()
{
	for (const auto& GameObject : GameObjects)
	{
		GameObject->Tick();
	}
}

void OWorld::End()
{
	for (const auto& GameObject : GameObjects)
	{
		GameObject->End();
	}
}