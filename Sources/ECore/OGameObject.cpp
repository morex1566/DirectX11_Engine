#include "PCH.h"
#include "CTransform.h"
#include "ODirectX11.h"
#include "OGameObject.h"


OGameObject::OGameObject()
	: Object()
{
	Transform = TAddComponent<CTransform>();
}

OGameObject::~OGameObject()
{
}

void OGameObject::Initialize()
{
	Object::Initialize();

	for (const auto& Component : Components)
	{
		if (Component->CheckIsEnabled())
		{
			Component->Initialize();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->CheckIsEnabled())
		{
			Child->Initialize();
		}
	}
}

void OGameObject::Release()
{
	Object::Release();

	for (const auto& Component : Components)
	{
		Component->Release();
	}

	for (const auto& Child : Children)
	{
		Child->Release();
	}
}

void OGameObject::Start()
{
	Object::Start();

	for (const auto& Component : Components)
	{
		if (Component->CheckIsEnabled())
		{
			Component->Start();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->CheckIsEnabled())
		{
			Child->Start();
		}
	}
}

void OGameObject::Tick()
{
	Object::Tick();

	for (const auto& Component : Components)
	{
		if (Component->CheckIsEnabled())
		{
			Component->Tick();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->CheckIsEnabled())
		{
			Child->Tick();
		}
	}
}

void OGameObject::End()
{
	Object::End();

	for (const auto& Component : Components)
	{
		if (Component->CheckIsEnabled())
		{
			Component->End();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->CheckIsEnabled())
		{
			Child->End();
		}
	}
}

CTransform* OGameObject::GetTransform() const
{
	return Transform;
}
