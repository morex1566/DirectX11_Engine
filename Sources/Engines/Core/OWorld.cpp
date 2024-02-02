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


	GTestObject* TestGameObject = new GTestObject;
	TAttachGameObject<GTestObject>(TestGameObject);

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