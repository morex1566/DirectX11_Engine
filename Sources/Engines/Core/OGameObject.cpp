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

void OGameObject::Init()
{
	Object::Init();

	for (const auto& Component : Components)
	{
		if (Component->CheckIsEnabled())
		{
			Component->Init();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->CheckIsEnabled())
		{
			Child->Init();
		}
	}
}

void OGameObject::Shutdown()
{
	Object::Shutdown();

	for (const auto& Component : Components)
	{
		Component->Shutdown();
	}

	for (const auto& Child : Children)
	{
		Child->Shutdown();
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