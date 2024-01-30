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
	for (const auto& Component : Components)
	{
		Component->Init();
	}

	for (const auto& Child : Children)
	{
		Child->Init();
	}
}

void OGameObject::Shutdown()
{
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
	for (const auto& Component : Components)
	{
		if (Component->IsEnable)
		{
			Component->Start();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->IsEnable)
		{
			Child->Start();
		}
	}
}

void OGameObject::Tick()
{
	for (const auto& Component : Components)
	{
		if (Component->IsEnable)
		{
			Component->Tick();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->IsEnable)
		{
			Child->Tick();
		}
	}
}

void OGameObject::End()
{
	for (const auto& Component : Components)
	{
		if (Component->IsEnable)
		{
			Component->End();
		}
	}

	for (const auto& Child : Children)
	{
		if (Child->IsEnable)
		{
			Child->End();
		}
	}
}