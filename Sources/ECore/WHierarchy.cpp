#include "PCH.h"
#include "OGameObject.h"
#include "OWorld.h"
#include "WHierarchy.h"
#include "WInspector.h"

WHierarchy::WHierarchy(const OWorld& InWorld, WInspector* InInspector)
	: OWidget()
{
	World = &InWorld;
	Inspector = InInspector;
}

WHierarchy::~WHierarchy()
{
}

void WHierarchy::Initialize()
{
	OWidget::Initialize();

	auto& GameObjects = World->GetGameObjects();
	for (const auto& GameObject : GameObjects)
	{
		GameObjectBlocks.insert(std::pair<OGameObject*, uint8>(GameObject.get(), 0));
	}
}

void WHierarchy::Release()
{
	OWidget::Release();
}

void WHierarchy::Start()
{
	OWidget::Start();
}

void WHierarchy::Tick()
{
	OWidget::Tick();
}

void WHierarchy::End()
{
	OWidget::End();
}

void WHierarchy::Render()
{
	// Create your GUI here
	ImGui::Begin("Hierarchy");

	for (auto& GameObjectBlock : GameObjectBlocks)
	{
		if (ImGui::Selectable(ToString(GameObjectBlock.first->GetName()).c_str(), GameObjectBlock.second))
		{
			GameObjectBlock.second = 1;
			Inspector->SetFocus(GameObjectBlock.first);
		}
	}
	

	ImGui::End();
}
