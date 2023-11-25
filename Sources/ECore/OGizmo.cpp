#include "PCH.h"
#include "OGizmo.h"

OGizmo::OGizmo(const GCamera& InCamera)
	: Object()
{
	Camera = &InCamera;
}

OGizmo::~OGizmo()
{
}

void OGizmo::Initialize()
{
	Object::Initialize();
}

void OGizmo::Release()
{
	Object::Release();
}

void OGizmo::Start()
{
	Object::Start();
}

void OGizmo::Tick()
{
	Object::Tick();
}

void OGizmo::End()
{
	Object::End();
}
