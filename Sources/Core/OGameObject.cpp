#include "PCH.h"
#include "CTransform.h"
#include "ODirectX11.h"
#include "OGameObject.h"


OGameObject::OGameObject()
	: Object()
{
	Transform = TAddComponent<CTransform>(*this);
}

OGameObject::~OGameObject()
{
}

Object::EHandleResultType OGameObject::Initialize()
{
	Object::Initialize();

	for (const auto& Component : Components)
	{
		if (Component->CheckIsEnbled())
		{
			Component->Initialize();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->CheckIsEnbled())
		{
			Child->Initialize();
		}
	}

	return EHandleResultType::Success;
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
		if (Component->CheckIsEnbled())
		{
			Component->Start();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->CheckIsEnbled())
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
		if (Component->CheckIsEnbled())
		{
			Component->Tick();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->CheckIsEnbled())
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
		if (Component->CheckIsEnbled())
		{
			Component->End();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->CheckIsEnbled())
		{
			Child->End();
		}
	}
}

CTransform* OGameObject::GetTransform() const
{
	return Transform;
}
