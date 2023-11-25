#include "PCH.h"
#include "GCamera.h"
#include "GGizmo.h"

GGizmo::GGizmo(const GCamera& InCamera)
	: OGameObject()
{
	Camera = &InCamera;
}

GGizmo::~GGizmo()
{
}

void GGizmo::Initialize()
{
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
