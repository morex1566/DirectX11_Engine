#include "PCH.h"
#include "OComponent.h"
#include "OGameObject.h"

OComponent::OComponent(const OGameObject* InOwner)
	: Object()
{
	Owner = InOwner;
}