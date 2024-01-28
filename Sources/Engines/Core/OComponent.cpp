#include "PCH.h"
#include "OComponent.h"
#include "OGameObject.h"

OComponent::OComponent(const OGameObject* InOwner)
	: Object()
{
	Owner = InOwner;
}

OComponent::~OComponent()
{
}

void OComponent::Init()
{
	Object::Init();
}

void OComponent::Shutdown()
{
	Object::Shutdown();
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
