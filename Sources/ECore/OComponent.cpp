#include "PCH.h"
#include "OComponent.h"
#include "OGameObject.h"

OComponent::OComponent(const OGameObject& InOwner)
	: Object()
{
	Owner = &InOwner;
}

OComponent::~OComponent()
{
}

Object::EHandleResultType OComponent::Initialize()
{
	return Object::Initialize();
}

void OComponent::Release()
{
	Object::Release();
}

void OComponent::Start()
{
	Object::Start();
}

void OComponent::Tick()
{
	Object::Tick();
}

void OComponent::End()
{
	Object::End();
}
