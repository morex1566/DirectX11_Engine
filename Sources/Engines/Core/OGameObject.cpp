#include "PCH.h"
#include "OGameObject.h"
#include "CModel.h"


OGameObject::OGameObject()
	: Object()
{
	Transform = new CTransform;
	TAttachComponent<CTransform>(Transform);
}

OGameObject::~OGameObject()
{
}

void OGameObject::Init()
{
	// Components
	{
		for (auto& Hash : Components)
		{
			for (OComponent* Component : Hash.second)
			{
				Component->Init();
			}
		}
	}

	// Children
	{
		for (auto& Hash : Children)
		{
			for (OGameObject* Child : Hash.second)
			{
				Child->Init();
			}
		}
	}
}

void OGameObject::Shutdown()
{
	// Components
	{
		for (auto& Hash : Components)
		{
			for (OComponent* Component : Hash.second)
			{
				Component->Shutdown();
				delete Component;
			}

			Hash.second.clear();
		}

		Components.clear();
	}

	// Children
	{
		for (auto& Hash : Children)
		{
			for (OGameObject* Child : Hash.second)
			{
				Child->Shutdown();
				delete Child;
			}

			Hash.second.clear();
		}

		Children.clear();
	}
}

void OGameObject::Start()
{
	// Components
	{
		for (auto& Hash : Components)
		{
			for (OComponent* Component : Hash.second)
			{
				if (Component->IsEnable)
				{
					Component->Start();
				}
			}
		}
	}

	// Children
	{
		for (auto& Hash : Children)
		{
			for (OGameObject* Child : Hash.second)
			{
				if (Child->IsEnable)
				{
					Child->Start();
				}
			}
		}
	}
}

void OGameObject::Tick()
{
	// Components
	{
		for (auto& Hash : Components)
		{
			for (OComponent* Component : Hash.second)
			{
				if (Component->IsEnable)
				{
					Component->Tick();
				}
			}
		}
	}

	// Children
	{
		for (auto& Hash : Children)
		{
			for (OGameObject* Child : Hash.second)
			{
				if (Child->IsEnable)
				{
					Child->Tick();
				}
			}
		}
	}
}

void OGameObject::End()
{
	// Components
	{
		for (auto& Hash : Components)
		{
			for (OComponent* Component : Hash.second)
			{
				if (Component->IsEnable)
				{
					Component->End();
				}
			}
		}
	}

	// Children
	{
		for (auto& Hash : Children)
		{
			for (OGameObject* Child : Hash.second)
			{
				if (Child->IsEnable)
				{
					Child->End();
				}
			}
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