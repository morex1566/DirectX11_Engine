#include "PCH.h"
#include "OGameObject.h"
#include "CModel.h"


OGameObject::OGameObject()
	: Object()
{
	Transform = TAddComponent_Deprecated<CTransform>();
	
	CModel* Model = new CModel;
	TAttachComponent<CModel>(Model);
	TDetachComponent<CModel>(Model);
}

OGameObject::~OGameObject()
{
}

void OGameObject::Init()
{
	for (const auto& Component : Components_Deprecated)
	{
		Component->Init();
	}

	for (const auto& Child : Children_Deprecated)
	{
		Child->Init();
	}
}

void OGameObject::Shutdown()
{
	for (const auto& Component : Components_Deprecated)
	{
		Component->Shutdown();
	}

	for (const auto& Child : Children_Deprecated)
	{
		Child->Shutdown();
	}
}

void OGameObject::Start()
{
	for (const auto& Component : Components_Deprecated)
	{
		if (Component->IsEnable)
		{
			Component->Start();
		}
	}

	for (const auto& Child : Children_Deprecated)
	{
		if (Child->IsEnable)
		{
			Child->Start();
		}
	}
}

void OGameObject::Tick()
{
	for (const auto& Component : Components_Deprecated)
	{
		if (Component->IsEnable)
		{
			Component->Tick();
		}
	}

	for (const auto& Child : Children_Deprecated)
	{
		if (Child->IsEnable)
		{
			Child->Tick();
		}
	}
}

void OGameObject::End()
{
	for (const auto& Component : Components_Deprecated)
	{
		if (Component->IsEnable)
		{
			Component->End();
		}
	}

	for (const auto& Child : Children_Deprecated)
	{
		if (Child->IsEnable)
		{
			Child->End();
		}
	}
}

void OGameObject::SetWorld(OWorld* InWorld)
{
	// TODO : 이전 월드의 GameObject목록에서 제거

	// 월드 변경
	World = InWorld;
}

void OGameObject::SetParent(OGameObject* InParent)
{
	// TODO : 이전 부모의 Children목록에서 제거

	// 부모 재설정
	Parent = InParent;
}