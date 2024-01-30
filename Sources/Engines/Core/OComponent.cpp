#include "PCH.h"
#include "OComponent.h"
#include "OGameObject.h"

OComponent::OComponent(OGameObject* InOwner)
	: Object()
{
	Owner = InOwner;
}

OComponent::~OComponent()
{
}

void OComponent::SetOwner(OGameObject* InOwner)
{
	// TODO : 이전 오너의 Component목록에서 제거

	Owner = InOwner;
}
