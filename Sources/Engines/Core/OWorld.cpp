#include "PCH.h"
#include "GGizmo.h"
#include "GTestObject.h"
#include "ODirectX11.h"
#include "OWorld.h"

OWorld::OWorld()
	: Object()
{
}

OWorld::~OWorld()
{
}

void OWorld::Init()
{
	Object::Init();

	GTestObject& TestGameObject1 = TCreateGameObject<GTestObject>();
	GGizmo& Gizmo_Deprecated = TCreateGameObject<GGizmo>();

	for (const auto& GameObject : GameObjects_Deprecated)
	{
		GameObject->Init();
	}

	GTestObject* TestGameObject = new GTestObject;
	TAttachGameObject<GTestObject>(TestGameObject);

	GGizmo* Gizmo = new GGizmo;
	TAttachGameObject<GGizmo>(Gizmo);

	TDetachGameObject(TestGameObject);
	TDetachGameObject(Gizmo);

	// GameObjectHashMap
	{
		for (auto& Hash : GameObjects)
		{
			for (OGameObject* GameObject : Hash.second)
			{
				GameObject->Init();
			}
		}
	}
}

void OWorld::Shutdown()
{
	Object::Shutdown();

	for (const auto& GameObject : GameObjects_Deprecated)
	{
		GameObject->Shutdown();
	}

	// GameObjectHashMap
	{
		for (auto& Hash : GameObjects)
		{
			for (OGameObject* GameObject : Hash.second)
			{
				GameObject->Shutdown();
				delete GameObject;
			}

			Hash.second.clear();
		}

		GameObjects.clear();
	}
}

void OWorld::Start()
{
	Object::Start();

	for (const auto& GameObject : GameObjects_Deprecated)
	{
		GameObject->Start();
	}

	// GameObjectHashMap
	{
		for (auto& Hash : GameObjects)
		{
			for (OGameObject* GameObject : Hash.second)
			{
				if (GameObject->IsEnable)
				{
					GameObject->Start();
				}
			}
		}
	}
}

void OWorld::Tick()
{
	Object::Tick();

	for (const auto& GameObject : GameObjects_Deprecated)
	{
		GameObject->Tick();
	}

	// GameObjectHashMap
	{
		for (auto& Hash : GameObjects)
		{
			for (OGameObject* GameObject : Hash.second)
			{
				if (GameObject->IsEnable)
				{
					GameObject->Tick();
				}
			}
		}
	}
}

void OWorld::End()
{
	Object::End();

	for (const auto& GameObject : GameObjects_Deprecated)
	{
		GameObject->End();
	}

	// GameObjectHashMap
	{
		for (auto& Hash : GameObjects)
		{
			for (OGameObject* GameObject : Hash.second)
			{
				if (GameObject->IsEnable)
				{
					GameObject->End();
				}
			}
		}
	}
}