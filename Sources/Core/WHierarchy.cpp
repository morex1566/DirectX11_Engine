#include "PCH.h"
#include "OWorld.h"
#include "WHierarchy.h"

WHierarchy::WHierarchy(const OWorld& InWorld)
	: OWidget()
{
	World = &InWorld;
}

WHierarchy::~WHierarchy()
{
}

Object::EHandleResultType WHierarchy::Initialize()
{
	OWidget::Initialize();

	return EHandleResultType::Success;
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

	auto& GameObjects = World->GetGameObjects();
	for (const auto& GameObject : GameObjects)
	{
		ImGui::Selectable(ToString(GameObject->GetName()).c_str());
	}


	ImGui::End();
}
