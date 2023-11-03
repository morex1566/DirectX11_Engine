#include "PCH.h"
#include "OGameObject.h"

OGameObject::~OGameObject()
{
}

Object::EHandleResultType OGameObject::Initialize()
{
	return EHandleResultType::Success;
}

void OGameObject::Release()
{
}

void OGameObject::Start()
{
}

void OGameObject::Tick()
{
}

void OGameObject::End()
{
}
