#include "PCH.h"
#include "WInspector.h"

WInspector::WInspector(const OWorld& InWorld)
	: OWidget(), InspectTarget(nullptr)
{
}

WInspector::~WInspector()
{
}

Object::EHandleResultType WInspector::Initialize()
{
	OWidget::Initialize();

	return EHandleResultType::Success;
}

void WInspector::Release()
{
	OWidget::Release();
}

void WInspector::Start()
{
	OWidget::Start();
}

void WInspector::Tick()
{
	OWidget::Tick();
}

void WInspector::End()
{
	OWidget::End();
}

void WInspector::Render()
{
	// Create your GUI here
	ImGui::Begin("Inspector");

	if(InspectTarget)
	{
		
	}

	ImGui::End();
}
